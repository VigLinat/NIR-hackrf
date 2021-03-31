#pragma once
#include "../SDRDevice/SdrDevice.h"
#include "wx/wx.h"
class SDRDeviceList
{
	virtual wxArrayString&& GetSerialNumbers() const = 0;
	virtual void UpdateList() = 0;
};