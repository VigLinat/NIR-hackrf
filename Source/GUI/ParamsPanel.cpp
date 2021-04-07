#include "ParamsPanel.h"
#include "FrameContent.h"
#include <iostream>
#include <string>
ParamsPanel::ParamsPanel(wxWindow* parent)
	: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_RAISED)
{
	CreateRadioButtons();
	CreateTexts();
}

wxString ParamsPanel::GetFrequency() const
{
	return frequency->GetLineText(0);
}

wxString ParamsPanel::GetFilename() const
{
	return filename->GetLineText(0);
}

RXTXMode ParamsPanel::GetRXTXMode() const
{
	return static_cast<RXTXMode>(modeSelection->GetSelection());
}

wxString ParamsPanel::GetTXVGA() const
{
	return txvga->GetLineText(0);
}

wxString ParamsPanel::GetLNA() const
{
	// default value is 20
	return "20";
}

wxString ParamsPanel::GetVGA() const
{
	//default value is 8
	return "8";
}

void ParamsPanel::OnTextCtrlClicked(wxMouseEvent& evt)
{
	//int id = evt.GetId();

	///*for (auto it = textList.begin(); it != textList.end(); it++)
	//{
	//	if ((*it).first->GetId() == id && (*it).second == false)
	//	{
	//		(*it).first->Clear();
	//		(*it).second = true;
	//	}
	//}*/

	//evt.Skip();
}

void ParamsPanel::CreateRadioButtons()
{
	const wxString choices[] = { "Tx", "Rx", "Rx WAV" };
	modeSelection = new wxRadioBox(this, ID_SETMODE, "Select Mode", wxPoint(10, 120), wxSize(100, 100), 2, choices);
}

void ParamsPanel::CreateTexts()
{

	filename = new wxTextCtrl(this, ID_SETFILE, "Enter filename", wxPoint(10, 70), wxSize(250, 30));
	frequency = new wxTextCtrl(this, ID_SETFREQ, "Enter frequency", wxPoint(10, 240), wxSize(150, 30));
	txvga = new wxTextCtrl(this, ID_SETTXVGA, "Enter TX VGA", wxPoint(10, 290), wxSize(150, 30));
}