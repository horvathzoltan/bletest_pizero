#include "filenamehelper.h"
#include "helpers/logger.h"
#include "helpers/textfilehelper.h"
#include "uploadhelper.h"

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

bool UploadHelper::MetaData::isValid() const
{
    bool valid = fileSize>0 && !fileName.isEmpty();

    return valid;
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
                    QString valueStr = line.right(line.length()-ix);

                } else continue;
            }
        }
    }
}

int UploadHelper::AddUpload(const MetaData &m)
{
    bool valid = m.isValid();
    int key=-1;
    if(valid){
        QString upd = FileNameHelper::UploadFileName(m.fileName);
        QFile f(upd);

        bool noMeta = _metaData.empty();
        if(noMeta){
            LoadMeta();
        }

        bool exists = f.exists();
        //ha a fájl megvan
        if(exists){
            // ha megvan a downloadok között, is, folytatjuk
            // ha nincs
            if(f.size()==0){
                zInfo("new file:"+m.fileName);
            }
        }else{
            bool ok = f.open(QFile::OpenModeFlag::NewOnly);
            if(ok){

            }
        }
    }
    return key;
}
