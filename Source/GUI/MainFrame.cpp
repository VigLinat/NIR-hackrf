#include "MainFrame.h"
#include "../SDRException/NoHRFFound.h"
#include "ParamsPanel.h"
#include <exception>

MainFrame::MainFrame() 
	: wxFrame(NULL, wxID_ANY, "Visual SDR", wxPoint(50, 50), wxSize(800, 600))
{
	
	m_paramsPanel = new ParamsPanel(this);

	m_deviceChoiceList = new wxChoice(m_paramsPanel, ID_DEVICELIST, wxPoint(10, 10), wxSize(250, 30));
	m_updateButton = new wxButton(m_paramsPanel, ID_UPDATEDEVICE, "Update", wxPoint(270, 10), wxSize(100, 30));

	m_updateButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainFrame::UpdateDeviceList, this);

	m_deviceList = new HRFDeviceList;
}

void MainFrame::UpdateDeviceList(wxCommandEvent& evt)
{
	m_deviceChoiceList->Clear();
	UpdateListCommand command{ m_deviceList };
	command.Execute();
	std::vector<const char*> serials = command.GetSerialNumbers();
	if (serials.size() < 1)
	{
		wxMessageBox("No connected SDR boards Found!");
		return;
	}
	else
	{
		for (auto device : serials)
		{
			size_t maxn = strlen(device);
			std::string last4digits = &device[maxn - 4];
			m_deviceChoiceList->AppendString(std::string("HackRF: ") + last4digits);
		}
	}
}

MainFrame::~MainFrame()
{
	delete m_updateButton;
	delete m_deviceChoiceList;
	delete m_paramsPanel;
	delete m_deviceList;
}