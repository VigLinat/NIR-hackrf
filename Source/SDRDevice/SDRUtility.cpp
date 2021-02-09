#include "SDRUtility.h"
#include <string>

#include "../getopt/my_getopt.h"

extern char* optarg;

SdrException::SdrException(hackrf_error result)
{
	m_result = result;
}

std::string SdrException::What() const
{
	return "hackrf failed: " + std::string(hackrf_error_name(m_result));
}

HRFUtil::HRFParams* HRFUtil::HRFCmdParser::ParseCommandLine(int argc, char** argv)
{
	int opt;
	HRFParams* params = new HRFParams;
	char* endptr = NULL;
	while ((opt = my_getopt(argc, argv)) != EOF)
	{
		int result = HACKRF_SUCCESS;
		switch (opt)
		{
		case 'H':
			params->hw_sync = true;
			params->hw_sync_enable = parse_u32(optarg);
			break;
		case 'w':
			params->receive_wav = true;
			params->requested_mode_count++;
			break;

		case 'r':
			params->receive = true;
			params->requested_mode_count++;
			break;

		case 't':
			params->transmit = true;
			params->requested_mode_count++;
			break;

		case 'd':
			params->serial_number = optarg;
			break;

		case 'S':
			params->stream_size = parse_u64(optarg);
			params->stream_buf = (uint8_t*) calloc(1, params->stream_size);
			break;

		case 'f':
			params->freq_hz = parse_frequency_i64(optarg, endptr);
			params->automatic_tuning = true;
			break;

		case 'i':
			params->if_freq_hz = parse_frequency_i64(optarg, endptr);
			params->if_freq = true;
			break;

		case 'o':
			params->lo_freq_hz = parse_frequency_i64(optarg, endptr);
			params->lo_freq = true;
			break;

		case 'm':
			params->image_reject = true;
			params->image_reject_selection = parse_u32(optarg);
			break;

		case 'a':
			params->amp = true;
			params->amp_enable = parse_u32(optarg);
			break;

		case 'p':
			params->antenna = true;
			params->antenna_enable = parse_u32(optarg);
			break;

		case 'l':
			params->lna_gain = parse_u32(optarg);
			break;

		case 'g':
			params->vga_gain = parse_u32(optarg);
			break;

		case 'x':
			params->txvga_gain = parse_u32(optarg);
			break;

		case 's':
			params->sample_rate_hz = parse_frequency_u32(optarg, endptr);
			params->sample_rate = true;
			break;

		case 'n':
			params->limit_num_samples = true;
			params->samples_to_xfer = parse_u64(optarg);
			params->bytes_to_xfer = params->samples_to_xfer * 2ull;
			break;

		case 'b':
			params->baseband_filter_bw_hz = parse_frequency_u32(optarg, endptr);
			params->baseband_filter_bw = true;
			break;

		case 'c':
			params->signalsource = true;
			params->requested_mode_count++;
			params->amplitude = parse_u32(optarg);
			break;

		case 'R':
			params->repeat = true;
			break;

		case 'C':
			params->crystal_correct = true;
			params->crystal_correct_ppm = parse_u32(optarg);
			break;

		case 'h':
		case '?':
			usage();
			throw SdrException();// "EXIT_SUCCESS

		default:
			fprintf(stderr, "unknown argument '-%c %s'\n", opt, optarg);
			usage();
			throw SdrException(); // EXIT_FAILURE;
		}

		if (result != HACKRF_SUCCESS) {
			fprintf(stderr, "argument error: '-%c %s' %s (%d)\n", opt, optarg, hackrf_error_name((hackrf_error)result), result);
			usage();
			throw SdrException();// return EXIT_FAILURE;
		}
	}

	CheckCorrectParams(params);

	return params;
}

