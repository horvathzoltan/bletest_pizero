#ifndef MCPREADER_H
#define MCPREADER_H

#include "qglobal.h"
#include <QVarLengthArray>

class McpReader
{
public:
    static QVarLengthArray<int> GetValues();
};

#endif // MCPREADER_H
