#ifndef BBEXT_MULTIMEDIA_FILEPERIMETER_HPP
#define BBEXT_MULTIMEDIA_FILEPERIMETER_HPP

#include <QtCore/QObject>
#include <QtDeclarative/QtDeclarative>

namespace bbext
{
namespace multimedia
{

class FilePerimeter
{
    Q_GADGET
    Q_ENUMS(Type)
public:
    enum Type {
        Unknown = 0,
        Personal = 1,
        Enterprise = 2,
        SDCard = 3
    };
private:
    FilePerimeter();
    ~FilePerimeter();
};

QDebug operator<<(QDebug dbg, const FilePerimeter::Type &filePerimeter);

} // namespace multimedia
} // namespace bbext

QML_DECLARE_TYPE(bbext::multimedia::FilePerimeter)

#endif // BBEXT_MULTIMEDIA_FILEPERIMETER_HPP
