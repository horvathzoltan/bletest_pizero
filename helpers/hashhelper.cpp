#include "filenamehelper.h"
#include "hashhelper.h"

#include <QCryptographicHash>
#include <QFile>

HashHelper::Model HashHelper::Model::Parse(const QString &v)
{
    bool valid = !v.isEmpty();
    Model m;

    if(valid){
        int ix = v.indexOf(',');
        if(ix>0){
            m.fileName=v.left(ix);
            if(v.length()>ix){
                m.hash0 = v.mid(ix+1).toUtf8();
            }
        }
    }
    return m;
}


bool HashHelper::Hash(const HashHelper::Model&m){
    bool retVal = false;

    QString upd = FileNameHelper::UploadFileName(m.fileName);

    QFile f(upd);
    bool isFileExists = f.exists();
    if(isFileExists){
        QCryptographicHash hashMaster( QCryptographicHash::Sha256 );
        if( f.open(QIODevice::ReadOnly ) )
        {
            if( hashMaster.addData( &f ) )
            {
                QByteArray hash1 = hashMaster.result().toHex();
                if(hash1==m.hash0){
                    retVal = true;
                }
            }
            f.close();
        }
    }
    return retVal;
}
