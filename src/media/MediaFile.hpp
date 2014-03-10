#ifndef BBEXT_MULTIMEDIA_MEDIAFILE_HPP
#define BBEXT_MULTIMEDIA_MEDIAFILE_HPP

#include <QtCore/QObject>
#include <QtCore/QMetaType>
#include <QtCore/QSharedDataPointer>
#include <QtCore/QDebug>

#include "FileType.hpp"
#include "FilePerimeter.hpp"

namespace bbext
{
namespace multimedia
{

class MediaLibraryPrivate;
class MediaSyncServicePrivate;
class MediaFileData;

class MediaFile
{
public:
    MediaFile();
    MediaFile(const MediaFile& other);
    ~MediaFile();
    MediaFile& operator=(const MediaFile& other);

    bool isNull() const;

    QString path() const;
    qlonglong fileId() const;
    qlonglong folderId() const;
    FileType::Type fileType() const;
    FilePerimeter::Type perimeter() const;

private:
    QSharedDataPointer<MediaFileData> d;
    friend class MediaLibraryPrivate;
    friend class MediaSyncServicePrivate;
};

QDebug operator<<(QDebug dbg, const MediaFile &mediaFile);

} // namespace multimedia
} // namespace bbext

Q_DECLARE_METATYPE(bbext::multimedia::MediaFile)
Q_DECLARE_TYPEINFO(bbext::multimedia::MediaFile, Q_MOVABLE_TYPE);

#endif // BBEXT_MULTIMEDIA_MEDIAFILE_HPP
