#pragma once
#include "SDRTransceiver.h"
#include <fstream>

class HRFTransceiver :
    public SDRTransceiver
{   
public:
    HRFTransceiver();
    HRFTransceiver(const std::shared_ptr<HRFUtil::HRFParams> params, const std::wstring filename);
    
    void Transfer(hackrf_device* device);
    void Receive();


private:
    static int tx_callback(hackrf_transfer* transfer);
    inline static  std::shared_ptr<HRFUtil::HRFParams> m_params;
    inline static std::ifstream m_fileToSend;

    static void make_psk2_buffer(uint8_t* buffer, char* data, size_t size);
};

