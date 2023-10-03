#ifndef HASHHELPER_H
#define HASHHELPER_H

#include <QString>



class HashHelper
{
public:
    struct Model{
        QString fileName;
        QByteArray hash0;

        static Model Parse(const QString& v);
        bool isValid() const;
    };


    static bool Hash(const Model &m);
};

#endif // HASHHELPER_H
