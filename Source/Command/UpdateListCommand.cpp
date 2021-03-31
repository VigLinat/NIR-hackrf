#include "UpdateListCommand.h"
UpdateListCommand::UpdateListCommand(SDRDeviceList* deviceList)
{
	m_deviceList = deviceList;
}

void UpdateListCommand::Execute()
{
	m_deviceList->UpdateList();
	m_serialNumbers = m_deviceList->GetSerialNumbers();
}

std::vector<const char*> UpdateListCommand::GetSerialNumbers() const
{
	return m_serialNumbers;
}