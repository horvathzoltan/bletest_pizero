#ifndef WIRINGPIHELPER_H
#define WIRINGPIHELPER_H

#include <QVarLengthArray>


class WiringPiHelper
{
private:
    static bool _inited;
public:
    static bool Init();
    static bool Inited(){return _inited;}
    static QVarLengthArray<int> ReadMcp(QVarLengthArray<int> bases);
};

#endif // WIRINGPIHELPER_H
