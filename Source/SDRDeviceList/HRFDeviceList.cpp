#include "HRFDeviceList.h"
#include "../SDRUtility/SDRUtility.h"
#include "../SDRException/NoHRFFound.h"
HRFDeviceList::HRFDeviceList()
{

}

wxArrayString&& HRFDeviceList::GetSerialNumbers() const 
{
	wxArrayString deviceNameList;
	for (auto it = m_hrfDeviceList.begin(); it != m_hrfDeviceList.end(); it++)
	{
		deviceNameList.Add(wxString((*it)->GetSerialNumber()));
	}
	return std::move(deviceNameList);
}

void HRFDeviceList::UpdateList()
{
	hackrf_device_list_t* list = hackrf_device_list();

	if (list->devicecount < 1)
	{
		throw NoHRFFound();
	}
	
	for (auto it = m_hrfDeviceList.begin(); it != m_hrfDeviceList.end(); it++)
	{
		delete (*it);
	}

	for (int i = 0; i < list->devicecount; i++)
	{
		m_hrfDeviceList.push_back(new HRFDevice(list->serial_numbers[i]));
	}

	hackrf_device_list_free(list);
}
