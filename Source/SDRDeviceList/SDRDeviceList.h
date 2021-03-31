#pragma once
#include "../SDRDevice/SdrDevice.h"
#include <vector>
class SDRDeviceList
{
public:
	virtual std::vector<const char*> GetSerialNumbers() const = 0;
	virtual void UpdateList() = 0;
};