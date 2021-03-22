#pragma once
#include "../hackrf/hackrf.h"
#include "../SDRDevice/SdrDevice.h"
#include "../SDRDevice/HRFDevice.h"
#include <vector>

class HRFDeviceList
{
public:
	HRFDeviceList();

	void UpdateList();

	HRFDevice* GetDeviceById() const;

	~HRFDeviceList() = default;

private:
	std::vector<HRFDevice*> m_hrfDeviceList;
};

