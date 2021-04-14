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

	void Init();
	//send data from fileToSend using modulation mod
	void SendData(HRFUtil::MODULATIONS mod);
	void StopTX();
	const char* GetSerialNumber() const;
	void SetParams(HRFUtil::HRFParams params);
	HRFTransceiver* GetTransceiver() const;

private:
	void OnExit();
	HRFTransceiver* m_transceiver;
	hackrf_device* m_device;

	char* m_serialNumber;

	static volatile uint32_t m_byte_count;
};