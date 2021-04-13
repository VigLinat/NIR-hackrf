#pragma once
#include "SDRTransceiver.h"
#include "../hackrf/hackrf.h"
#include <fstream>

class HRFTransceiver :
    public SDRTransceiver
{   
public:
	HRFTransceiver(hackrf_device* device);
	~HRFTransceiver();

	HRFTransceiver& operator==(const HRFTransceiver&) = delete;

	void Transfer() override;
	void Receive() override {};
	void SetParams(const HRFUtil::HRFParams params);
	int GetStreamingStatus() const;

	void Stop();

private:
	void EnableParamsForRXTX();
	void InitTransceiver();

	void StartConsoleLogger();

	void ConsoleLogger();
	void TransferMonitor();

	hackrf_device* m_device;
	HRFUtil::HRFParams m_params;
public:
	static void make_psk4_buffer(uint8_t* buffer, char* data, size_t size);
private:
	static int gettimeofday(struct timeval* tv, void* ignored);
	static float TimevalDiff(const struct timeval* a, const struct timeval* b);
};