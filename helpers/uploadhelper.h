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
        QString toString();
        bool isValid() const;
        static int Find(const QMap<int,MetaData>&m, const QString& fileName);
        static int NewKey(const QMap<int,MetaData>&m);
    };

    struct UploadModel{
        int key=-1;
        int offset=-1;
        QString data;        

        static UploadModel Parse(const QString& v);

        QString toString() const;
    };

    struct UploadResponseModel{
        int key=-1;
        int offset=-1;
        int flag=-1;

        QString toString() const;
    };


private:
    static QMap<int,MetaData> _metaData;

    static void LoadMeta();

public:
    static bool RemoveUpload(const QString &fileName);
    static UploadResponseModel AddUpload(const MetaData& m);
    static UploadResponseModel Upload(const UploadModel &m, bool isBase64);
    static void SaveMeta();
    static int AddNewMeta(const MetaData &m);
    static void RemoveMeta(int key);
};

#endif // UPLOADHELPER_H
