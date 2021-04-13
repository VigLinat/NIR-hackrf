#include "ConsoleLogger.h"

void ConsoleLogger::Log(const char* what)
{
	std::cerr << what << std::endl;
}