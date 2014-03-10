#include "applicationui.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/LocaleHandler>

#include "MediaLibrary.hpp"
#include "MediaFile.hpp"

using namespace bb::cascades;

ApplicationUI::ApplicationUI(bb::cascades::Application *app) :
        QObject(app)
{
    translator_ = new QTranslator(this);
    localeHandler_ = new LocaleHandler(this);

    QObject::connect(localeHandler_, SIGNAL(systemLanguageChanged()), this, SLOT(onSystemLanguageChanged()));

    onSystemLanguageChanged();

    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

    AbstractPane *root = qml->createRootObject<AbstractPane>();
    connect(root, SIGNAL(testQuery()), this, SLOT(onTestQuery()));

    app->setScene(root);

    mediaLibrary_ = new bbext::multimedia::MediaLibrary(this);
    connect(mediaLibrary_, SIGNAL(mediaAdded(bbext::multimedia::MediaFile)),
        this, SLOT(onMediaAdded(bbext::multimedia::MediaFile)));
    connect(mediaLibrary_, SIGNAL(mediaDeleted(bbext::multimedia::MediaFile)),
        this, SLOT(onMediaDeleted(bbext::multimedia::MediaFile)));
}

void ApplicationUI::onSystemLanguageChanged()
{
    QCoreApplication::instance()->removeTranslator(translator_);
    QString locale_string = QLocale().name();
    QString file_name = QString("MediaServiceUtility_%1").arg(locale_string);
    if (translator_->load(file_name, "app/native/qm")) {
        QCoreApplication::instance()->installTranslator(translator_);
    }
}

void ApplicationUI::onMediaAdded(const bbext::multimedia::MediaFile &mediaFile)
{
    qDebug() << "--> onMediaAdded:" << mediaFile;
}

void ApplicationUI::onMediaDeleted(const bbext::multimedia::MediaFile &mediaFile)
{
    qDebug() << "--> onMediaDeleted:" << mediaFile;
}

void ApplicationUI::onTestQuery()
{
    qDebug() << "--> onTestQuery";

    bbext::multimedia::MediaFile photoFile = mediaLibrary_->findMediaFile("shared/photos/arrakeenday11440x1280.jpg");
    QString photoThumbnail = mediaLibrary_->mediaThumbnail(photoFile);
    qDebug() << "Photo:" << photoFile;
    qDebug() << "  Thumbnail:" << photoThumbnail;

    bbext::multimedia::MediaFile videoFile = mediaLibrary_->findMediaFile("shared/videos/Videos - Chromatic.mp4");
    QString videoThumbnail = mediaLibrary_->mediaThumbnail(videoFile);
    QString videoArtwork = mediaLibrary_->mediaArtwork(videoFile);
    qDebug() << "Video:" << videoFile;
    qDebug() << "  Thumbnail:" << videoThumbnail;
    qDebug() << "  Artwork:" << videoArtwork;

    bbext::multimedia::MediaFile audioFile = mediaLibrary_->findMediaFile("shared/music/Tetris.mp3");
    QString audioThumbnail = mediaLibrary_->mediaThumbnail(audioFile);
    QString audioArtwork = mediaLibrary_->mediaArtwork(audioFile);
    qDebug() << "Audio:" << audioFile;
    qDebug() << "  Thumbnail:" << audioThumbnail;
    qDebug() << "  Artwork:" << audioArtwork;
}
