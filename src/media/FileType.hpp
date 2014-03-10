#ifndef BBEXT_MULTIMEDIA_FILETYPE_HPP
#define BBEXT_MULTIMEDIA_FILETYPE_HPP

#include <QtCore/QObject>
#include <QtDeclarative/QtDeclarative>

namespace bbext
{
namespace multimedia
{

class FileType
{
    Q_GADGET
    Q_ENUMS(Type)
public:
    enum Type {
        Unknown = 0,
        Audio = 1,
        Video = 2,
        Photo = 4,
        Document = 6,
        Other = 99
    };
private:
    FileType();
    ~FileType();
};

QDebug operator<<(QDebug dbg, const FileType::Type &fileType);

} // namespace multimedia
} // namespace bbext

QML_DECLARE_TYPE(bbext::multimedia::FileType)

#endif // BBEXT_MULTIMEDIA_FILETYPE_HPP
