#include "ParamsPanel.h"
#include "FrameContent.h"
#include <iostream>
#include <string>
ParamsPanel::ParamsPanel(wxWindow* parent)
	: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_RAISED)
{
	CreateButtons();
	CreateRadioButtons();
	CreateTexts();
}

void ParamsPanel::OnButtonClicked(wxCommandEvent& evt)
{
	if (evt.GetId() == ID_SETALL) {
		for (auto it = textList.begin(); it != textList.end(); it++)
		{
			switch ((*it).first->GetId())
			{
			case ID_SETFILE:
				params.filename = (*it).first->GetValue();
				break;
			case ID_SETFREQ:
				params.freq = atoll((*it).first->GetValue());
				break;
			case ID_SETLNA:
				params.lna = atoi((*it).first->GetValue());
				break;
			case ID_SETVGA:
				params.vga = atoi((*it).first->GetValue());
				break;
			case ID_SETTXVGA:
				params.txvga = atoi((*it).first->GetValue());
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

void ParamsPanel::OnTextCtrlClicked(wxMouseEvent& evt)
{
	int id = evt.GetId();

	for (auto it = textList.begin(); it != textList.end(); it++)
	{
		if ((*it).first->GetId() == id && (*it).second == false)
		{
			(*it).first->Clear();
			(*it).second = true;
		}
	}

	evt.Skip();
}

frameContent ParamsPanel::GetContent() const
{
	return params;
}

void ParamsPanel::CreateButtons()
{
	buttonList.push_back(
		new wxButton(this, ID_SETALL, "Set Params", wxPoint(10, 280), wxSize(150, 50))
	);

	for (auto& btn : buttonList)
	{
		btn->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &ParamsPanel::OnButtonClicked, this);
	}
}

void ParamsPanel::CreateRadioButtons()
{

}

void ParamsPanel::CreateTexts()
{
	textList.push_back(
		std::pair<wxTextCtrl*, bool>(
			new wxTextCtrl(this, ID_SETFILE, "Enter filename", wxPoint(10, 70), wxSize(250, 30)),
			false
			)
	);

	textList.push_back(
		std::pair<wxTextCtrl*, bool>(
			new wxTextCtrl(this, ID_SETFREQ, "Enter frequency", wxPoint(10, 120), wxSize(150, 30)),
			false
			)
	);

	textList.push_back(
		std::pair<wxTextCtrl*, bool>(
			new wxTextCtrl(this, ID_SETTXVGA, "Enter TX VGA", wxPoint(10, 170), wxSize(150, 30)),
			false
			)
	);



	for (auto& list : textList)
	{
		list.first->Bind(wxEVT_LEFT_DOWN, &ParamsPanel::OnTextCtrlClicked, this);
	}
}