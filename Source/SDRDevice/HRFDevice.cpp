#include "HRFDevice.h"
#include <iostream>
#include "../SDRException/SDRException.h"

HRFDevice::HRFDevice()
{
	m_device = nullptr;
	m_serialNumber = nullptr;
}

HRFDevice::HRFDevice(const char* serialNumber)
{
	m_device = nullptr;
	m_serialNumber = serialNumber;
}

HRFDevice::~HRFDevice()
{
	OnExit();
}

// Send data from fileToSend using modulation mod 
/*TODO : SendData inits HRFTransceiver and calls HRFTransceiver::Transfer*/
void HRFDevice::SendData(HRFUtil::MODULATIONS mod)
{
	m_transceiver.Transfer(m_device);
}

const char* HRFDevice::GetSerialNumber() const
{
	return m_serialNumber;
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

	std::cerr << "call hackrf_set_sample_rate("
		<< m_params->sample_rate_hz << "Hz / "
		<< static_cast<float>(m_params->sample_rate_hz / FREQ_ONE_MHZ)
		<< " MHz)" << std::endl;

	result = hackrf_set_sample_rate(m_device, m_params->sample_rate_hz);
	
	
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

	std::cerr << "call hackrf_set_hw_sync_mode(" << m_params->hw_sync_enable << ")" << std::endl;
	result = hackrf_set_hw_sync_mode(m_device, m_params->hw_sync_enable ? 1 : 0);
	if (result != HACKRF_SUCCESS)
	{
		std::cerr << "hackrf_set_hw_sync_mode() failed: ";
		throw SDRException((hackrf_error)result);
	}

	m_transceiver = HRFTransceiver(m_params, m_params->filepath);
}

void HRFDevice::SetCmdLineParams(const HRFUtil::HRFParams& params)
{
	m_params = std::make_shared<HRFUtil::HRFParams>(params);
	m_filename = m_params->filepath;
	std::cout << "path to file: " << m_params->filepath << std::endl;
}

void HRFDevice::OnExit()
{
	// hackrf_close(), hackrf_exit() ...
	int result = 0;
	if (m_device != nullptr) {
		if (m_params->receive || m_params->receive_wav) {
			result = hackrf_stop_rx(m_device);
			if (result != HACKRF_SUCCESS) {
				fprintf(stderr, "hackrf_stop_rx() failed: %s (%d)\n", hackrf_error_name((hackrf_error)result), result);
			}
			else {
				std::cerr << "hackrf_stop_rx() done" << std::endl;
			}
		}

		if (m_params->transmit || m_params->signalsource) {
			result = hackrf_stop_tx(m_device);
			if (result != HACKRF_SUCCESS) {
				fprintf(stderr, "hackrf_stop_tx() failed: %s (%d)\n", hackrf_error_name((hackrf_error)result), result);
			}
			else {
				fprintf(stderr, "hackrf_stop_tx() done\n");
			}
		}

		result = hackrf_close(m_device);
		if (result != HACKRF_SUCCESS) {
			fprintf(stderr, "hackrf_close() failed: %s (%d)\n", hackrf_error_name((hackrf_error)result), result);
		}
		else {
			fprintf(stderr, "hackrf_close() done\n");
		}

		hackrf_exit();
		fprintf(stderr, "hackrf_exit() done\n");
		
		std::cerr << "exit" << std::endl;
	}
}