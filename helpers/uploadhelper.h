#ifndef UPLOADHELPER_H
#define UPLOADHELPER_H

#include <QMap>
#include <QString>



class UploadHelper
{
public:
    struct MetaData{
        QString fileName;
        int fileSize=-1;

        static MetaData Parse(const QString& v);
        bool isValid() const;
    };

private:
    static QMap<int,MetaData> _metaData;

    static void LoadMeta();
public:
    static int AddUpload(const MetaData& m);
};

#endif // UPLOADHELPER_H
