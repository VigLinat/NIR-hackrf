#include "../hackrf/hackrf.h"
#include "MyApp.h"
#include "MainFrame.h"
#include "ParamsPanel.h"
#include "iostream"

bool MyApp::OnInit()
{
	int result = 0;

	AllocConsole();
	freopen("CONOUT$", "w+", stderr);

	std::cerr << "Hello VisualSDR!" << std::endl;

	result = hackrf_init();
	if (result != HACKRF_SUCCESS) 
	{
		fprintf(stderr, "hackrf_init() failed: %s (%d)\n",
			hackrf_error_name((hackrf_error)result), result);
		return EXIT_FAILURE;
	}

	MainFrame* frame = new MainFrame();
	frame->Show(true);

	return true;
}

wxIMPLEMENT_APP(MyApp);