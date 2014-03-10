#include "MediaFile.hpp"
#include "MediaFile_p.hpp"

namespace bbext
{
namespace multimedia
{

MediaFileData::MediaFileData()
    : fileId(-1), folderId(-1), fileType(FileType::Unknown),
      perimeter(FilePerimeter::Unknown)
{
}

MediaFile::MediaFile()
{
    d = new MediaFileData();
}

MediaFile::MediaFile(const MediaFile& other)
    : d(other.d)
{
}

MediaFile::~MediaFile()
{
}

MediaFile& MediaFile::operator=(const MediaFile& other)
{
    d = other.d;
    return *this;
}

bool MediaFile::isNull() const
{
    return d->fileId == -1 && d->folderId == -1;
}

QString MediaFile::path() const
{
    return d->path;
}

qlonglong MediaFile::fileId() const
{
    return d->fileId;
}

qlonglong MediaFile::folderId() const
{
    return d->folderId;
}

FileType::Type MediaFile::fileType() const
{
    return d->fileType;
}

FilePerimeter::Type MediaFile::perimeter() const
{
    return d->perimeter;
}
QDebug operator<<(QDebug dbg, const MediaFile &mediaFile)
{
    if(mediaFile.isNull()) {
        dbg.nospace() << "MediaFile()";
    }
    else {
        dbg.nospace() << "MediaFile("
            << mediaFile.path() << ", "
            << mediaFile.fileId() << ", "
            << mediaFile.folderId() << ", "
            << mediaFile.fileType() << ", "
            << mediaFile.perimeter() << ")";
    }
    return dbg.maybeSpace();
}

} // namespace multimedia
} // namespace bbext
