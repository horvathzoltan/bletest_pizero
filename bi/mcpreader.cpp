#include "mcpreader.h"
#include "helpers/wiringpihelper.h"

# define BASE 100
# define SPI_CHAN 0
# define BASE1 140
# define SPI_CHAN1 1


QVarLengthArray<int> McpReader::GetValues()
{
    QVarLengthArray<int>  a = WiringPiHelper::ReadMcp({BASE, BASE1});
    return a;
}