void HRFUtil::HRFCmdParser::CheckCorrectParams(HRFParams* params)
{
	HRFUtil::t_u64toa ascii_u64_data1;
	HRFUtil::t_u64toa ascii_u64_data2;

	if (params->lna_gain % 8)
		fprintf(stderr, "warning: lna_gain (-l) must be a multiple of 8\n");

	if (params->vga_gain % 2)
		fprintf(stderr, "warning: vga_gain (-g) must be a multiple of 2\n");

	if (params->samples_to_xfer >= SAMPLES_TO_XFER_MAX) {
		fprintf(stderr, "argument error: num_samples must be less than %s/%sMio\n",
			u64toa(SAMPLES_TO_XFER_MAX, &ascii_u64_data1),
			u64toa((SAMPLES_TO_XFER_MAX / FREQ_ONE_MHZ), &ascii_u64_data2));
		usage();
		throw SdrException();// return EXIT_FAILURE;
	}

	if (params->if_freq || params->lo_freq || params->image_reject) {
		/* explicit tuning selected */
		if (!params->if_freq) {
			fprintf(stderr, "argument error: if_freq_hz must be specified for explicit tuning.\n");
			usage();
			throw SdrException();// return EXIT_FAILURE;
		}
		if (!params->image_reject) {
			fprintf(stderr, "argument error: image_reject must be specified for explicit tuning.\n");
			usage();
			throw SdrException();// return EXIT_FAILURE;
		}
		if (!params->lo_freq && (params->image_reject_selection != RF_PATH_FILTER_BYPASS)) {
			fprintf(stderr, "argument error: lo_freq_hz must be specified for explicit tuning unless image_reject is set to bypass.\n");
			usage();
			throw SdrException();// return EXIT_FAILURE;
		}
		if ((params->if_freq_hz > IF_MAX_HZ) || (params->if_freq_hz < IF_MIN_HZ)) {
			fprintf(stderr, "argument error: if_freq_hz shall be between %s and %s.\n",
				u64toa(IF_MIN_HZ, &ascii_u64_data1),
				u64toa(IF_MAX_HZ, &ascii_u64_data2));
			usage();
			throw SdrException();// return EXIT_FAILURE;
		}
		if ((params->lo_freq_hz > LO_MAX_HZ) || (params->lo_freq_hz < LO_MIN_HZ)) {
			fprintf(stderr, "argument error: lo_freq_hz shall be between %s and %s.\n",
				u64toa(LO_MIN_HZ, &ascii_u64_data1),
				u64toa(LO_MAX_HZ, &ascii_u64_data2));
			usage();
			throw SdrException();// return EXIT_FAILURE;
		}
		if (params->image_reject_selection > 2) {
			fprintf(stderr, "argument error: image_reject must be 0, 1, or 2 .\n");
			usage();
			throw SdrException();// return EXIT_FAILURE;
		}
		if (params->automatic_tuning) {
			fprintf(stderr, "warning: freq_hz ignored by explicit tuning selection.\n");
			params->automatic_tuning = false;
		}
		switch (params->image_reject_selection) {
		case RF_PATH_FILTER_BYPASS:
			params->freq_hz = params->if_freq_hz;
			break;
		case RF_PATH_FILTER_LOW_PASS:
			params->freq_hz = (int64_t)labs((long int)(params->if_freq_hz - params->lo_freq_hz));
			break;
		case RF_PATH_FILTER_HIGH_PASS:
			params->freq_hz = params->if_freq_hz + params->lo_freq_hz;
			break;
		default:
			params->freq_hz = DEFAULT_FREQ_HZ;
			break;
		}
		fprintf(stderr, "explicit tuning specified for %s Hz.\n",
			u64toa(params->freq_hz, &ascii_u64_data1));

	}
	else if (params->automatic_tuning) {
		if (params->freq_hz > FREQ_MAX_HZ)
		{
			fprintf(stderr, "argument error: freq_hz shall be between %s and %s.\n",
				u64toa(FREQ_MIN_HZ, &ascii_u64_data1),
				u64toa(FREQ_MAX_HZ, &ascii_u64_data2));
			usage();
			throw SdrException();// return EXIT_FAILURE;
		}
	}
	else {
		/* Use default freq */
		params->freq_hz = DEFAULT_FREQ_HZ;
		params->automatic_tuning = true;
	}

	if (params->amp) {
		if (params->amp_enable > 1)
		{
			fprintf(stderr, "argument error: amp_enable shall be 0 or 1.\n");
			usage();
			throw SdrException();// return EXIT_FAILURE;
		}
	}

	if (params->antenna) {
		if (params->antenna_enable > 1) {
			fprintf(stderr, "argument error: antenna_enable shall be 0 or 1.\n");
			usage();
			throw SdrException();// return EXIT_FAILURE;
		}
	}

	if (params->sample_rate == false)
	{
		params->sample_rate_hz = DEFAULT_SAMPLE_RATE_HZ;
	}

	if (params->baseband_filter_bw)
	{
		if (params->baseband_filter_bw_hz > BASEBAND_FILTER_BW_MAX) {
			fprintf(stderr, "argument error: baseband_filter_bw_hz must be less or equal to %u Hz/%.03f MHz\n",
				BASEBAND_FILTER_BW_MAX, (float)(BASEBAND_FILTER_BW_MAX / FREQ_ONE_MHZ));
			usage();
			throw SdrException();// return EXIT_FAILURE;
		}

		if (params->baseband_filter_bw_hz < BASEBAND_FILTER_BW_MIN) {
			fprintf(stderr, "argument error: baseband_filter_bw_hz must be greater or equal to %u Hz/%.03f MHz\n",
				BASEBAND_FILTER_BW_MIN, (float)(BASEBAND_FILTER_BW_MIN / FREQ_ONE_MHZ));
			usage();
			throw SdrException();// return EXIT_FAILURE;
		}

		/* Compute nearest freq for bw filter */
		params->baseband_filter_bw_hz = hackrf_compute_baseband_filter_bw(params->baseband_filter_bw_hz);
	}

	if (params->requested_mode_count > 1) {
		fprintf(stderr, "specify only one of: -t, -c, -r, -w\n");
		usage();
		throw SdrException();// return EXIT_FAILURE;
	}

	if (params->requested_mode_count < 1) {
		fprintf(stderr, "specify one of: -t, -c, -r, -w\n");
		usage();
		throw SdrException();// return EXIT_FAILURE;
	}

	if (params->crystal_correct) {
		params->sample_rate_hz = (uint32_t)((double)
			params->sample_rate_hz * (1000000 - params->crystal_correct_ppm) / 1000000 + 0.5);
		params->freq_hz = params->freq_hz * (1000000 - params->crystal_correct_ppm) / 1000000;
	}
}

