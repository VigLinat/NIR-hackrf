#include "MyApp.h"
#include "ParamsFrame.h"
#include "iostream"

bool MyApp::OnInit()
{
	AllocConsole();
	freopen("CONOUT$", "w+", stderr);

	std::cerr << "Hello VisualSDR!" << std::endl;

	ParamsFrame* frame = new ParamsFrame();
	frame->Show(true);

	return true;
}

wxIMPLEMENT_APP(MyApp);