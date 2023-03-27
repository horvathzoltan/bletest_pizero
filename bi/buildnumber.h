#ifndef BUILDNUMBER_H
#define BUILDNUMBER_H

#define BUILDNUMBER 1061

#include <QString>

class Buildnumber{
public:
    static const QString _value;
private:
    static QString GetValue();
};

#endif // BUILDNUMBER_H


