#pragma once
#include "SDRTransceiver.h"
#include "../hackrf/hackrf.h"
#include <fstream>

class HRFTransceiver :
    public SDRTransceiver
{   
public:
    HRFTransceiver();
    HRFTransceiver(const std::shared_ptr<HRFUtil::HRFParams> params, const std::string filename);
	~HRFTransceiver();
	void Transfer(hackrf_device* device);
    void Receive();

private:
	void EnableParamsForRXTX(hackrf_device* device);
	int tx_callback(hackrf_transfer* transfer);

	std::shared_ptr<HRFUtil::HRFParams> m_params;
    std::ifstream m_fileToSend;

	static void make_psk4_buffer(uint8_t* buffer, char* data, size_t size);
	static void test(uint8_t* buffer, size_t buffer_size);
};