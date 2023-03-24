#ifndef INSTANCE_H
#define INSTANCE_H

#include "helpers/shortguid.h"

class Instance
{
private:
    static const zShortGuid _instance_id;
public:
    static QString ToString(){ return _instance_id.value;}
};

#endif // INSTANCE_H
