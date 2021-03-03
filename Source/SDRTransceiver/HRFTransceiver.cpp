#include "../SDRUtility/SDRUtility.h"
#include "HRFTransceiver.h"
#include "../HRFCmdParser/HRFCmdParser.h"
#include "../SDRException/SDRException.h"


volatile uint32_t byte_count = 0;

struct timeval time_start;
struct timeval t_start;

bool do_exit = false;

static int gettimeofday(struct timeval* tv, void* ignored)
{
	FILETIME ft;
	unsigned __int64 tmp = 0;
	if (NULL != tv) {
		GetSystemTimeAsFileTime(&ft);
		tmp |= ft.dwHighDateTime;
		tmp <<= 32;
		tmp |= ft.dwLowDateTime;
		tmp /= 10;
		tmp -= 11644473600000000Ui64;
		tv->tv_sec = (long)(tmp / 1000000UL);
		tv->tv_usec = (long)(tmp % 1000000UL);
	}
	return 0;
}

static float
TimevalDiff(const struct timeval* a, const struct timeval* b)
{
	return (a->tv_sec - b->tv_sec) + 1e-6f * (a->tv_usec - b->tv_usec);
}

bool WINAPI sighandler(int signum)
{
	if (CTRL_C_EVENT == signum) {
		fprintf(stderr, "Caught signal %d\n", signum);
		do_exit = true;
		return TRUE;
	}
	return FALSE;
}

HRFTransceiver::HRFTransceiver()
{
	m_params = nullptr;
	m_fileToSend = std::ifstream();
	SetConsoleCtrlHandler((PHANDLER_ROUTINE)sighandler, TRUE);
}

HRFTransceiver::HRFTransceiver(const std::shared_ptr<HRFUtil::HRFParams> params, const std::string filename)
{
	m_params = params;

	if (!m_fileToSend.is_open()) 
	{
		m_fileToSend = std::ifstream();
		m_fileToSend.open(filename, std::ios::binary);
	}
}

void HRFTransceiver::Receive()
{

}

void HRFTransceiver::Transfer(hackrf_device* device)
{

	int result = 0;
	int exit_code = 0;

	struct timeval t_end;
	float time_diff;

	result = hackrf_set_txvga_gain(device, m_params->txvga_gain);
	result |= hackrf_start_tx(device, tx_callback, NULL);

	EnableParamsForRXTX(device);

	if (result != HACKRF_SUCCESS)
	{
		std::cerr << "Transfer failed: ";
		throw SDRException((hackrf_error)result);
	}

	gettimeofday(&t_start, NULL);
	gettimeofday(&time_start, NULL);

	fprintf(stderr, "Stop with Ctrl-C\n");
	while ((hackrf_is_streaming(device) == HACKRF_TRUE) &&
		(do_exit == false))
	{
		uint32_t byte_count_now;
		struct timeval time_now;
		float rate = 0;
		float time_difference = 0;
		if (m_params->stream_size > 0) {

		}
		else {
			sleep(1);
			gettimeofday(&time_now, NULL);
			byte_count_now = byte_count;
			byte_count = 0;

			time_difference = TimevalDiff(&time_now, &time_start);
			rate = (float)byte_count_now / time_difference;
			if (byte_count_now == 0 && m_params->hw_sync == true && m_params->hw_sync_enable != 0) {
				fprintf(stderr, "Waiting for sync...\n");
			}
			else {
				fprintf(stderr, "%4.2f MiB / %5.3f sec = %4.2f MiB/second\n",
					(byte_count_now / 1e6f), time_difference, (rate / 1e6f));
			}

			time_start = time_now;

			if (byte_count_now == 0 && (m_params->hw_sync == false || m_params->hw_sync_enable == 0)) {
				exit_code = EXIT_FAILURE;
				fprintf(stderr, "\nCouldn't transfer any bytes for one second.\n");
				break;
			}
		}
	}

	result = hackrf_is_streaming(device);
	if (do_exit)
	{
		fprintf(stderr, "\nExiting...\n");
	}
	else {
		fprintf(stderr, "\nExiting... hackrf_is_streaming() result: %s (%d)\n", hackrf_error_name((hackrf_error)result), result);
	}

	gettimeofday(&t_end, NULL);
	time_diff = TimevalDiff(&t_end, &t_start);
	fprintf(stderr, "Total time: %5.5f s\n", time_diff);
}

