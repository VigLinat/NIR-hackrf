#pragma once
#include <iostream>
#include "Logger.h"
class ConsoleLogger :
    public Logger
{
public:
    void Log(const char* what) override;
private:
    
};

