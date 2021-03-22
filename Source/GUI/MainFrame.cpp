#include "MainFrame.h"
#include "../SDRException/NoHRFFound.h"
#include "ParamsPanel.h"
#include <exception>
MainFrame::MainFrame() 
	: wxFrame(NULL, wxID_ANY, "Visual SDR", wxPoint(50, 50), wxSize(800, 600))
{
	
	m_paramsPanel = new ParamsPanel(this);

	updateButton = new wxButton(m_paramsPanel, ID_UPDATEDEVICE, "Update", wxPoint(300, 10), wxSize(150, 50));

	updateButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainFrame::UpdateHRFList, this);
}

void MainFrame::UpdateHRFList(wxCommandEvent& evt)
{
	try
	{
		m_hrfDeviceList.UpdateList();
	}
	catch (NoHRFFound e)
	{
		/* show dialog box "No HRF Found!" */
	}
}