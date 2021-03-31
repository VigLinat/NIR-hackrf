#include "MainFrame.h"
#include "../SDRException/NoHRFFound.h"
#include "ParamsPanel.h"
#include <exception>
MainFrame::MainFrame() 
	: wxFrame(NULL, wxID_ANY, "Visual SDR", wxPoint(50, 50), wxSize(800, 600))
{
	
	m_paramsPanel = new ParamsPanel(this);

	deviceChoiceList = new wxChoice(m_paramsPanel, ID_DEVICELIST, wxPoint(10, 10), wxSize(250, 30));
	updateButton = new wxButton(m_paramsPanel, ID_UPDATEDEVICE, "Update", wxPoint(270, 10), wxSize(100, 30));

	updateButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainFrame::UpdateDeviceList, this);

	m_deviceList = new HRFDeviceList;
}

void MainFrame::UpdateDeviceList(wxCommandEvent& evt)
{
	try
	{
		bool isNewDeviceDetected = false;
		m_deviceList->UpdateList();

		wxArrayString serialNumbers = m_deviceList->GetSerialNumbers();
		deviceChoiceList->Clear();
		for (auto it = serialNumbers.begin(); it != serialNumbers.end(); it++)
		{
			size_t maxn = (*it).length();
			wxString last4digits = (*it).SubString(maxn - 4, maxn - 1);
			deviceChoiceList->AppendString(wxString("HackRF: ") + last4digits);
		}
	}
	catch (const NoHRFFound& e)
	{
		wxMessageBox(e.What());
	}
}

MainFrame::~MainFrame()
{
	delete updateButton;
	delete deviceChoiceList;
	delete m_paramsPanel;

	delete m_deviceList;
}