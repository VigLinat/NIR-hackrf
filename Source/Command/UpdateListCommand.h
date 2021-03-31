#pragma once
#include "Command.h"
#include "../SDRDeviceList/SDRDeviceList.h"
#include "../SdrException/NoHRFFound.h"

class UpdateListCommand:
    public Command
{

public:
	void Execute() override;
	UpdateListCommand(SDRDeviceList* deviceList);
	std::vector<const char*> GetSerialNumbers() const;
private:
	SDRDeviceList* m_deviceList;
	std::vector<const char*> m_serialNumbers;
};
