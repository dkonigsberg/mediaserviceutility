#include "MediaLibrary.hpp"

#include <QtCore/QDebug>

#include <bb/PpsObject>

#include "MediaFile_p.hpp"

namespace bbext
{
namespace multimedia
{

class MediaLibraryPrivate
{
public:
    MediaLibraryPrivate();
    ~MediaLibraryPrivate();

    void setSyncMonitoringEnabled(bool enabled);

    void _q_syncReadyRead();
    static MediaFile createFileFromChangeData(const QVariantMap &changeData);

    bb::PpsObject *syncPpsObject_;
    MediaLibrary *q_ptr;
    Q_DECLARE_PUBLIC(MediaLibrary);
};

MediaLibrary::MediaLibrary(QObject *parent) : QObject(parent), d_ptr(new MediaLibraryPrivate())
{
    qDebug() << "MediaLibrary::MediaLibrary()";
    Q_D(MediaLibrary);
    d->q_ptr = this;
    d->setSyncMonitoringEnabled(true);
}

MediaLibrary::~MediaLibrary()
{
    qDebug() << "MediaLibrary::~MediaLibrary()";
}

MediaLibraryPrivate::MediaLibraryPrivate() : q_ptr(0)
{
    qDebug() << "MediaLibraryPrivate::MediaLibraryPrivate()";
    syncPpsObject_ = new bb::PpsObject("/pps/services/multimedia/sync/changes?wait,delta");
}

MediaLibraryPrivate::~MediaLibraryPrivate()
{
    qDebug() << "MediaLibraryPrivate::~MediaLibraryPrivate()";
    delete syncPpsObject_;
}

void MediaLibraryPrivate::setSyncMonitoringEnabled(bool enabled)
{
    Q_Q(MediaLibrary);
    if(enabled) {
        if(!syncPpsObject_->isOpen()) {
            if(syncPpsObject_->open(bb::PpsOpenMode::Subscribe)) {
                q->connect(syncPpsObject_, SIGNAL(readyRead()), SLOT(_q_syncReadyRead()));
                _q_syncReadyRead();
            }
            else {
                qWarning() << "Could not connect to sync PPS object:" << syncPpsObject_->error() << syncPpsObject_->errorString();
            }
        }
    }
    else {
        if(syncPpsObject_->isOpen()) {
            QObject::disconnect(syncPpsObject_, SIGNAL(readyRead()), q, SLOT(_q_syncReadyRead()));
            syncPpsObject_->close();
        }
    }
}

void MediaLibraryPrivate::_q_syncReadyRead()
{
    Q_Q(MediaLibrary);
    bool readOk;
    QByteArray data = syncPpsObject_->read(&readOk);
    if(!readOk) { return; }

    bool decodeOk;
    const QVariantMap map = bb::PpsObject::decode(data, &decodeOk);
    if(!decodeOk) { return; }

    const QVariantMap changeData = map["@changes"].toMap();
    if(changeData.isEmpty()) { return; }

    MediaFile mediaFile = createFileFromChangeData(changeData);

    QString eventType = changeData["change"].toString();
    if(eventType == QLatin1String("added")) {
        q->emit mediaAdded(mediaFile);
    }
    else if(eventType == QLatin1String("del")) {
        q->emit mediaDeleted(mediaFile);
    }
    else {
        qWarning() << "Unknown sync event type:" << eventType;
    }
}

MediaFile MediaLibraryPrivate::createFileFromChangeData(const QVariantMap &changeData)
{
    MediaFile mediaFile;

    mediaFile.d->fileId = changeData["fid"].toLongLong();
    mediaFile.d->folderId = changeData["folderid"].toLongLong();

    int fileType = changeData["ftype"].toInt();
    switch(fileType) {
    case 1:
        mediaFile.d->fileType = FileType::Audio;
        break;
    case 2:
        mediaFile.d->fileType = FileType::Video;
        break;
    case 4:
        mediaFile.d->fileType = FileType::Photo;
        break;
    case 6:
        mediaFile.d->fileType = FileType::Document;
        break;
    case 99:
        mediaFile.d->fileType = FileType::Other;
        break;
    default:
        mediaFile.d->fileType = FileType::Unknown;
        break;
    }

    mediaFile.d->path = changeData["path"].toString();

    QString perimeterString = changeData["perimeter"].toString();
    if(perimeterString == QLatin1String("personal")) {
        mediaFile.d->perimeter = FilePerimeter::Personal;
    }
    else if(perimeterString == QLatin1String("enterprise")) {
        mediaFile.d->perimeter = FilePerimeter::Enterprise;
    }
    else if(perimeterString == QLatin1String("sdcard")) {
        mediaFile.d->perimeter = FilePerimeter::SDCard;
    }
    else {
        mediaFile.d->perimeter = FilePerimeter::Unknown;
    }

    return mediaFile;
}

} // namespace multimedia
} // namespace bbext

#include "moc_MediaLibrary.cpp"
