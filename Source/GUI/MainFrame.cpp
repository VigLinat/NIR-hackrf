#include "MainFrame.h"
#include "../SDRException/NoHRFFound.h"
#include "ParamsPanel.h"
#include <exception>
MainFrame::MainFrame() 
	: wxFrame(NULL, wxID_ANY, "Visual SDR", wxPoint(50, 50), wxSize(800, 600))
{
	
	m_paramsPanel = new ParamsPanel(this);

	hackrfChoiceList = new wxChoice(m_paramsPanel, ID_DEVICELIST, wxPoint(10, 10), wxSize(250, 30));
	updateButton = new wxButton(m_paramsPanel, ID_UPDATEDEVICE, "Update", wxPoint(270, 10), wxSize(100, 30));

	updateButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainFrame::UpdateHRFList, this);
}

void MainFrame::UpdateHRFList(wxCommandEvent& evt)
{
	try
	{
		bool isNewDeviceDetected = false;
		isNewDeviceDetected = m_hrfDeviceList.UpdateList();
		if (isNewDeviceDetected)
		{
			wxArrayString hackrfArrayString = m_hrfDeviceList.GetDeviceList();
			hackrfChoiceList->Clear();
			for (auto it = hackrfArrayString.begin(); it != hackrfArrayString.end(); it++)
			{
				hackrfChoiceList->AppendString((*it));
			}
		}
	}
	catch (const SDRException& e)
	{
		wxMessageBox(e.What());
	}
}