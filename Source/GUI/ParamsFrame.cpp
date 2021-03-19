#include "ParamsFrame.h"
#include "FrameContent.h"
#include <iostream>
#include <string>
ParamsFrame::ParamsFrame()
	: wxFrame(NULL, wxID_ANY, "Hello World", wxPoint(30, 30), wxSize(800, 600))
{
	CreateButtons();
	CreateRadioButtons();
	CreateTexts();
}

frameContent ParamsFrame::GetContent() const 
{
	return params;
}

void ParamsFrame::CreateButtons()
{
	button_list.push_back(
		new wxButton(this, ID_SETALL, "Set Params", wxPoint(10, 280), wxSize(150, 50))
	);

	/*wxSize btn_size = wxSize(button_width, button_height);

	button_list.push_back(
		new wxButton(this, ID_SETFILE, "Set Filename", wxPoint(10, 10), btn_size)
	);

	button_list.push_back(
		new wxButton(this, ID_SETFREQ, "Set Frequency", wxPoint(10, 110), btn_size)
	);

	button_list.push_back( 
		new wxButton(this, ID_SETTXVGA, "Set TX VGA gain", wxPoint(10, 170), btn_size)
	);
	*/
	for (auto& btn : button_list)
	{
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &ParamsFrame::OnButtonClicked, this, btn->GetId());
	}
}

void ParamsFrame::CreateRadioButtons()
{

}

void ParamsFrame::CreateTexts()
{	
	text_list.push_back(
		new wxTextCtrl(this, wxID_ANY, "Enter filename", wxPoint(10, 10), wxSize(150, 30))
	);

	text_list.push_back(
		new wxTextCtrl(this, ID_SETFILE, "", wxPoint(10, 50), wxSize(300, 30))
	);

	text_list.push_back(
		new wxTextCtrl(this, wxID_ANY, "Enter frequency", wxPoint(10, 90), wxSize(100, 30))
	);

	text_list.push_back(
		new wxTextCtrl(this, ID_SETFREQ, "", wxPoint(10, 130), wxSize(150, 30))
	);

	text_list.push_back(
		new wxTextCtrl(this, wxID_ANY, "Enter TX VGA", wxPoint(10, 170), wxSize(150, 30))
	);

	text_list.push_back(
		new wxTextCtrl(this, ID_SETTXVGA, "", wxPoint(10, 210), wxSize(100, 30))
	);
}

void ParamsFrame::OnButtonClicked(wxCommandEvent& evt)
{
	if (evt.GetId() == ID_SETALL) {
		for (auto it = text_list.begin(); it != text_list.end(); it++)
		{
			switch ((*it)->GetId())
			{
			case ID_SETFILE:
				params.filename = (*it)->GetValue();
				break;
			case ID_SETFREQ:
				params.freq = atoll((*it)->GetValue());
				break;
			case ID_SETLNA:
				params.lna = atoi((*it)->GetValue());
				break;
			case ID_SETVGA:
				params.vga = atoi((*it)->GetValue());
				break;
			case ID_SETTXVGA:
				params.txvga = atoi((*it)->GetValue());
				break;
			default:
				params.lna = 8;
				params.vga = 20;
				params.txvga = 0;
				break;
			}
		}
	}

	std::cerr << params.filename << "\n" << params.freq << "\n"
		<< params.lna << "\n" << params.vga << "\n" << params.txvga << std::endl;
}