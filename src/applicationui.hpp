#ifndef APPLICATIONUI_HPP
#define APPLICATIONUI_HPP

#include <QtCore/QObject>

#include "MediaFile.hpp"

namespace bb { namespace cascades {
class Application;
class LocaleHandler;
}}

namespace bbext { namespace multimedia {
class MediaLibrary;
}}

class QTranslator;

class ApplicationUI : public QObject
{
    Q_OBJECT
public:
    ApplicationUI(bb::cascades::Application *app);
    virtual ~ApplicationUI() { }
private slots:
    void onSystemLanguageChanged();
    void onMediaAdded(const bbext::multimedia::MediaFile &mediaFile);
    void onMediaDeleted(const bbext::multimedia::MediaFile &mediaFile);
    void onMediaUpdated(const bbext::multimedia::MediaFile &mediaFile);
    void onMediaInvalidated(const bbext::multimedia::MediaFile &mediaFile);
    void onTestQuery();
private:
    QTranslator* translator_;
    bb::cascades::LocaleHandler* localeHandler_;
    bbext::multimedia::MediaLibrary *mediaLibrary_;
};

#endif // APPLICATIONUI_HPP
