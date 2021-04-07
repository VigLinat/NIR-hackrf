#include "SDRException.h"
#include "../SDRUtility/SDRUtility.h"

SDRException::SDRException()
{
	m_message = "";
}

SDRException::SDRException(const hackrf_error result)
{
	m_message = std::string(hackrf_error_name(result));
}

SDRException::SDRException(const std::string& message)
{
	m_message = message;
}

std::string SDRException::What() const
{
	return m_message;
}