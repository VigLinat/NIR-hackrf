#pragma once
#include <string>
#include "../SDRUtility/SDRUtility.h"

class SDRException
{
public:
	SDRException();
	 
	SDRException(const hackrf_error result);
	
	virtual std::string What() const;

private:
	hackrf_error m_result;
};
