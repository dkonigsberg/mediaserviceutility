#include "applicationui.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/LocaleHandler>

#include "MediaLibrary.hpp"

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

    app->setScene(root);

    bbext::multimedia::MediaLibrary *mediaLibrary = new bbext::multimedia::MediaLibrary(this);
    connect(mediaLibrary, SIGNAL(mediaAdded(bbext::multimedia::MediaFile)),
        this, SLOT(onMediaAdded(bbext::multimedia::MediaFile)));
    connect(mediaLibrary, SIGNAL(mediaDeleted(bbext::multimedia::MediaFile)),
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
