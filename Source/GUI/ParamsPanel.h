#pragma once
#include "wx/wx.h"
#include <vector>
#include <string>
#include "MainFrame.h"

enum
{
    ID_Hello = 1,
};

enum
{
    ID_SETALL,
    ID_STARTTX,
    ID_STOPTX,
	ID_DEVICELIST,
    ID_UPDATEDEVICE,
};

enum
{
    ID_SETFILE,
    ID_SETFREQ,
    ID_SETMODE,
    ID_SETLNA,
    ID_SETVGA,
    ID_SETTXVGA,
};

enum
{
    ID_RADIO_MODE,
    ID_RADIO_AMP,
};

typedef enum
{
    ID_TX_MODE = 0,
    ID_RX_MODE = 1,
    ID_RX_WAV_MODE = 2
} RXTXMode;

class ParamsPanel :
    public wxPanel
{
public:
    ParamsPanel(wxWindow* parent);
	ParamsPanel(wxWindow* parent, const wxPoint& point, const wxSize& size);
public:
    wxString GetFrequency() const;
    wxString GetFilename() const;
    RXTXMode GetRXTXMode() const;
    wxString GetTXVGA() const;
    wxString GetLNA() const;
    wxString GetVGA() const;
private:
    wxWindow* parentFrame;
	wxRadioBox* modeSelection;
    std::vector<std::pair<wxTextCtrl*, bool>> textList;

    wxTextCtrl* filename;
    wxTextCtrl* frequency;
    wxTextCtrl* txvga;
    

private:
    void OnTextCtrlClicked(wxMouseEvent& evt);
    void CreateRadioButtons();
    void CreateTexts();
};