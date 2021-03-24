#include "HRFDeviceList.h"
#include "../SDRException/NoHRFFound.h"
HRFDeviceList::HRFDeviceList()
{
	//UpdateList();
}

void HRFDeviceList::UpdateList()
{
	bool isInList = false;
	hackrf_device_list_t* list;

	list = hackrf_device_list();

	if (list->devicecount < 1)
	{
		throw new NoHRFFound();
	}
	else 
	{
		for (int i = 0; i < list->devicecount; i++)
		{
			for (auto it = m_hrfDeviceList.begin(); it != m_hrfDeviceList.end(); it++)
			{
				if (strcmp(list->serial_numbers[i], (*it)->GetSerialNumber()))
				{
					isInList = true;
					break;
				}
			}
			if (isInList)
			{
				break;
			}
			else 
			{
				m_hrfDeviceList.push_back(new HRFDevice(list->serial_numbers[i]));
			}
		}
	}
}