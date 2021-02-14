#pragma once
#include "../SDRUtility/SDRUtility.h"
class SDRTransceiver
{
public:
	virtual void Transfer(hackrf_device* device) = 0;
	virtual void Receive() = 0;

	~SDRTransceiver() {}
};

