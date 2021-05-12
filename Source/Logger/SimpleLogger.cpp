#include "SimpleLogger.h"

SimpleLogger::SimpleLogger(const char* filename)
{
	std::time_t currentTime = std::time(nullptr);
	m_logfile.open(filename);
	m_logfile << std::asctime (std::localtime(&currentTime)) 
		      << ": " 
		      << "Log started;" 
		      << std::endl;
}

void SimpleLogger::WriteLine(const char* line)
{
	std::time_t currentTime = std::time(nullptr);
	m_logfile << std::asctime(std::localtime(&currentTime))
			  << line
		      << std::endl;
}

void SimpleLogger::WriteCharArray(uint8_t* data, uint32_t size)
{
	for (int i = 0; i < size; i++)
	{
		m_logfile << data[i];
	}
}

SimpleLogger::~SimpleLogger()
{
	m_logfile.close();
}