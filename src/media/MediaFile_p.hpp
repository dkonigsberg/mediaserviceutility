#ifndef BBEXT_MULTIMEDIA_MEDIAFILE_P_HPP
#define BBEXT_MULTIMEDIA_MEDIAFILE_P_HPP

#include <QtCore/QSharedData>

#include "FileType.hpp"
#include "FilePerimeter.hpp"

namespace bbext
{
namespace multimedia
{

class MediaFileData : public QSharedData
{
public:
    MediaFileData();
    QString path;
    qlonglong fileId;
    qlonglong folderId;
    FileType::Type fileType;
    FilePerimeter::Type perimeter;
};

} // namespace multimedia
} // namespace bbext

#endif // BBEXT_MULTIMEDIA_MEDIAFILE_P_HPP
