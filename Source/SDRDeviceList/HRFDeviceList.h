#pragma once
#include "../hackrf/hackrf.h"
#include "wx/arrstr.h"
#include "wx/string.h"
#include "../SDRDevice/SdrDevice.h"
#include "../SDRDevice/HRFDevice.h"
#include <vector>

class HRFDeviceList
{
public:
	HRFDeviceList();

	bool UpdateList();

	HRFDevice* GetDeviceById() const;
	wxArrayString GetDeviceList() const;

	~HRFDeviceList();

private:
	std::vector<HRFDevice*> m_hrfDeviceList;
	hackrf_device_list_t* m_list;
};