#include "../SDRUtility/SDRUtility.h"

class HRFCmdParser
{
public:
	static HRFUtil::HRFParams ParseCommandLine(int argc, char** argv);
private:
	static void CheckCorrectParams(HRFUtil::HRFParams& params);
	static uint32_t parse_u32(char* s);
	static uint64_t parse_u64(char* s);
	static int64_t parse_frequency_i64(char* optarg, char* endptr);
	static uint32_t parse_frequency_u32(char* optarg, char* endptr);
	static char* stringrev(char* str);

public:
	static char* u64toa(uint64_t val, HRFUtil::t_u64toa* str);
	static void usage();
};