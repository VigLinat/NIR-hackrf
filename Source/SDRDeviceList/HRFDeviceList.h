#pragma once
#include "SDRDeviceList.h"
#include "../SDRDevice/HRFDevice.h"
#include <vector>

class HRFDeviceList : 
	public SDRDeviceList
{
public:
	HRFDeviceList();

	void UpdateList() override;
	std::vector<const char*> GetSerialNumbers() const override;
	void SetParamsBySerial(const char* serial, HRFUtil::HRFParams params);
	~HRFDeviceList() = default;

private:
	std::vector<HRFDevice*> m_hrfDeviceList;
};