#pragma once
#include "wx/wx.h"
#include "ParamsPanel.h"
#include "../SDRDevice/HRFDevice.h"
#include "../SDRDeviceList/HRFDeviceList.h"

class MainFrame :
    public wxFrame
{
public:
    MainFrame();
    ~MainFrame();
private:
	wxButton* updateButton;
	wxChoice* deviceChoiceList;
	wxPanel* m_paramsPanel;

    void UpdateDeviceList(wxCommandEvent& evt);

    HRFDeviceList* m_deviceList;
};