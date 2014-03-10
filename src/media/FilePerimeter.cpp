#include "FilePerimeter.hpp"

namespace bbext
{
namespace multimedia
{

FilePerimeter::FilePerimeter()
{
}

FilePerimeter::~FilePerimeter()
{
}

QDebug operator<<(QDebug dbg, const FilePerimeter::Type &filePerimeter)
{
    switch(filePerimeter) {
    case FilePerimeter::Unknown:
        dbg << "FilePerimeter::Unknown";
        break;
    case FilePerimeter::Personal:
        dbg << "FilePerimeter::Personal";
        break;
    case FilePerimeter::Enterprise:
        dbg << "FilePerimeter::Enterprise";
        break;
    case FilePerimeter::SDCard:
        dbg << "FilePerimeter::SDCard";
        break;
    default:
        dbg << "FilePerimeter::Type(" << int(filePerimeter) << ')';
        break;
    }
    return dbg.maybeSpace();
}

} // namespace multimedia
} // namespace bbext
