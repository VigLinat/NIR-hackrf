#pragma once
#include "wx/wx.h"
#include "../SDRDeviceList/HRFDeviceList.h"
class MyApp :
    public wxApp
{
	~MyApp();
    virtual bool OnInit();
};
