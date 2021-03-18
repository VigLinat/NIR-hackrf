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
    ID_BTN_SETFILE,
    ID_BTN_SETFREQ,
    ID_BTN_SETLNA,
    ID_BTN_SETVGA,
    ID_BTN_SETTXVGA,
};

enum
{
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

enum
{
    ID_TXT_FILENAME,
    ID_TXT_MODE,
    ID_TXT_FREQ,
    ID_TXT_AMP,
    ID_TXT_LNA,
    ID_TXT_VGA,
    ID_TXT_TXVGA,
};

class Frame :
    public wxFrame
{
public:
    Frame();
private:
    std::list<wxButton*> button_list;
    std::list<wxTextCtrl*> text_list;

    wxListBox* listbox;

    struct frameContent 
    {
        std::string filename;
        uint32_t freq;
        uint32_t lna;
        uint32_t vga;
        uint32_t txvga;
    } params;

    const unsigned int button_width = 150;
    const unsigned int button_height = 50;
    const unsigned int button_padding = 10;

    void OnButtonClicked(wxCommandEvent& evt);

    void CreateButtons();
    void CreateRadioButtons();
    void CreateTexts();
};

