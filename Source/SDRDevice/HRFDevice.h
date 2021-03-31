#pragma once
#include <fstream>
#include "SdrDevice.h"
#include "../SDRUtility/SDRUtility.h"
#include "../SDRTransceiver/HRFTransceiver.h"
#pragma comment(lib, "hackrf.lib")

class HRFDevice :
	public SDRDevice
{
public:
	HRFDevice();
	HRFDevice(const char* serialNumber);
	~HRFDevice();

	//send data from fileToSend using modulation mod
	void SendData(HRFUtil::MODULATIONS mod);

	//initialize hackrf device via libusb here
	void Init();

	void SetCmdLineParams(const HRFUtil::HRFParams& params);

	const char* GetSerialNumber() const;

	void OnExit();
private:
	HRFTransceiver* m_transceiver;

	std::string m_filename;
	hackrf_device* m_device;

	std::shared_ptr<HRFUtil::HRFParams> m_params;
	char* m_serialNumber;

	static volatile uint32_t m_byte_count;
};