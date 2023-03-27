#include "wiringpihelper.h"
#include "helpers/logger.h"

# ifdef RPI
#include <wiringPi.h>
#include <mcp3004.h>
# endif

bool WiringPiHelper::_inited = false;

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


QVarLengthArray<int> WiringPiHelper::ReadMcp(QVarLengthArray<int> bases)
{
    if(!_inited) return {};

    QVarLengthArray<int> v(16);
    for(auto&base:bases){
        for(int i=0;i<8;i++){
            int d = analogRead (base + i);
            v[i] = d;
            zInfo("d"+QString::number(i)+"="+QString::number(d));
        }
    }
    return v;
}
