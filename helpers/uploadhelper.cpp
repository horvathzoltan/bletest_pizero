#include "filenamehelper.h"
#include "helpers/logger.h"
#include "helpers/textfilehelper.h"
#include "uploadhelper.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>

QMap<int,UploadHelper::MetaData> UploadHelper::_metaData;

UploadHelper::MetaData UploadHelper::MetaData::Parse(const QString &v)
{
    bool valid = !v.isEmpty();
    MetaData m;

    if(valid){
        QStringList lines = v.split(',');
        valid = !lines.isEmpty() && lines.length()>=2;
        if(valid){
            m.fileName=lines[0];
            bool ok;
            int s = lines[1].toInt(&ok);
            if(ok){
                m.fileSize = s;
            }
        }
    }
    return m;
}

QString UploadHelper::MetaData::toString()
{
    QString str = fileName+','+QString::number(fileSize);
    return str;
}

bool UploadHelper::MetaData::isValid() const
{
    bool valid = fileSize>0 && !fileName.isEmpty();

    return valid;
}

int UploadHelper::MetaData::Find(const QMap<int, MetaData> &m, const QString &fileName)
{
    int retVal = -1;

    for(auto i = m.keyValueBegin(),end=m.keyValueEnd();i!=end;i++){
        if(i->second.fileName == fileName)
        {
            retVal =  i->first;
            break;
        }
    }
    return retVal;
}

int UploadHelper::MetaData::NewKey(const QMap<int, MetaData> &m)
{
    int retVal = -1;

    for(auto i = m.keyValueBegin(),end=m.keyValueEnd();i!=end;i++){
        if(i->first > retVal)
        {
            retVal = i->first;
        }
    }
    return retVal+1;
}


void UploadHelper::LoadMeta()
{
    QString fn = FileNameHelper::UploadMetaFileName();
    QFileInfo fi(fn);
    bool metaExists = fi.exists();
    if(metaExists){
        QStringList lines;
        bool metaLoaded = TextFileHelper::LoadLines(fn, &lines);
        if(metaLoaded){
            for(auto&line:lines){
                int ix = line.indexOf('=');
                if(ix>0){
                    QString keyStr = line.left(ix);
                    QString valueStr = line.mid(ix+1);

                    bool ok;
                    int key = keyStr.toInt(&ok);
                    if(ok){
                        auto m = MetaData::Parse(valueStr);
                        if(m.isValid()){
                            _metaData.insert(key, m);
                        }
                    }
                } else continue;
            }
        }
    }
}

void UploadHelper::SaveMeta(){
    QString fn = FileNameHelper::UploadMetaFileName();

    QStringList lines;
    for(auto i = _metaData.keyValueBegin(),end=_metaData.keyValueEnd();i!=end;i++){
        QString str = QString::number(i->first)+'='+i->second.toString();
        lines.append(str);
    }

    bool metaSaved = TextFileHelper::Save(lines.join('\n'), fn, false);
    zInfo(QStringLiteral("SaveMeta:")+(metaSaved?"ok":"failed"))
}

UploadHelper::UploadResponseModel UploadHelper::AddUpload(const MetaData &m)
{
    bool valid = m.isValid();
    int key=-1;
    int offset = -1;
    if(valid){
        QString udPath = FileNameHelper::UploadDir();

        zInfo("udPath:"+udPath)

        QDir ud(udPath);
        if(!ud.exists()){
            bool ok = ud.mkpath(udPath);
            if(!ok){
                zInfo("udPath: cannot make path")
            }
        }

        QString upd = FileNameHelper::UploadFileName(m.fileName);
        QFile f(upd);

        bool noMeta = _metaData.empty();
        if(noMeta){
            LoadMeta();
        }

        bool isFileExists = f.exists();
        int key = MetaData::Find(_metaData, m.fileName);
        bool isMetaExists = key>-1;

        //ha a fájl létezik
        if(isFileExists){
            if(isMetaExists){// és megvan a downloadok között, is,
                int metaSize = _metaData[key].fileSize;
                if(f.size()==metaSize){// és a mérete megfelelő, folytathatjuk
                    offset = metaSize;
                }else{
                    // nem jó a mérete, újrakezdjük
                    offset = 0;
                    f.resize(0);
                }
            } else{ // nincs a downloadok között, újrakezdjük
                offset = 0;
                f.resize(0);
            }
        }else{ // nincs meg a file, újrakezdjük
            offset = 0;
            f.open(QFile::OpenModeFlag::WriteOnly);
            if(isMetaExists){
                // eltűnt a fájl, újra kell kezdeni
                int metaSize = _metaData[key].fileSize;
                if(m.fileSize!=metaSize){
                    _metaData[key]=m;

                    SaveMeta();
                }
            } else{
                key = MetaData::NewKey(_metaData);
                _metaData.insert(key, m);
                SaveMeta();
            }
        }
    }

    UploadResponseModel m2{ .key=key, .offset=offset, .flag = 0};
    return m2;
}

