#include "MyApp.h"
#include "Frame.h"

bool MyApp::OnInit()
{
	Frame* frame = new Frame();
	frame->Show(true);
	return true;
}

wxIMPLEMENT_APP(MyApp);