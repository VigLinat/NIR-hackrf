#pragma once
#include "wx/wx.h"
#include "ParamsPanel.h"
#include "../SDRDevice/HRFDevice.h"
#include "../SDRDeviceList/HRFDeviceList.h"
#include "../SDRDeviceList/SDRDeviceList.h"
#include "../Command/UpdateListCommand.h"
#include "../Command/SetParamsCommand.h"

class ParamsPanel;

class MainFrame :
    public wxFrame
{
public:
    MainFrame();
private:
	void OnSetParams(wxCommandEvent& evt);
	void OnStartTX(wxCommandEvent& evt);
	void UpdateDeviceList(wxCommandEvent& evt);
	void OnExit(wxCloseEvent& evt);
	wxButton* m_startTX;
	wxButton* m_setParams;
	wxButton* m_updateButton;
	wxChoice* m_deviceChoiceList;
	std::vector<ParamsPanel*> m_paramsPanel;
	std::vector<const char*> m_serialNumbers;
    HRFDeviceList* m_deviceList;
};