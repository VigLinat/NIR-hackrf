#pragma once
#include <string>
#include "../SDRUtility/SDRUtility.h"

class SDRException
{
public:
	SDRException();
	 
	SDRException(const hackrf_error result);

	SDRException(const std::string& message);

	virtual std::string What() const;

private:
	std::string m_message;
};
