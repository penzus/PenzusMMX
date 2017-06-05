#ifndef APPLICATION_H
#define APPLICATION_H

#include <bb/cascades/Invocation>
#include <bb/cascades/InvokeQuery>

#include <bb/system/CardDoneMessage.hpp>
#include <bb/system/InvokeManager>
#include <bb/system/InvokeRequest>
#include <QObject>
#include <QDateTime>


namespace bb {
namespace cascades {
class Application;
class AbstractPane;
class LocaleHandler;

}
}

class ApplicationUI: public QObject
{
    Q_OBJECT
public:
    ApplicationUI(bb::cascades::Application *app);
    virtual ~ApplicationUI() {}

    static Q_INVOKABLE QString contentNumber();
    static Q_INVOKABLE QString officialAppDescription();
    static Q_INVOKABLE QString officialAppDescriptionUpper();
    static Q_INVOKABLE QString officialAppName();
    static Q_INVOKABLE QString officialBBMChannelLink(bool html = true);
    static Q_INVOKABLE QString officialEMailLink(bool html = true);
    static Q_INVOKABLE QString officialUrlLink(bool hmtl = true);
    static Q_INVOKABLE QString vendorNumber();
    static Q_INVOKABLE QString versionNumber();
    static bb::cascades::AbstractPane *rootObject();
    static void prepareApplication();

    Q_INVOKABLE QString mimeType() const;
    Q_INVOKABLE QString uri() const;
    Q_INVOKABLE QStringList queryReplyResults() const;
    Q_INVOKABLE bool createShortCut(const QString& path, const QString& imageSource);
    Q_INVOKABLE bool invokedAsApplication() const;
    Q_INVOKABLE bool invokedAsCard() const;
    Q_INVOKABLE bool specialThanksEnabled() const;
    Q_INVOKABLE int currentCascadesThemeIndex() const;
    Q_INVOKABLE void closeCard();
    Q_INVOKABLE void invokeApp(const QString& fileName, int index);
    Q_INVOKABLE void invokeCard(const QString& fileName);
    Q_INVOKABLE void openEditPictureFile(const QString& fileName);
    Q_INVOKABLE void processQueryReply();
    Q_INVOKABLE void queryTargets(const QString& fileName);
    Q_INVOKABLE void setCurrentCascadesThemeIndex(int index) const;
    Q_INVOKABLE void shareFile(const QString& fileNamePath);
    Q_INVOKABLE void shareFiles(const QStringList& fileNamePaths);
    Q_INVOKABLE void viewMediaFile(const QString& fileName);

signals:
    void externalInvokeError(QString message, QString lastFileNameForInvoke);
    void fileEditingDone(const QString& filePath);
    void handleInvokeFinished();
    void invokedDirectory1(QString path);
    void processQueryReplyFinished(QString lastFileNameForInvoke);

public slots:
    void cardDone(const QString& msg);

private slots:
    void childCardDone(const bb::system::CardDoneMessage& doneMessage);
    void handleInvoke(const bb::system::InvokeRequest&);
    void onArmed();
    void onSystemLanguageChanged();
    void pooled(const bb::system::CardDoneMessage&);
    void processInvokeReply();
    void resized(const bb::system::CardResizeMessage&);

protected:
    QString getMimeType(const QString& fileName);
    void initCardUI();
    void initFullUI();

    QString lastFileNameForInvoke;
    QString m_action;
    QString m_data;
    QString m_mimeType;
    QString m_source;
    QString m_target;
    QString m_uri;
    QTranslator* translator;
    QVector<QVariantMap> replyResults;
    bb::cascades::Invocation* m_pInvocation;
    bb::cascades::LocaleHandler* localeHandler;
    bb::system::InvokeManager *invokeManager;
    bool invAsCard;
    bool specialThanks;
    static QString cascadesTheme;
    static bb::cascades::AbstractPane *root;
};

#endif // APPLICATION_H
