#include "FileType.hpp"

namespace bbext
{
namespace multimedia
{

FileType::FileType()
{
}

FileType::~FileType()
{
}

QDebug operator<<(QDebug dbg, const FileType::Type &fileType)
{
    switch(fileType) {
    case FileType::Unknown:
        dbg << "FileType::Unknown";
        break;
    case FileType::Audio:
        dbg << "FileType::Audio";
        break;
    case FileType::Video:
        dbg << "FileType::Video";
        break;
    case FileType::AudioVideo:
        dbg << "FileType::AudioVideo";
        break;
    case FileType::Photo:
        dbg << "FileType::Photo";
        break;
    case FileType::Device:
        dbg << "FileType::Device";
        break;
    case FileType::Document:
        dbg << "FileType::Document";
        break;
    case FileType::Other:
        dbg << "FileType::Other";
        break;
    default:
        dbg << "FileType::Type(" << int(fileType) << ')';
        break;
    }
    return dbg.maybeSpace();
}

} // namespace multimedia
} // namespace bbext
