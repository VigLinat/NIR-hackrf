#include "HRFDeviceList.h"
#include "../SDRUtility/SDRUtility.h"
#include "../SDRException/NoHRFFound.h"
HRFDeviceList::HRFDeviceList()
{
	//UpdateList();
}

HRFDeviceList::~HRFDeviceList()
{
	hackrf_device_list_free(m_list);
}

wxArrayString HRFDeviceList::GetDeviceList() const
{
	wxArrayString deviceNameList;
	for (auto it = m_hrfDeviceList.begin(); it != m_hrfDeviceList.end(); it++)
	{
		const char* serial = (*it)->GetSerialNumber();
		uint32_t maxn = strlen(serial);
		wxString last4digits = wxString(serial).SubString(maxn - 4, maxn - 1);

		deviceNameList.Add(wxString("HackRF: ") + last4digits);
	}
	return deviceNameList;
}

bool HRFDeviceList::UpdateList()
{
	bool isNewDeviceDetected = false;

	m_list = hackrf_device_list();
	int count = m_list->devicecount;
	if (m_list->devicecount < 1)
	{
		throw NoHRFFound();
	}
	else
	{
		bool shouldUpdateList = false;
		for (int i = 0; i < m_list->devicecount; i++)
		{
			for (auto it = m_hrfDeviceList.begin(); it != m_hrfDeviceList.end(); it++)
			{
				if ((strcmp(m_list->serial_numbers[i], (*it)->GetSerialNumber())) == 0)
				{
					shouldUpdateList = true;
				}
			}
			if (!shouldUpdateList)
			{
				isNewDeviceDetected = true;
				m_hrfDeviceList.push_back(new HRFDevice(m_list->serial_numbers[i]));
			}
		}
	}

	return isNewDeviceDetected ? true : false;
}