#pragma once
#include "wx/wx.h"
#include <list>
#include <string>

enum
{
    ID_Hello = 1,
};

enum
{
    ID_SETALL,
    ID_SETFILE,
    ID_SETFREQ,
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

class ParamsFrame :
    public wxFrame
{
public:
    ParamsFrame();
    frameContent GetContent() const;
private:
    std::list<wxButton*> button_list;
    std::list<wxTextCtrl*> text_list;

    

    frameContent params;

    wxString str;
    
    const unsigned int button_width = 150;
    const unsigned int button_height = 50;
    const unsigned int button_padding = 10;

    void OnButtonClicked(wxCommandEvent& evt);

    void CreateButtons();
    void CreateRadioButtons();
    void CreateTexts();
};