int HRFTransceiver::tx_callback(hackrf_transfer* transfer)
{
	if (m_fileToSend.is_open() && m_fileToSend.good() && !m_fileToSend.eof())
	{
		size_t buffer_length = transfer->valid_length;
		byte_count += buffer_length / 8;
		size_t bits_to_read = buffer_length / 8;
		char* read_buffer = (char*)malloc(bits_to_read);
		m_fileToSend.read(read_buffer, bits_to_read);

		size_t bits_read = m_fileToSend.gcount();
		make_psk4_buffer(transfer->buffer, read_buffer, bits_read);
		if (bits_read != bits_to_read)
		{
			if (m_params->repeat)
			{
				size_t extra_bits = bits_to_read - bits_read;
				std::cerr << "Input file end reached. Rewind to beginning" << std::endl;
				m_fileToSend.seekg(0);
				m_fileToSend.read(read_buffer + bits_read, extra_bits);
				make_psk4_buffer(transfer->buffer + bits_read * 8, read_buffer + bits_read, extra_bits);
				free(read_buffer);
				return 0;
			}
			else
			{
				std::cout << "End of file reached" << std::endl;
				free(read_buffer);
				m_fileToSend.close();
				return -1;
			}
		}
		else
		{
			free(read_buffer);
			return 0;
		}
	}
	else
	{
		m_fileToSend.close();
		return -1;
	}
}

void HRFTransceiver::make_psk4_buffer(uint8_t* buffer, char* data, size_t size)
{
	for (int i = 0; i < size; i++)
	{
		uint8_t modulation_symbol;
		for (int j = 7; j >= 0; j--)
		{
			modulation_symbol = (data[i] & (1 << j)) != 0 ? 127 : -128;
			buffer[i * 8 + (7 - j)] = modulation_symbol;
		}
	}
}

void HRFTransceiver::make_psk2_buffer(uint8_t* buffer, char* data, size_t size)
{
	// for bpsk, buffer should be x2 in size and every odd bit should be equal 1 (127, I = const)
	// and every even bit should be modulated (Q = Q(t))
	
	// for bpsk, code below is invalid
	/*memset(buffer, 127, size);
	for (int i = 0; i < size; i++)
	{
		for (int j = 7; j >= 0; j--)
		{
			uint8_t modulation_symbol = (data[i] & (1 << j)) != 0 ? 127 : -128;
			buffer[i * 8 + j] = modulation_symbol;
		}
	}*/
}
void HRFTransceiver::EnableParamsForRXTX(hackrf_device* device)
{

	HRFUtil::t_u64toa ascii_u64_data1;
	HRFUtil::t_u64toa ascii_u64_data2;

	int result = 0;

	if (m_params->automatic_tuning) {
		fprintf(stderr, "call hackrf_set_freq(%s Hz/%.03f MHz)\n",
			HRFCmdParser::u64toa(m_params->freq_hz, &ascii_u64_data1), ((double)m_params->freq_hz / (double)FREQ_ONE_MHZ));
		result = hackrf_set_freq(device, m_params->freq_hz);
		if (result != HACKRF_SUCCESS) {
			fprintf(stderr, "hackrf_set_freq() failed: %s (%d)\n", hackrf_error_name((hackrf_error)result), result);
			HRFCmdParser::usage();
			throw SDRException((hackrf_error)result);
		}
	}
	else {
		fprintf(stderr, "call hackrf_set_freq_explicit() with %s Hz IF, %s Hz LO, %s\n",
			HRFCmdParser::u64toa(m_params->if_freq_hz, &ascii_u64_data1),
			HRFCmdParser::u64toa(m_params->lo_freq_hz, &ascii_u64_data2),
			hackrf_filter_path_name((rf_path_filter)m_params->image_reject_selection));
		result = hackrf_set_freq_explicit(device, m_params->if_freq_hz, m_params->lo_freq_hz,
			(rf_path_filter)m_params->image_reject_selection);
		if (result != HACKRF_SUCCESS) {
			fprintf(stderr, "hackrf_set_freq_explicit() failed: %s (%d)\n",
				hackrf_error_name((hackrf_error)result), result);
			HRFCmdParser::usage();
			throw SDRException((hackrf_error)result);
		}
	}

	if (m_params->amp) {
		fprintf(stderr, "call hackrf_set_amp_enable(%u)\n", m_params->amp_enable);
		result = hackrf_set_amp_enable(device, (uint8_t)m_params->amp_enable);
		if (result != HACKRF_SUCCESS) {
			fprintf(stderr, "hackrf_set_amp_enable() failed: %s (%d)\n", hackrf_error_name((hackrf_error)result), result);
			HRFCmdParser::usage();
			throw SDRException((hackrf_error)result);
		}
	}

	if (m_params->antenna) {
		fprintf(stderr, "call hackrf_set_antenna_enable(%u)\n", m_params->antenna_enable);
		result = hackrf_set_antenna_enable(device, (uint8_t)m_params->antenna_enable);
		if (result != HACKRF_SUCCESS) {
			fprintf(stderr, "hackrf_set_antenna_enable() failed: %s (%d)\n", hackrf_error_name((hackrf_error)result), result);
			HRFCmdParser::usage();
			throw SDRException((hackrf_error)result);
		}
	}

	if (m_params->limit_num_samples) {
		fprintf(stderr, "samples_to_xfer %s/%sMio\n",
			HRFCmdParser::u64toa(m_params->samples_to_xfer, &ascii_u64_data1),
			HRFCmdParser::u64toa((m_params->samples_to_xfer / FREQ_ONE_MHZ), &ascii_u64_data2));
	}
}