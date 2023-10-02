#include "helpers/shortguid.h"
#include "instance.h"

const QString Instance::_value = GetValue();

QString Instance::GetValue()
{
    return zShortGuid::createNew().value;
}
