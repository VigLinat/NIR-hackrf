#pragma once
#include "wx/wx.h"
#include "../SDRDeviceList/HRFDeviceList.h"
class MyApp :
    public wxApp
{
    virtual bool OnInit();
	int OnExit();
};
