#pragma once
#include "../hackrf/hackrf.h"
#include "wx/arrstr.h"
#include "wx/string.h"
#include "SDRDeviceList.h"
#include "../SDRDevice/HRFDevice.h"
#include <vector>

class HRFDeviceList : 
	SDRDeviceList
{
public:
	HRFDeviceList();

	void UpdateList() override;
	wxArrayString&& GetSerialNumbers() const override;

	~HRFDeviceList() = default;

private:
	std::vector<HRFDevice*> m_hrfDeviceList;
};