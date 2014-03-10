#ifndef BBEXT_MULTIMEDIA_MEDIALIBRARY_HPP
#define BBEXT_MULTIMEDIA_MEDIALIBRARY_HPP

#include <QtCore/QObject>

#include "MediaFile.hpp"

namespace bbext
{
namespace multimedia
{

class MediaLibraryPrivate;

class MediaLibrary : public QObject
{
    Q_OBJECT
public:
    MediaLibrary(QObject *parent=0);
    virtual ~MediaLibrary();

    MediaFile findMediaFile(const QString &filePath);

    QString mediaThumbnail(const MediaFile &mediaFile);

Q_SIGNALS:
    void mediaAdded(const bbext::multimedia::MediaFile &mediaFile);
    void mediaDeleted(const bbext::multimedia::MediaFile &mediaFile);

private:
    Q_DISABLE_COPY(MediaLibrary)
    Q_DECLARE_PRIVATE(MediaLibrary)
    Q_PRIVATE_SLOT(d_func(), void _q_syncReadyRead());
    QScopedPointer<MediaLibraryPrivate> d_ptr;
};

} // namespace multimedia
} // namespace bbext

#endif // BBEXT_MULTIMEDIA_MEDIALIBRARY_HPP
