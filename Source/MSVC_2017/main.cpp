#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif
#include <stdlib.h>
#include <iostream>
#include <string>
#include "../SDRDevice/SDRUtility.h"
#include "../SDRDevice/HRFDevice.h"
#include "../SDRException/SDRException.h"

int main(int argc, char** argv)
{
	std::cerr << "Hello VisualSDR!\n";

	SDRDevice* sdrDevice = new HRFDevice();
	std::wstring filepath = L"data.txt";

	try
	{
		sdrDevice->Init();
	}
	catch (SDRException e)
	{
		std::cerr << e.What();
	}

	sdrDevice->OpenFile(filepath);

	sdrDevice->SendData(HRFUtil::VSDR_MODULATION_PSK_2);
	delete sdrDevice;
	sdrDevice = nullptr;
	return 0;
}

