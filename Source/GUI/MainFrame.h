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
private:
	wxButton* updateButton;
	wxChoice* hackrfChoiceList;
	wxPanel* m_paramsPanel;

    void UpdateHRFList(wxCommandEvent& evt);

    HRFDeviceList m_hrfDeviceList;
};