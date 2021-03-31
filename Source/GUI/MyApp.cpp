#include "../hackrf/hackrf.h"
#include "MyApp.h"
#include "MainFrame.h"
#include "ParamsPanel.h"
#include "iostream"

bool MyApp::OnInit()
{
	AllocConsole();
	freopen("CONOUT$", "w+", stderr);

	std::cerr << "Hello VisualSDR!" << std::endl;
	
	int result = 0;
	result = hackrf_init();
	if (result != HACKRF_SUCCESS) 
	{
		fprintf(stderr, "hackrf_init() failed: %s (%d)\n",
			hackrf_error_name((hackrf_error)result), result);
		return EXIT_FAILURE;
	}

	wxFrame* mainFrame = new MainFrame();
	mainFrame->Show(true);
	return true;
}

MyApp::~MyApp()
{
	FreeConsole();
	hackrf_exit();
	fprintf(stderr, "hackrf_exit() done\n");
	std::cerr << "exit" << std::endl;
}

wxIMPLEMENT_APP(MyApp);