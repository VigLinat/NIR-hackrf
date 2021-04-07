#pragma once
#include <Windows.h>
#include <time.h>
#include <iostream>
#include "../hackrf/hackrf.h"

#pragma comment (lib, "libusb-1.0.lib")
#pragma comment (lib, "pthreadVC2.lib")

namespace HRFUtil {

	const uint32_t HRF_SERIAL_NUMBER_LENGTH = 64;

	enum class MODULATIONS : uint16_t {
		VSDR_MODULATION_PSK_2,
		VSDR_MODULATION_PSK_4,
		VSDR_MODULATION_QAM_8,
		VSDR_MODULATION_QAM_16
	};

	enum rf_path_filter {
		RF_PATH_FILTER_BYPASS = 0,
		RF_PATH_FILTER_LOW_PASS = 1,
		RF_PATH_FILTER_HIGH_PASS = 2,
	};

	#define FD_BUFFER_SIZE (8*1024)

	#define FREQ_ONE_MHZ (1000000ll)

	#define DEFAULT_FREQ_HZ (900000000ll) /* 900MHz */
	#define FREQ_MIN_HZ	(0ull) /* 0 Hz */
	#define FREQ_MAX_HZ	(7250000000ll) /* 7250MHz */
	#define IF_MIN_HZ (2150000000ll)
	#define IF_MAX_HZ (2750000000ll)
	#define LO_MIN_HZ (84375000ll)
	#define LO_MAX_HZ (5400000000ll)
	#define DEFAULT_LO_HZ (1000000000ll)

	#define DEFAULT_SAMPLE_RATE_HZ (10000000) /* 10MHz default sample rate */

	#define DEFAULT_BASEBAND_FILTER_BANDWIDTH (5000000) /* 5MHz default */

	#define SAMPLES_TO_XFER_MAX (0x8000000000000000ull) /* Max value */

	#define BASEBAND_FILTER_BW_MIN (1750000)  /* 1.75 MHz min value */
	#define BASEBAND_FILTER_BW_MAX (28000000) /* 28 MHz max value */

#if defined _WIN32
	#define sleep(a) Sleep( (a*1000) )
#endif

	#define U64TOA_MAX_DIGIT (31)
	typedef struct
	{
		char data[U64TOA_MAX_DIGIT + 1];
	} t_u64toa;

	struct HRFParams
	{
		bool signalsource = false;
		uint32_t amplitude = 0;

		bool hw_sync = false;
		uint32_t hw_sync_enable = 0;

		bool receive = false;
		bool receive_wav = false;
		uint64_t stream_size = 0;
		uint32_t stream_head = 0;
		uint32_t stream_tail = 0;
		uint32_t stream_drop = 0;
		uint8_t* stream_buf = NULL;

		bool transmit = false;

		bool automatic_tuning = false;
		int64_t freq_hz;

		bool if_freq = false;
		int64_t if_freq_hz;

		bool lo_freq = false;
		int64_t lo_freq_hz = DEFAULT_LO_HZ;

		bool image_reject = false;
		uint32_t image_reject_selection;

		bool amp = false;
		uint32_t amp_enable;

		bool antenna = false;
		uint32_t antenna_enable;

		bool sample_rate = false;
		uint32_t sample_rate_hz;

		bool limit_num_samples = false;
		uint64_t samples_to_xfer = 0;
		size_t bytes_to_xfer = 0;

		bool baseband_filter_bw = false;
		uint32_t baseband_filter_bw_hz = 0;

		bool repeat = false;

		bool crystal_correct = false;
		uint32_t crystal_correct_ppm;

		const char* serial_number;

		uint32_t lna_gain = 8, vga_gain = 20, txvga_gain = 0;
		uint32_t requested_mode_count = 0;

		const char* filepath;
		std::string filename;
	};
}