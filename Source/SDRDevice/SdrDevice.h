#pragma once
#include <string>
#include "../SDRUtility/SDRUtility.h"

class SDRDevice
{
public:
	virtual ~SDRDevice();

	// Send data from fileToSend with modulation mod
	virtual void SendData(HRFUtil::MODULATIONS mod) = 0;

	// Init USBDevice via libusb
	virtual void Init() = 0;

	virtual void SetCmdLineParams(const HRFUtil::HRFParams& params) = 0;
};