#pragma once
#include "wx/wx.h"
#include "ParamsPanel.h"
#include "../SDRDevice/HRFDevice.h"
#include "../SDRDeviceList/HRFDeviceList.h"
#include "../SDRDeviceList/SDRDeviceList.h"
#include "../Command/UpdateListCommand.h"

class MainFrame :
    public wxFrame
{
public:
    MainFrame();
    ~MainFrame();
private:
	void UpdateDeviceList(wxCommandEvent& evt);

	wxButton* m_updateButton;
	wxChoice* m_deviceChoiceList;
	wxPanel* m_paramsPanel;
    HRFDeviceList* m_deviceList;
};