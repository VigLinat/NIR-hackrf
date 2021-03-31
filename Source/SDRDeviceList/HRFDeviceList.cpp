#include "HRFDeviceList.h"
#include "../SDRUtility/SDRUtility.h"
#include "../SDRException/NoHRFFound.h"
HRFDeviceList::HRFDeviceList()
{

}

void HRFDeviceList::UpdateList()
{
	m_hrfDeviceList.clear();
	hackrf_device_list_t* list = hackrf_device_list();
	if (list->devicecount < 1)
		return;
	else
	{
		for (int i = 0; i < list->devicecount; i++)
		{
			m_hrfDeviceList.push_back(new HRFDevice(list->serial_numbers[i]));
		}
		hackrf_device_list_free(list);
	}
}

std::vector<const char*> HRFDeviceList::GetSerialNumbers() const
{
	size_t count = m_hrfDeviceList.size();
	std::vector<const char*> serial(count);
	int i = 0;
	for (auto device : m_hrfDeviceList)
	{
		serial[i++] = device->GetSerialNumber();
	}
	return serial;
}