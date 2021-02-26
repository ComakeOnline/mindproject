#pragma once


#include "global.hpp"

class CISysHdmi
{
public:
    DISABLE_COPY(CISysHdmi);
    DISABLE_EQUAL(CISysHdmi);

    CISysHdmi(/* args */){};
    virtual ~CISysHdmi(){};

public:
public:

    virtual int Start() = 0;
    virtual int Stop() = 0;
    
private:
};


