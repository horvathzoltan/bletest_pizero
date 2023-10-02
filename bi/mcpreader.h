#ifndef MCPREADER_H
#define MCPREADER_H

#include <QVarLengthArray>
#include "helpers/wiringpihelper.h"

class McpReader
{
private:
    static bool _inited;
    static QVarLengthArray<WiringPiHelper::McpSetupModel> _models;

public:
    static bool Init(const QString& typeName);
    static QVarLengthArray<int> GetValues();
};

#endif // MCPREADER_H
