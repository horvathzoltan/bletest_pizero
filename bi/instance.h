#ifndef INSTANCE_H
#define INSTANCE_H

#include <QString>

class Instance
{
public:
    static const QString _value;
private:
    static QString GetValue();
};

#endif // INSTANCE_H
