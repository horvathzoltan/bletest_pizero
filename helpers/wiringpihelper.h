#ifndef WIRINGPIHELPER_H
#define WIRINGPIHELPER_H

#include <QVarLengthArray>

# ifdef RPI
# define BASE_0 100
# define SPI_0 0
# define BASE_1 140
# define SPI_1 1
# endif

// 3004 and 3008 are the same 4/8 channels
# define MCP3004_CHANNELS 4
# define MCP3008_CHANNELS 8

class WiringPiHelper
{
private:
    static bool _inited;
    static bool _verbose;
    static int _dataLength;
public:
    struct McpSetupModel{
        int base;
        int spi_chanel;
        int chanels;
    };

    static bool Init();
    static bool Inited(){return _inited;}
    static int DataLength(){return _dataLength;}
    static void setVerbose(bool v){_verbose=v;}

    static bool McpSetup(const QVarLengthArray<McpSetupModel>& model);

    static QVarLengthArray<int> ReadMcp(const QVarLengthArray<McpSetupModel>& models);
};

#endif // WIRINGPIHELPER_H
