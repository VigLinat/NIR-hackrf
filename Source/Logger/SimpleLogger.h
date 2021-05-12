#pragma once
#include <fstream>
#include <ctime>
class SimpleLogger
{
public:
	SimpleLogger(const char* logfile);
	void WriteLine(const char* line);
	void WriteCharArray(uint8_t* data, uint32_t size);
	~SimpleLogger();
private:
	std::ofstream m_logfile;
};

