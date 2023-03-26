#ifndef HWINFO_H
#define HWINFO_H

#include <QString>

class HwInfo
{
public:
    static const QString _value;
private:
    static QString GetValue();
};

#endif // HWINFO_H
