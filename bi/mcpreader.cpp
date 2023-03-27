#include "bi/mcpreader.h"
#include "helpers/wiringpihelper.h"

bool McpReader::_inited = false;
QVarLengthArray<WiringPiHelper::McpSetupModel> McpReader::_models;

bool McpReader::Init(const QString& typeName)
{
    _inited = false;

    if(typeName.compare(QStringLiteral("logger_2v0"), Qt::CaseInsensitive)){
        _models = {
            {BASE_0, SPI_0, MCP3008_CHANNELS},
            {BASE_1, SPI_1, MCP3008_CHANNELS}
        };
    } else if(typeName.compare(QStringLiteral("logger_1v0"), Qt::CaseInsensitive)){
        _models = {
            {BASE_0, SPI_0, MCP3008_CHANNELS},
            {BASE_1, SPI_1, MCP3008_CHANNELS}
        };
    }

    bool ok = WiringPiHelper::McpSetup(_models);
    if(!ok) return false;
    _inited = true;
    return true;
}

QVarLengthArray<int> McpReader::GetValues()
{
    QVarLengthArray<int>  a = WiringPiHelper::ReadMcp(_models);
    return a;
}
