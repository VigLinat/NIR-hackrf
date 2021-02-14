#include "SDRException.h"
#include "../SDRUtility/SDRUtility.h"

SDRException::SDRException()
{
	m_result = HACKRF_SUCCESS;
}

SDRException::SDRException(const hackrf_error result)
{
	m_result = result;
}

std::string SDRException::What() const
{
	return std::string(hackrf_error_name(m_result));
}