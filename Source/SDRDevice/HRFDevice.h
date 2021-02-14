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
	~HRFDevice();

	//send data from fileToSend using modulation mod
	void SendData(HRFUtil::MODULATIONS mod);

	//initialize hackrf device via libusb here
	void Init();

	void SetCmdLineParams(const HRFUtil::HRFParams& params);

	void SetFilename(const std::wstring& filename);

	void OnExit();
private:
	HRFTransceiver m_transceiver;

	std::wstring m_filename;
	hackrf_device* m_device;

	std::shared_ptr<HRFUtil::HRFParams> m_params;
	const char* m_serialNumber;

	static volatile uint32_t m_byte_count;

	
};