uint32_t HRFUtil::HRFCmdParser::parse_u32(char* s)
{
	uint_fast8_t base = 10;
	char* s_end;
	uint64_t ulong_value;

	if (strlen(s) > 2) {
		if (s[0] == '0') {
			if ((s[1] == 'x') || (s[1] == 'X')) {
				base = 16;
				s += 2;
			}
			else if ((s[1] == 'b') || (s[1] == 'B')) {
				base = 2;
				s += 2;
			}
		}
	}

	s_end = s;
	ulong_value = strtoul(s, &s_end, base);
	if ((s != s_end) && (*s_end == 0)) {
		return (uint32_t)ulong_value;
	}
	else {
		throw SdrException(); // return HACKRF_ERROR_INVALID_PARAM;
	}
}

uint64_t HRFUtil::HRFCmdParser::parse_u64(char* s)
{
	uint_fast8_t base = 10;
	char* s_end;
	uint64_t u64_value;

	if (strlen(s) > 2) {
		if (s[0] == '0') {
			if ((s[1] == 'x') || (s[1] == 'X')) {
				base = 16;
				s += 2;
			}
			else if ((s[1] == 'b') || (s[1] == 'B')) {
				base = 2;
				s += 2;
			}
		}
	}

	s_end = s;
	u64_value = strtoull(s, &s_end, base);
	if ((s != s_end) && (*s_end == 0)) {
		return u64_value;
		return HACKRF_SUCCESS;
	}
	else {
		throw SdrException();// return HACKRF_ERROR_INVALID_PARAM;
	}
}

int64_t HRFUtil::HRFCmdParser::parse_frequency_i64(char* optarg, char* endptr)
{
	if (optarg == endptr) {
		throw SdrException();// return HACKRF_ERROR_INVALID_PARAM;
	}
	return (int64_t)strtod(optarg, &endptr);
}

uint32_t HRFUtil::HRFCmdParser::parse_frequency_u32(char* optarg, char* endptr)
{
	if (optarg == endptr) {
		throw SdrException();// return HACKRF_ERROR_INVALID_PARAM;
	}
	return (uint32_t)strtod(optarg, &endptr);
}

