#pragma once
#include "wx/wx.h"
#include <list>
#include <string>
#include "Frame.h"

enum
{
    ID_Hello = 1,
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

class FrameContent
{
public:
    FrameContent();
    ~FrameContent();

private:


    std::list<wxTextCtrl*> text_list;
    std::list<wxButton*> button_list;

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

