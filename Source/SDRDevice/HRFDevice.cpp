#include "HRFDevice.h"
#include <iostream>
#include "SDRUtility.h"

HRFDevice::HRFDevice()
{
	m_hrfDevice = nullptr;
	m_serialNumber = nullptr;
}

HRFDevice::~HRFDevice()
{
	OnExit();
}

// Send data from fileToSend using modulation mod 
void HRFDevice::SendData(HRFUtil::MODULATIONS mod)
{
}

// Initialize hackrf device via libusb here
void HRFDevice::Init()
{
	int result = 0;
	result = hackrf_init();

	if ( result != HACKRF_SUCCESS )
	{
		throw SdrException((hackrf_error)result);
	}
	
	result = hackrf_open_by_serial(m_serialNumber, &m_hrfDevice);
	if ( result != HACKRF_SUCCESS )
	{
		throw SdrException((hackrf_error)result);
	}
}

void HRFDevice::OpenFile(const std::wstring& pathFileToSend)
{
	try
	{
		m_fileToSend = std::ifstream();
		m_fileToSend.open(pathFileToSend, std::ios_base::binary);
	}
	catch (std::exception e)
	{
		std::cerr << e.what() << std::endl;
	}
}

void HRFDevice::OnExit()
{
	// hackrf_close(), hackrf_exit() ...
}