#include "FrameContent.h"

void FrameContent::CreateButtons()
{
	wxSize btn_size = wxSize(button_width, button_height);

	button_list.push_back(
		new wxButton(this, ID_SETFILE, "Set Filename", wxPoint(10, 10), btn_size)
	);

	button_list.push_back(
		new wxButton(this, ID_SETFREQ, "Set Frequency", wxPoint(10, 110), btn_size)
	);

	button_list.push_back(
		new wxButton(this, ID_SETLNA, "Set LNA gain", wxPoint(10, 210), btn_size)
	);

	button_list.push_back(
		new wxButton(this, ID_SETVGA, "Set VGA gain", wxPoint(170, 210), btn_size)
	);

	button_list.push_back(
		new wxButton(this, ID_SETTXVGA, "Set TX VGA gain", wxPoint(330, 210), btn_size)
	);

	for (auto& btn : button_list)
	{
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &Frame::OnButtonClicked, this, btn->GetId());
	}
}

void FrameContent::CreateRadioButtons()
{

}

void FrameContent::CreateTexts()
{
	text_list.push_back(
		new wxTextCtrl(this, ID_SETFILE, "", wxPoint(10, 70), wxSize(300, 30))
	);

	text_list.push_back(
		new wxTextCtrl(this, ID_SETFREQ, "", wxPoint(10, 170), wxSize(150, 30))
	);

	text_list.push_back(
		new wxTextCtrl(this, ID_SETLNA, "", wxPoint(10, 270), wxSize(150, 30))
	);

	text_list.push_back(
		new wxTextCtrl(this, ID_SETVGA, "", wxPoint(170, 270), wxSize(150, 30))
	);

	text_list.push_back(
		new wxTextCtrl(this, ID_SETTXVGA, "", wxPoint(330, 270), wxSize(150, 30))
	);
}

void FrameContent::OnButtonClicked(wxCommandEvent& evt)
{
	int id = evt.GetId();
	wxTextCtrl* textBox = nullptr;

	for (auto it = text_list.begin(); it != text_list.end(); it++)
	{
		if ((*it)->GetId() == id)
		{
			textBox = *it;
			break;
		}
	}

	if (textBox != nullptr) {
		switch (id)
		{
		case ID_SETFILE:
			params.filename = textBox->GetValue();
			break;
		case ID_SETFREQ:
			params.freq = atoi(textBox->GetValue());
			break;
		case ID_SETLNA:
			params.lna = atoi(textBox->GetValue());
			break;
		case ID_SETVGA:
			params.vga = atoi(textBox->GetValue());
			break;
		case ID_SETTXVGA:
			params.txvga = atoi(textBox->GetValue());
			break;
		default:
			params.lna = 8;
			params.vga = 20;
			params.txvga = 0;
			break;
		}
	}
}