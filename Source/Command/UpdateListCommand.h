#pragma once
#include "Command.h"
#include "../SDRDeviceList/SDRDeviceList.h"

class UpdateListCommand:
    public Command
{
    UpdateListCommand(const SDRDeviceList& deviceList);
    void Execute() override;
};
