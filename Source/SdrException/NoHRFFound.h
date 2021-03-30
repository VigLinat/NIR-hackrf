#pragma once
#include "SDRException.h"
class NoHRFFound :
    public SDRException
{
public:
    NoHRFFound();
    
    std::string What() const override;
};

