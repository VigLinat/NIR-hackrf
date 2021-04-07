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
	HRFDevice* GetDeviceBySerial(const char* serial);
	~HRFDeviceList() = default;

private:
	HRFDevice* FindDeviceBySerial(const char* serial) const;

	std::vector<HRFDevice*> m_hrfDeviceList;
};