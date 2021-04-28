#include "../SDRUtility/SDRUtility.h"
#include "HRFTransceiver.h"
#include "../HRFCmdParser/HRFCmdParser.h"
#include "../SDRException/SDRException.h"

#include <thread>

volatile uint32_t byte_count = 0;
struct timeval time_start;
struct timeval t_start;

HRFUtil::HRFParams params;
std::ifstream file;
char* read_buffer = nullptr;

bool do_exit = false;

HRFTransceiver::HRFTransceiver(hackrf_device* device)
{
	m_device = device;
}

HRFTransceiver::~HRFTransceiver()
{
	Stop();
}

void HRFTransceiver::Stop()
{
	int result = 0;
	if (m_device != nullptr) 
	{
		do_exit = true;
		if (hackrf_is_streaming(m_device))
		{
			if (m_params.receive || m_params.receive_wav)
			{
				result = hackrf_stop_rx(m_device);
				if (result != HACKRF_SUCCESS)
				{
					throw SDRException((hackrf_error)result);
				}
			}
			if (m_params.transmit || m_params.signalsource)
			{
				result = hackrf_stop_tx(m_device);
				if (result != HACKRF_SUCCESS)
				{
					throw SDRException((hackrf_error)result);
				}
			}
		}
	}
}

int tx_callback_qam4(hackrf_transfer*);
int tx_callback_qam16(hackrf_transfer*);

void HRFTransceiver::Transfer()
{
	do_exit = false;
	int result = 0;
	int exit_code = 0;

	result = hackrf_set_txvga_gain(m_device, m_params.txvga_gain);

	params = m_params;
	file.open(params.filename, std::ios_base::binary | std::ios_base::in);
	result |= hackrf_start_tx(m_device, tx_callback_qam16, NULL);
	EnableParamsForRXTX();

	std::thread consoleLoggerThread(&HRFTransceiver::ConsoleLogger, this);
	std::thread transferMonitorThread(&HRFTransceiver::TransferMonitor, this);

	consoleLoggerThread.detach();
	transferMonitorThread.detach();
}

