#include "MainFrame.h"
#include "../SDRException/NoHRFFound.h"
#include <exception>

MainFrame::MainFrame() 
	: wxFrame(NULL, wxID_ANY, "Visual SDR", wxPoint(50, 50), wxSize(800, 600))
{
	m_deviceChoiceList = new wxChoice(this, ID_DEVICELIST, wxPoint(10, 10), wxSize(250, 30));
	m_updateButton = new wxButton(this, ID_UPDATEDEVICE, "Update", wxPoint(270, 10), wxSize(100, 30));
	m_setParams = new wxButton(this, ID_SETALL, "Set Params", wxPoint(100, 440), wxSize(150, 50));
	m_startTX = new wxButton(this, ID_STARTTX, "Start TX", wxPoint(100, 500), wxSize(150, 50));

	m_updateButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainFrame::UpdateDeviceList, this);
	m_setParams->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainFrame::OnSetParams, this);
	m_startTX->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainFrame::OnStartTX, this);
	//this->Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnExit);
	m_deviceList = new HRFDeviceList;
}

void MainFrame::UpdateDeviceList(wxCommandEvent& evt)
{
	UpdateListCommand command{ m_deviceList };
	command.Execute();
	m_serialNumbers = command.GetSerialNumbers();
	m_deviceChoiceList->Clear();
	m_paramsPanel.clear();
	if (m_serialNumbers.size() < 1)
	{
		wxMessageBox("No connected SDR boards Found!");
		return;
	}
	else
	{
		std::sort(m_serialNumbers.begin(), m_serialNumbers.end());
		for (auto device : m_serialNumbers)
		{
			size_t maxn = strlen(device);
			std::string last4digits = &device[maxn - 4];
			m_deviceChoiceList->AppendString(std::string("HackRF: ") + last4digits);
			m_paramsPanel.push_back(new ParamsPanel(this, wxPoint(10, 50), wxSize(450, 320)));
		}
	}
}

void MainFrame::OnStartTX(wxCommandEvent& evt)
{
	int currentSelection = m_deviceChoiceList->GetCurrentSelection();
	if (currentSelection == wxNOT_FOUND)
	{
		wxMessageBox("Please select the HackRF board first");
		return;
	}
	const char* serial = m_serialNumbers[currentSelection];

	HRFDevice* targetDevice = m_deviceList->GetDeviceBySerial(serial);
	try
	{
		targetDevice->SendData(HRFUtil::MODULATIONS::VSDR_MODULATION_PSK_4);
	}
	catch (const SDRException& e)
	{
		wxMessageBox(e.What());
	}
}

void MainFrame::OnSetParams(wxCommandEvent& evt) 
{
	int currentSelection = m_deviceChoiceList->GetCurrentSelection();
	if (currentSelection == wxNOT_FOUND)
	{
		wxMessageBox("Please select the HackRF board first");
		return;
	}
	const char* serial = m_serialNumbers[currentSelection];

	ParamsPanel* currentPanel = m_paramsPanel[currentSelection];
	HRFUtil::HRFParams params;

	switch (currentPanel->GetRXTXMode())
	{
	case (ID_TX_MODE):
		params.transmit = true;
		break;
	case(ID_RX_MODE):
		params.receive = true;
		break;
	case(ID_RX_WAV_MODE):
		params.receive_wav = true;
		break;
	}
	unsigned long temp = 0;
	
	int result = 0;
	params.filename = currentPanel->GetFilename().ToStdString();
	
	result = currentPanel->GetFrequency().ToLongLong(&params.freq_hz);
	if (!result)
	{
		wxMessageBox("Pleae specify correct frequency");
		return;
	}
	result = currentPanel->GetTXVGA().ToULong(&temp);
	if (!result)
	{
		wxMessageBox("Pleae specify correct amplification");
		return;
	}
	params.txvga_gain = static_cast<uint32_t>(temp);
	try 
	{
		m_deviceList->SetParamsBySerial(serial, params);
	}
	catch (const SDRException& e)
	{
		wxMessageBox(e.What());
	}
}


void MainFrame::OnExit(wxCloseEvent& evt)
{
	delete m_updateButton;
	delete m_deviceChoiceList;
	delete m_deviceList;
	evt.Skip();
}
