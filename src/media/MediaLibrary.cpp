#include "MediaLibrary.hpp"

#include <QtCore/QDebug>
#include <QtSql/QtSql>

#include <bb/PpsObject>

#include "MediaFile_p.hpp"

namespace
{
const char* REL_DEVICE_ROOT_PREFIX = "shared/";
const char* PERSONAL_ROOT = "/accounts/1000/shared";
const char* ENTERPRISE_ROOT = "/accounts/1000-enterprise/shared";
const char* SDCARD_ROOT = "/accounts/1000/removable/sdcard";
const char* DEVICE_DB = "db/mmlibrary.db";
const char* SDCARD_DB = "db/mmlibrary_SD.db";
}

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
    static FileType::Type fileTypeFromValue(int fileTypeValue);

    QSqlDatabase addMediaDatabase(FilePerimeter::Type perimeter);
    void removeMediaDatabase(FilePerimeter::Type perimeter);
    QString mediaDatabaseConnectionName(FilePerimeter::Type perimeter);

    void loadMediaFileData(const QString &filePath, MediaFile *mediaFile);
    void queryMediaFileData(QSqlDatabase mediaDb, MediaFile *mediaFile);

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

    removeMediaDatabase(FilePerimeter::Personal);
    removeMediaDatabase(FilePerimeter::Enterprise);
    removeMediaDatabase(FilePerimeter::SDCard);
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
    mediaFile.d->fileType = fileTypeFromValue(fileType);

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

FileType::Type MediaLibraryPrivate::fileTypeFromValue(int fileTypeValue)
{
    switch(fileTypeValue) {
    case 1:
        return FileType::Audio;
    case 2:
        return FileType::Video;
    case 4:
        return FileType::Photo;
    case 6:
        return FileType::Document;
    case 99:
        return FileType::Other;
    default:
        return FileType::Unknown;
    }
}

MediaFile MediaLibrary::findMediaFile(const QString &filePath)
{
    Q_D(MediaLibrary);
    MediaFile mediaFile;
    d->loadMediaFileData(filePath, &mediaFile);
    return mediaFile;
}

QSqlDatabase MediaLibraryPrivate::addMediaDatabase(FilePerimeter::Type perimeter)
{
    const QString connectionName = mediaDatabaseConnectionName(perimeter);
    if(connectionName.isNull()) { return QSqlDatabase(); }

    QSqlDatabase mediaDb = QSqlDatabase::database(connectionName, false);
    if(!mediaDb.isValid()) {
        QString dbFileName;
        switch(perimeter) {
        case FilePerimeter::Personal:
        case FilePerimeter::Enterprise:
            dbFileName = DEVICE_DB;
            break;
        case FilePerimeter::SDCard:
            dbFileName = SDCARD_DB;
            break;
        default:
            return QSqlDatabase();
        }

        mediaDb = QSqlDatabase::addDatabase("QSQLITE", connectionName);
        mediaDb.setDatabaseName(dbFileName);
        mediaDb.setConnectOptions("QSQLITE_OPEN_READONLY");
    }
    return mediaDb;
}

void MediaLibraryPrivate::removeMediaDatabase(FilePerimeter::Type perimeter)
{
    const QString connectionName = mediaDatabaseConnectionName(perimeter);
    if(connectionName.isNull()) { return; }
    QSqlDatabase::removeDatabase(connectionName);
}

QString MediaLibraryPrivate::mediaDatabaseConnectionName(FilePerimeter::Type perimeter)
{
    QString suffix;
    switch(perimeter) {
    case FilePerimeter::Personal:
        suffix = QLatin1String("per");
        break;
    case FilePerimeter::Enterprise:
        suffix = QLatin1String("ent");
        break;
    case FilePerimeter::SDCard:
        suffix = QLatin1String("sd");
        break;
    default:
        return QString::null;
    }

    return QString("mediaLibrary_%1_%2").arg(suffix).arg(reinterpret_cast<int>(QThread::currentThreadId()));
}

void MediaLibraryPrivate::loadMediaFileData(const QString &filePath, MediaFile *mediaFile)
{
    QString searchPath;
    if(filePath.startsWith(QLatin1String(REL_DEVICE_ROOT_PREFIX))) {
        searchPath = QLatin1String(PERSONAL_ROOT) + QChar('/') + filePath.mid(qstrlen(REL_DEVICE_ROOT_PREFIX));
    }
    else {
        searchPath = filePath;
    }

    if(searchPath.startsWith(QLatin1String(PERSONAL_ROOT) + QChar('/'))) {
        mediaFile->d->perimeter = FilePerimeter::Personal;
        mediaFile->d->path = searchPath.mid(qstrlen(PERSONAL_ROOT) + 1);
    }
    else if(searchPath.startsWith(QLatin1String(ENTERPRISE_ROOT) + QChar('/'))) {
        mediaFile->d->perimeter = FilePerimeter::Enterprise;
        mediaFile->d->path = searchPath.mid(qstrlen(ENTERPRISE_ROOT) + 1);
    }
    else if(searchPath.startsWith(QLatin1String(SDCARD_ROOT) + QChar('/'))) {
        mediaFile->d->perimeter = FilePerimeter::SDCard;
        mediaFile->d->path = searchPath.mid(qstrlen(SDCARD_ROOT) + 1);
    }
    else {
        qWarning() << "Unrecognized path:" << searchPath;
        return;
    }

    QSqlDatabase mediaDb = addMediaDatabase(mediaFile->perimeter());
    if(mediaDb.isValid() && mediaDb.open()) {
        queryMediaFileData(mediaDb, mediaFile);
        mediaDb.close();
    }
    else {
        qWarning() << mediaDb.lastError();
    }
}

void MediaLibraryPrivate::queryMediaFileData(QSqlDatabase mediaDb, MediaFile *mediaFile)
{
    const QString path = mediaFile->path();
    const int p = path.lastIndexOf(QChar('/'));
    const QString basePath = QLatin1Char('/') + path.left(p + 1);
    const QString simpleFileName = path.mid(p + 1);

    QSqlQuery query(mediaDb);
    query.setForwardOnly(true);

    query.prepare("SELECT files.fid, files.folderid, files.ftype "
        "FROM folders, files "
        "WHERE folders.basepath = :basepath "
        "AND files.filename = :filename "
        "AND folders.folderid = files.folderid");
    query.bindValue(":basepath", basePath);
    query.bindValue(":filename", simpleFileName);

    if(!query.exec()) {
        qWarning() << "Unable to query file ID:" << query.lastError();
        return;
    }

    if(query.next()) {
        mediaFile->d->fileId = query.value(0).toLongLong();
        mediaFile->d->folderId = query.value(1).toLongLong();
        mediaFile->d->fileType = fileTypeFromValue(query.value(2).toInt());
    }
    else {
        qWarning() << "Unable to find file ID:" << path;
    }
}

} // namespace multimedia
} // namespace bbext

#include "moc_MediaLibrary.cpp"
