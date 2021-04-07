#include "HRFDevice.h"
#include <iostream>
#include "../SDRException/SDRException.h"

HRFDevice::HRFDevice()
{
	m_device = nullptr;
	m_serialNumber = nullptr;
	m_transceiver = nullptr;
}

HRFDevice::HRFDevice(const char* serialNumber)
{
	m_device = nullptr;
	m_transceiver = nullptr;

	size_t len = strlen(serialNumber);
	m_serialNumber = new char[len];
	strcpy(m_serialNumber, serialNumber);
}

HRFDevice::~HRFDevice()
{
	delete m_serialNumber;
	delete m_transceiver;
	OnExit();
}

void HRFDevice::Init()
{
	int result = 0;

	result = hackrf_open_by_serial(m_serialNumber, &m_device);
	if (result != HACKRF_SUCCESS)
	{
		throw SDRException((hackrf_error)result);
	}

	m_transceiver = new HRFTransceiver(m_device);
}


void HRFDevice::SetParams(HRFUtil::HRFParams params)
{
	m_transceiver->SetParams(params);
}

void HRFDevice::SendData(HRFUtil::MODULATIONS mod)
{
	m_transceiver->Transfer();
}

const char* HRFDevice::GetSerialNumber() const
{
	return m_serialNumber;
}

HRFTransceiver* HRFDevice::GetTransceiver() const
{
	return m_transceiver;
}

void HRFDevice::OnExit()
{
	m_transceiver->Stop();
	int result = hackrf_close(m_device);
	if (result != HACKRF_SUCCESS)
	{
		throw SDRException((hackrf_error)result);
	}
}