#pragma once
#include "SdrDevice.h"
#include "SDRUtility.h"
#include <fstream>

#pragma comment(lib, "hackrf.lib")

class HRFDevice :
	public SDRDevice
{
public:
	HRFDevice();
	~HRFDevice();

	//send data from fileToSend using modulation mod
	void SendData(HRFUtil::MODULATIONS mod);

	//initialize hackrf device via libusb here
	void Init();

	void OpenFile(const std::wstring& pathFileToSend);

	void OnExit();

private:
	std::ifstream m_fileToSend;
	hackrf_device* m_hrfDevice;
	std::shared_ptr<HRFUtil::HRFParams> m_params;
	const char* m_serialNumber;
};