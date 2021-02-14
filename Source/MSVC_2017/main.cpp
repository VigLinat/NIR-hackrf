#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif
#include <stdlib.h>
#include <iostream>
#include <string>
#include "../SDRDevice/SdrDevice.h"
#include "../SDRDevice/HRFDevice.h"
#include "../SDRException/SDRException.h"
#include "../HRFCmdParser/HRFCmdParser.h"

int main(int argc, char** argv)
{
	std::cerr << "Hello VisualSDR!\n";

	SDRDevice* sdrDevice = new HRFDevice();
	std::wstring filepath = L"data.txt";

	HRFUtil::HRFParams cmdLineParams;
	try
	{
		HRFUtil::HRFParams cmdLineParams = HRFCmdParser::ParseCommandLine(argc, argv);
		sdrDevice->SetCmdLineParams(cmdLineParams);
		sdrDevice->SetFilename(filepath);
		sdrDevice->Init();
	}
	catch (SDRException e)
	{
		std::cerr << e.What() << std::endl;
	}
	sdrDevice->SendData(HRFUtil::VSDR_MODULATION_PSK_2);

	delete sdrDevice;
	sdrDevice = nullptr;
	return 0;
}

