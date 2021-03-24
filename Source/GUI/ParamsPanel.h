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
	ID_DEVICELIST,
    ID_UPDATEDEVICE,
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

struct frameContent
{
    std::string filename;
    uint64_t freq;
    uint32_t lna;
    uint32_t vga;
    uint32_t txvga;
};

class ParamsPanel :
    public wxPanel
{
public:
    ParamsPanel(wxWindow* parent);
    frameContent GetContent() const;

private:
    wxWindow* parentFrame;
    wxButton* updateButton;
	wxRadioBox* modeSelection;
    std::vector<wxButton*> buttonList;
    std::vector<std::pair<wxTextCtrl*, bool>> textList;

    frameContent params;
private:
    void OnButtonClicked(wxCommandEvent& evt);
    void OnTextCtrlClicked(wxMouseEvent& evt);

    void CreateButtons();
    void CreateRadioButtons();
    void CreateTexts();
};