int tx_callback_qam16(hackrf_transfer* transfer)
{
	if (file.is_open() && file.good() && !file.eof())
	{
		size_t buffer_length = transfer->valid_length;
		size_t bits_to_read = buffer_length / 4;
		byte_count += bits_to_read; // since each inforamtion bit is transmitted as 1 byte,
									// number of sent bytes is equal to number of information bits
		read_buffer = (char*)malloc(bits_to_read);
		file.read(read_buffer, bits_to_read);

		size_t bits_read = file.gcount();
		HRFTransceiver::make_qam16_buffer(transfer->buffer, read_buffer, bits_read);
		if (bits_read != bits_to_read)
		{
			if (params.repeat)
			{
				size_t extra_bits = bits_to_read - bits_read;
				file.seekg(0);
				file.read(read_buffer + bits_read, extra_bits);
				HRFTransceiver::make_qam16_buffer(transfer->buffer + bits_read * 4, read_buffer + bits_read, extra_bits);
				return 0;
			}
			else
			{
				std::cout << "End of file reached" << std::endl;
				return -1;
			}
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return -1;
	}
}

int tx_callback_qam4(hackrf_transfer* transfer)
{
	if (file.is_open() && file.good() && !file.eof())
	{
		size_t buffer_length = transfer->valid_length;
		size_t bits_to_read = buffer_length / 8;
		byte_count += bits_to_read; // since each inforamtion bit is transmitted as 1 byte,
									// number of sent bytes is equal to number of information bits

		read_buffer = (char*)malloc(bits_to_read);
		file.read(read_buffer, bits_to_read);

		size_t bits_read = file.gcount();
		HRFTransceiver::make_qam4_buffer(transfer->buffer, read_buffer, bits_read);
		if (bits_read != bits_to_read)
		{
			if (params.repeat)
			{
				size_t extra_bits = bits_to_read - bits_read;
				file.seekg(0);
				file.read(read_buffer + bits_read, extra_bits);
				HRFTransceiver::make_qam4_buffer(transfer->buffer + bits_read * 8, read_buffer + bits_read, extra_bits);
				return 0;
			}
			else
			{
				std::cout << "End of file reached" << std::endl;
				return -1;
			}
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return -1;
	}
}

void HRFTransceiver::SetParams(const HRFUtil::HRFParams params)
{
	m_params = params;
	std::cerr << "frequency: " << m_params.freq_hz << " (" << m_params.freq_hz / (double) 1000000 <<" MHz"<< std::endl
		<< "tx gain: " << m_params.txvga_gain << std::endl
		<< "sample rate: " << m_params.sample_rate_hz << std::endl;
	HRFCmdParser::CheckCorrectParams(m_params);
	InitTransceiver();
}

int HRFTransceiver::GetStreamingStatus() const
{
	return hackrf_is_streaming(m_device);
}

void HRFTransceiver::make_qam4_buffer(uint8_t* buffer, char* data, size_t size)
{
	// QAM-4: 1 byte of information is mapped into 8 bytes of IQ 
	// information byte is splitted into 4 parts 
	// each part requires 2 bytes of IQ
	struct IQ_byte
	{
		uint8_t I;
		uint8_t Q;
	};
	IQ_byte iq1, iq2, iq3, iq4;

	// this array stores Gray codes for semibytes from 
	// 0 to 15
	// each entry in array maps for it's index in Gray code
	uint8_t gray_mapping[4] = {
		0x00,	0x01,	
		0x02,	0x03
	};

	// this array represents mapping from Gray symbol
	// into I, Q bytes 
	// indexes addressed by Gray coding corresponds to it
	// I, Q byte values
	IQ_byte qam4_mapping[4] = {
		{64, 64},	{127, 64}, 
	    {64, 127},	{127, 127}
	};

	for (int i = 0; i < size; i++)
	{
		uint8_t byte = data[i];
		//		 bits 7-6   bits 5-4   bits 3-2   bits 1-0
		uint8_t semibyte1, semibyte2, semibyte3, semibyte4;

		semibyte1 = (
			byte & (1 << 7) +
			byte & (1 << 6)
		) >> 6;

		semibyte2 = (
			byte & (1 << 5) +
			byte & (1 << 4)
		) >> 4;

		semibyte3 = (
			byte & (1 << 3) +
			byte & (1 << 2)
		) >> 2;

		semibyte4 = (
			byte & (1 << 1) +
			byte & (1 << 0)
		);

		semibyte1 = gray_mapping[semibyte1];
		semibyte2 = gray_mapping[semibyte2];
		semibyte3 = gray_mapping[semibyte3];
		semibyte4 = gray_mapping[semibyte4];
		iq1 = qam4_mapping[semibyte1];
		iq2 = qam4_mapping[semibyte2];
		iq3 = qam4_mapping[semibyte3];
		iq4 = qam4_mapping[semibyte4];
		buffer[8 * i + 0] = iq1.I;
		buffer[8 * i + 1] = iq1.Q;
		buffer[8 * i + 2] = iq2.I;
		buffer[8 * i + 3] = iq2.Q;
		buffer[8 * i + 4] = iq3.I;
		buffer[8 * i + 5] = iq3.Q;
		buffer[8 * i + 6] = iq4.I;
		buffer[8 * i + 7] = iq4.Q;
	}
}

void HRFTransceiver::make_qam16_buffer(uint8_t* buffer, char* data, size_t size)
{
	struct IQ_byte
	{
		uint8_t I;
		uint8_t Q;
	};
	// QAM-16: 1 byte of information is mapped into 4 bytes of IQ 
	// first semi-byte is coded by I1, Q1
	// second semi-byte is coded by I2, Q2
	IQ_byte iq1;
	IQ_byte iq2;

	// this array stores Gray codes for semibytes from 
	// 0 to 15
	// each entry in array maps for it's index in Gray code
	uint8_t gray_mapping[16] = {
		0x00,	0x01,	0x03,	0x02,
		0x0C,	0x0D,	0x0F,	0x0E,
		0x06,	0x07,	0x05,	0x04,
		0x0A,	0x0B,	0x09,	0x08
	};

	// this array represents mapping from Gray symbol
	// into I, Q bytes 
	// indexes addressed by Gray coding corresponds to it
	// I, Q byte values
	IQ_byte qam16_mapping[16] = {
		{32, 32},  {64, 32},  {96, 32},  {127, 32},
		{32, 64},  {64, 64},  {96, 64},  {127, 64},
		{32, 96},  {64, 96},  {96, 96},  {127, 96},
		{32, 127}, {64, 127}, {96, 127}, {127, 127}
	};
	
	for (int i = 0; i < size; i++)
	{
		uint8_t byte = data[i];
		//       bits 7-4   bits 3-0
		uint8_t semibyte1, semibyte2;
		semibyte1 = (
				byte & (1 << 7) +
				byte & (1 << 6) +
				byte & (1 << 5) +
				byte & (1 << 4)
		) >> 4;
			
		semibyte2 =
			byte & (1 << 3) +
			byte & (1 << 2) +
			byte & (1 << 1) +
			byte & (1 << 0);
		// converting semibytes into Gray code
		semibyte1 = gray_mapping[semibyte1];
		semibyte2 = gray_mapping[semibyte2];
		iq1 = qam16_mapping[semibyte1];
		iq2 = qam16_mapping[semibyte2];
		buffer[4 * i + 0] = iq1.I;
		buffer[4 * i + 1] = iq1.Q;
		buffer[4 * i + 2] = iq2.I;
		buffer[4 * i + 3] = iq2.Q;
	}
}

void HRFTransceiver::InitTransceiver()
{
	int result = 0;

	result = hackrf_set_sample_rate(m_device, m_params.sample_rate_hz);
	if (result != HACKRF_SUCCESS)
	{
		throw SDRException((hackrf_error)result);
	}

	if (m_params.baseband_filter_bw)
	{
		result = hackrf_set_baseband_filter_bandwidth(m_device, m_params.baseband_filter_bw_hz);
		if (result != HACKRF_SUCCESS)
		{
			throw SDRException((hackrf_error)result);
		}
	}

	result = hackrf_set_hw_sync_mode(m_device, m_params.hw_sync_enable ? 1 : 0);
	if (result != HACKRF_SUCCESS)
	{
		throw SDRException((hackrf_error)result);
	}
}

void HRFTransceiver::EnableParamsForRXTX()
{
	int result = 0;
	if (m_params.automatic_tuning)
	{
		result |= hackrf_set_freq(m_device, m_params.freq_hz);
		if (result != HACKRF_SUCCESS)
		{
			throw SDRException((hackrf_error)result);
		}
	}
	else
	{
		result |= hackrf_set_freq_explicit(
			m_device,
			m_params.if_freq_hz, m_params.lo_freq_hz,
			(rf_path_filter)m_params.image_reject_selection
		);
		if (result != HACKRF_SUCCESS)
		{
			throw SDRException((hackrf_error)result);
		}
	}

	if (m_params.amp)
	{
		result |= hackrf_set_amp_enable(m_device, (uint8_t)m_params.amp_enable);
		if (result != HACKRF_SUCCESS)
		{
			throw SDRException((hackrf_error)result);
		}
	}

	if (m_params.antenna)
	{
		result |= hackrf_set_antenna_enable(m_device, (uint8_t)m_params.antenna_enable);
		if (result != HACKRF_SUCCESS)
		{
			throw SDRException((hackrf_error)result);
		}
	}
}

void HRFTransceiver::TransferMonitor()
{
	while ((hackrf_is_streaming(m_device) == HACKRF_TRUE) &&
		(do_exit == false))
	{

	}

	Stop();
}

void HRFTransceiver::ConsoleLogger()
{
	int result = 0;
	int exit_code = 0;

	struct timeval t_end;
	float time_diff;

	gettimeofday(&t_start, NULL);
	gettimeofday(&time_start, NULL);

	fprintf(stderr, "Stop with Ctrl-C\n");
	while ((hackrf_is_streaming(m_device) == HACKRF_TRUE) &&
		(do_exit == false))
	{
		uint32_t byte_count_now;
		struct timeval time_now;
		float rate = 0;
		float time_difference = 0;
		if (m_params.stream_size > 0)
		{

		}
		else
		{
			sleep(1);
			gettimeofday(&time_now, NULL);
			byte_count_now = byte_count;
			byte_count = 0;

			time_difference = TimevalDiff(&time_now, &time_start);
			rate = (float)byte_count_now / time_difference;
			if (byte_count_now == 0 && m_params.hw_sync == true && m_params.hw_sync_enable != 0)
			{
				fprintf(stderr, "Waiting for sync...\n");
			}
			else
			{
				fprintf(stderr, "%4.2f MiB / %5.3f sec = %4.2f MiB/second\n",
					(byte_count_now / 1e6f), time_difference, (rate / 1e6f));
			}

			time_start = time_now;

			if (byte_count_now == 0 && (m_params.hw_sync == false || m_params.hw_sync_enable == 0))
			{
				exit_code = EXIT_FAILURE;
				fprintf(stderr, "\nCouldn't transfer any bytes for one second.\n");
				break;
			}
		}
	}

	result = hackrf_is_streaming(m_device);

	if (do_exit)
	{
		fprintf(stderr, "\nExiting...\n");
	}
	else
	{
		fprintf(stderr, "\nExiting... hackrf_is_streaming() result: %s (%d)\n", hackrf_error_name((hackrf_error)result), result);
	}

	if (read_buffer != nullptr)
	{
		free(read_buffer);
	}

	gettimeofday(&t_end, NULL);
	time_diff = TimevalDiff(&t_end, &t_start);
	fprintf(stderr, "Total time: %5.5f s\n", time_diff);
}

int HRFTransceiver::gettimeofday(struct timeval* tv, void* ignored)
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

float HRFTransceiver::TimevalDiff(const struct timeval* a, const struct timeval* b)
{
	return (a->tv_sec - b->tv_sec) + 1e-6f * (a->tv_usec - b->tv_usec);
}