void HRFUtil::HRFCmdParser::usage()
{
	HRFUtil::t_u64toa ascii_u64_data1;
	HRFUtil::t_u64toa ascii_u64_data2;
	printf("Usage:\n");
	printf("\t-h # this help\n");
	printf("\t[-d serial_number] # Serial number of desired HackRF.\n");
	printf("\t-r <filename> # Receive data into file (use '-' for stdout).\n");
	printf("\t-t <filename> # Transmit data from file (use '-' for stdin).\n");
	printf("\t-w # Receive data into file with WAV header and automatic name.\n");
	printf("\t   # This is for SDR# compatibility and may not work with other software.\n");
	printf("\t[-f freq_hz] # Frequency in Hz [%sMHz to %sMHz].\n",
		u64toa((FREQ_MIN_HZ / FREQ_ONE_MHZ), &ascii_u64_data1),
		u64toa((FREQ_MAX_HZ / FREQ_ONE_MHZ), &ascii_u64_data2));
	printf("\t[-i if_freq_hz] # Intermediate Frequency (IF) in Hz [%sMHz to %sMHz].\n",
		u64toa((IF_MIN_HZ / FREQ_ONE_MHZ), &ascii_u64_data1),
		u64toa((IF_MAX_HZ / FREQ_ONE_MHZ), &ascii_u64_data2));
	printf("\t[-o lo_freq_hz] # Front-end Local Oscillator (LO) frequency in Hz [%sMHz to %sMHz].\n",
		u64toa((LO_MIN_HZ / FREQ_ONE_MHZ), &ascii_u64_data1),
		u64toa((LO_MAX_HZ / FREQ_ONE_MHZ), &ascii_u64_data2));
	printf("\t[-m image_reject] # Image rejection filter selection, 0=bypass, 1=low pass, 2=high pass.\n");
	printf("\t[-a amp_enable] # RX/TX RF amplifier 1=Enable, 0=Disable.\n");
	printf("\t[-p antenna_enable] # Antenna port power, 1=Enable, 0=Disable.\n");
	printf("\t[-l gain_db] # RX LNA (IF) gain, 0-40dB, 8dB steps\n");
	printf("\t[-g gain_db] # RX VGA (baseband) gain, 0-62dB, 2dB steps\n");
	printf("\t[-x gain_db] # TX VGA (IF) gain, 0-47dB, 1dB steps\n");
	printf("\t[-s sample_rate_hz] # Sample rate in Hz (2-20MHz, default %sMHz).\n",
		u64toa((DEFAULT_SAMPLE_RATE_HZ / FREQ_ONE_MHZ), &ascii_u64_data1));
	printf("\t[-n num_samples] # Number of samples to transfer (default is unlimited).\n");
#ifndef _WIN32
	/* The required atomic load/store functions aren't available when using C with MSVC */
	printf("\t[-S buf_size] # Enable receive streaming with buffer size buf_size.\n");
#endif
	printf("\t[-c amplitude] # CW signal source mode, amplitude 0-127 (DC value to DAC).\n");
	printf("\t[-R] # Repeat TX mode (default is off) \n");
	printf("\t[-b baseband_filter_bw_hz] # Set baseband filter bandwidth in Hz.\n\tPossible values: 1.75/2.5/3.5/5/5.5/6/7/8/9/10/12/14/15/20/24/28MHz, default <= 0.75 * sample_rate_hz.\n");
	printf("\t[-C ppm] # Set Internal crystal clock error in ppm.\n");
	printf("\t[-H hw_sync_enable] # Synchronise USB transfer using GPIO pins.\n");
}

char* HRFUtil::HRFCmdParser::u64toa(uint64_t val, HRFUtil::t_u64toa* str)
{
#define BASE (10ull) /* Base10 by default */
	uint64_t sum;
	int pos;
	int digit;
	int max_len;
	char* res;

	sum = val;
	max_len = U64TOA_MAX_DIGIT;
	pos = 0;

	do
	{
		digit = (sum % BASE);
		str->data[pos] = digit + '0';
		pos++;

		sum /= BASE;
	} while ((sum > 0) && (pos < max_len));

	if ((pos == max_len) && (sum > 0))
		return NULL;

	str->data[pos] = '\0';
	res = stringrev(str->data);

	return res;
}

char* HRFUtil::HRFCmdParser::stringrev(char* str)
{
	char* p1, * p2;

	if (!str || !*str)
		return str;

	for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
	{
		*p1 ^= *p2;
		*p2 ^= *p1;
		*p1 ^= *p2;
	}
	return str;
}