QString UploadHelper::UploadModel::toString() const
{
    QString a = QString::number(key).toUtf8()+','+QString::number(offset)+","+data;
    return a;
}

QString UploadHelper::UploadResponseModel::toString() const
{
    QString a = QString::number(key).toUtf8()+','+QString::number(offset)+","+QString::number(flag);
    return a;
}


UploadHelper::UploadModel UploadHelper::UploadModel::Parse(const QString &v)
{
    bool valid = !v.isEmpty();
    UploadModel m;

    if(valid){
        int L = v.length();
        //zInfo("L:"+QString::number(L));
        int ix = v.indexOf(',');
        if(ix>0){
            //QString keyStr, offsetStr, dataStr;

            bool ok;
            QString keyStr = v.left(ix);
            int i = keyStr.toInt(&ok);
            if(ok) m.key = i;
            //zInfo("keyStr:"+QString::number(ix)+":"+keyStr);
            if(L>=ix+1){
                int ix2 = v.indexOf(',', ix+1);
                if(ix2>ix){
                    QString offsetStr = v.mid(ix+1, ix2-ix-1);
                    i = offsetStr.toInt(&ok);
                    if(ok) m.offset=i;
                    //zInfo("offsetStr:"+QString::number(ix2)+":"+offsetStr);
                    if(L>=ix2+1){
                        m.data = v.mid(ix2+1);
                        //zInfo("dataStr:"+dataStr)
                    }
                }
            }
        }
    }

    //zInfo(QStringLiteral("UploadModel:")+m.toString());
    return m;
}

UploadHelper::UploadResponseModel UploadHelper::Upload(const UploadModel &m, bool isBase64){
    //zInfo(QStringLiteral("Upload:")+m.toString());

    bool noMeta = _metaData.empty();
    if(noMeta){
        LoadMeta();
    }

    bool isMetaExists = _metaData.contains(m.key);
    UploadResponseModel retVal;

    if(isMetaExists){
        auto metaData = _metaData[m.key];

      //  zInfo("meta:"+metaData.toString());

        retVal.key = m.key;

        QString upd = FileNameHelper::UploadFileName(metaData.fileName);

        zInfo("upd:"+upd);

        QFile f(upd);
        bool isFileExists = f.exists();

        if(isFileExists)
        {
            if(m.offset==f.size())
            {
                zInfo("append...");
                bool ok = f.open(QFile::OpenModeFlag::WriteOnly |QFile::OpenModeFlag::Append );
                if(ok){
                    QByteArray data;
                    if(isBase64){
                        data = QByteArray::fromBase64(m.data.toUtf8());
                    } else {
                        data = m.data.toUtf8();
                    }
                    auto bytes = f.write(data);
                    if(bytes>0){
                        ok = f.flush();
                    } else{
                        ok = false;
                    }
                    f.close();
                    retVal.offset=f.size();

                    if(retVal.offset==metaData.fileSize){
                        zInfo("upload ok");
                        _metaData.remove(m.key);
                        retVal.flag=1;
                    } else if(retVal.offset>metaData.fileSize){
                        zInfo("upload failed");
                        _metaData.remove(m.key);
                        retVal.flag=2;
                    } else{
                        retVal.flag=0;
                    }
                }
                zInfo(QStringLiteral("append:")+(ok?"ok":"failed"));
            } else{
                zInfo("append failed");
            }
        } else{
            zInfo("file not exists");
        }
    } else{
        zInfo("file has no metadata");
    }

    return retVal;
}


