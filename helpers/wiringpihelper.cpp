#include "wiringpihelper.h"
#include "helpers/logger.h"

# ifdef RPI
#include <wiringPi.h>
#include <mcp3004.h>
# endif

bool WiringPiHelper::_inited = false;
bool WiringPiHelper::_verbose = false;
int WiringPiHelper::_dataLength = 0;

bool WiringPiHelper::Init()
{
    _inited = false;
# ifdef RPI
    wiringPiSetup();
    _inited = true;
#else
    zInfo("wiringpi: no RaspberryPi detected");
# endif

    return true;
}


bool WiringPiHelper::McpSetup(const QVarLengthArray<McpSetupModel>& models)
{
    if(!WiringPiHelper::Inited()) return false;
    if(models.isEmpty()) return false;
#ifdef RPI
    _dataLength=0;
    for(auto&m:models){
        mcp3004Setup(m.base, m.spi_chanel);
        _dataLength+=m.chanels;
    }
    return true;
#else
    return false;
#endif

}

// logger_2v0
QVarLengthArray<int> WiringPiHelper::ReadMcp(const QVarLengthArray<McpSetupModel>& models)
{

    if(!_inited) return {};
    if(models.isEmpty()) return {};

    QVarLengthArray<int> v(_dataLength);
    int j=0;
    for(auto&m:models){
        for(int i=0;i<m.chanels;i++){
            v[j] = analogRead(m.base + i);
            if(_verbose)zInfo("d["+QString::number(j)+"]="+QString::number(v[j]));
            j++;
        }
    }
    return v;
}
