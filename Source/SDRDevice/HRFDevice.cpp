#include "HRFDevice.h"
#include <iostream>
#include "../SDRException/SDRException.h"

HRFDevice::HRFDevice()
{
	m_device = nullptr;
	m_serialNumber = nullptr;
}

HRFDevice::~HRFDevice()
{
	OnExit();
}

// Send data from fileToSend using modulation mod 
/*TODO : SendData inits HRFTransceiver and calls HRFTransceiver::Transfer*/
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
		throw SDRException((hackrf_error)result);
	}
	
	result = hackrf_open_by_serial(m_serialNumber, &m_device);
	if ( result != HACKRF_SUCCESS )
	{
		std::cerr << "hackrf_open_by_serial() failed: ";
		throw SDRException((hackrf_error)result);
	}

	result = hackrf_set_sample_rate(m_device, m_params->sample_rate_hz);
	std::cerr << "call hackrf_set_sample_rate(" 
		<< m_params->sample_rate_hz <<"Hz / "
		<< static_cast<float>(m_params->sample_rate_hz / FREQ_ONE_MHZ)
		<<" MHz)" << std::endl;

	if ( result != HACKRF_SUCCESS)
	{
		std::cerr << "hackrf_set_sample_rate() failed: ";
		throw SDRException((hackrf_error)result);
	}

	if (m_params->baseband_filter_bw)
	{
		result = hackrf_set_baseband_filter_bandwidth(m_device, m_params->baseband_filter_bw_hz);
		if (result != HACKRF_SUCCESS)
		{
			std::cerr << "hackrf_set_baseband_filter_bandwith() failed: ";
		}
		throw SDRException((hackrf_error)result);
	}
	
	result = hackrf_set_hw_sync_mode(m_device, m_params->hw_sync_enable ? 1 : 0);
	if (result != HACKRF_SUCCESS)
	{
		std::cerr << "hackrf_set_hw_sync_mode() failed: ";
		throw SDRException((hackrf_error)result);
	}

	HRFTransceiver m_transceiver(m_params, m_filename);
}

void HRFDevice::SetFilename(const std::wstring& filename)
{
	m_filename = filename;
}

void HRFDevice::SetCmdLineParams(const HRFUtil::HRFParams& params)
{
	m_params = std::make_shared<HRFUtil::HRFParams>(params);
}

void HRFDevice::OnExit()
{
	// hackrf_close(), hackrf_exit() ...
}