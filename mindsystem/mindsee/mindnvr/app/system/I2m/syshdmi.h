#pragma once

#include "sys.hpp"
#include "isyshdmi.h"

class CSysHdmi : public CSys , public CISysHdmi
{
private:
    bool m_hdmi_status;
private:
    CSysHdmi(/* args */);
    ~CSysHdmi();

    void SetAttr();
public:
    static CSysHdmi* GetInstance();

    int Start() override;
    int Stop() override;
};