#ifndef HWINFO_H
#define HWINFO_H

#include <QString>

class HwInfo
{
public:
    struct Model{
        QString hwid;
        QString serial;
        QString type;

        static Model Parse(const QString& str);
        QString toString();
    };

    static const QString _value;
    static Model _model;
    static bool _inited;
    static QString _lastError;
private:
    static QString GetValue();
public:
    static bool Init();
    static QString Value(){return _model.toString();}
    static QString LastError(){return _lastError;}
    static QString HwType(){return _model.type;}
    static QString Serial(){return _model.serial;}
};

#endif // HWINFO_H
