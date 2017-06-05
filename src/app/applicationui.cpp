#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/TextArea>
#include <bb/cascades/AbstractPane>
#include <bb/ApplicationInfo>
#include <bb/system/SystemDialog>
#include <bb/system/InvokeTargetReply>
#include <bb/system/InvokeQueryTargetsReply>
#include <bb/system/InvokeQueryTargetsRequest>
#include <bb/cascades/pickers/FilePicker>
#include <bb/cascades/pickers/FilePickerMode>
#include <bb/cascades/pickers/FilePickerSortFlag>
#include <bb/cascades/pickers/FilePickerSortOrder>
#include <bb/cascades/pickers/FilePickerViewMode>
#include <bb/cascades/pickers/FileType>
#include <bb/cascades/SceneCover>
#include <bb/cascades/LocaleHandler>
#include <bb/cascades/MultiCover>
#include <bb/platform/HomeScreen>
#include <bb/data/JsonDataAccess>
#include <bb/PpsObject>


#include <iostream>
#include <QTimer>
#include <QLocale>
#include <QTranslator>

#include "mediastorage.hpp"
#include "mainlistviewmodel.h"
#include "favoriteslistviewmodel.h"
#include "settings.h"
#include "tageditor.h"
#include "applicationui.h"
#include "common.h"
#include "filesysteminfo.h"
#include "properties.h"
#include "texteditor.h"
#include "fileutils.h"

using namespace bb::cascades;
using namespace bb::system;
using namespace bb::data;
using namespace bb::system;

bb::cascades::AbstractPane* ApplicationUI::root;

AbstractPane* ApplicationUI::rootObject()
{
    return root;
}

ApplicationUI::ApplicationUI(bb::cascades::Application *app) :	QObject(app)
{
    specialThanks = true;
    translator = new QTranslator(this);
    localeHandler = new LocaleHandler(this);
    QObject::connect(localeHandler, SIGNAL(systemLanguageChanged()), this, SLOT(onSystemLanguageChanged()));

    invAsCard = false;
    invokeManager = new InvokeManager(this);
    connect(invokeManager, SIGNAL(invoked(const bb::system::InvokeRequest&)), this, SLOT(handleInvoke(const bb::system::InvokeRequest&)));
    connect(invokeManager, SIGNAL(cardResizeRequested(const bb::system::CardResizeMessage&)), this, SLOT(resized(const bb::system::CardResizeMessage&)));
    connect(invokeManager, SIGNAL(cardPooled(const bb::system::CardDoneMessage&)), this,  SLOT(pooled(const bb::system::CardDoneMessage&)));
    connect(invokeManager,  SIGNAL(childCardDone(const bb::system::CardDoneMessage&)),  this, SLOT(childCardDone (const bb::system::CardDoneMessage&)));

    qmlRegisterType<QTimer>("CustomTimer", 1, 0, "QTimer");
    qmlRegisterType<TagInfo>("TagInfo", 1, 0, "TagInfo");
    qmlRegisterType<TagEditor>("TagEditor", 1, 0, "TagEditor");
    qmlRegisterType<FileUtils>("FileUtils", 1, 0, "FileUtils");

    qmlRegisterUncreatableType < pickers::FilePickerMode
            > ("bb.cascades.pickers", 1, 0, "FilePickerMode", "");
    qmlRegisterUncreatableType < pickers::FilePickerSortFlag
            > ("bb.cascades.pickers", 1, 0, "FilePickerSortFlag", "");
    qmlRegisterUncreatableType < pickers::FilePickerSortOrder
            > ("bb.cascades.pickers", 1, 0, "FilePickerSortOrder", "");
    qmlRegisterUncreatableType < pickers::FileType
            > ("bb.cascades.pickers", 1, 0, "FileType", "");
    qmlRegisterUncreatableType < pickers::FilePickerViewMode
            > ("bb.cascades.pickers", 1, 0, "FilePickerViewMode", "");
    qmlRegisterType < pickers::FilePicker
            > ("bb.cascades.pickers", 1, 0, "FilePicker");

    // The SceneCover is registered so that it can be used in QML, we register it to the bb.cascades namespace
    // since this is really something that belongs in cascades.
    qmlRegisterType < SceneCover > ("bb.cascades", 1, 0, "SceneCover");

    // Since it is not possible to create an instance of the AbstractCover it is registered as an uncreatable type,
    // we need to register it since it is used when setting the application cover in QML (Application.cover).

    switch (invokeManager->startupMode()) {
    case ApplicationStartupMode::LaunchApplication:
        initFullUI();
        return;
    case ApplicationStartupMode::InvokeApplication:
        initFullUI();
        return;
    case ApplicationStartupMode::InvokeCard:
        initCardUI();
        return;
    default:
        ;
    }
    initFullUI();
}


void ApplicationUI::initFullUI()
{
    QmlDocument *qml = QmlDocument::create("asset:///main.qml");
    qml->setContextProperty("myApp", this);

    qml->setContextProperty("settings", &(Settings::instance()));
    Settings::instance().read();
    onSystemLanguageChanged();

    qml->setContextProperty("textEditor", new ::TextEditor() );
    qml->setContextProperty("properties", new Properties());
    qml->setContextProperty("fsInfo", &(FileSystemInfo::instance())  );
    qml->setContextProperty("fileUtils", new FileUtils());
    qml->setContextProperty("mainListViewModel", new MainListViewModel(this));
    qml->setContextProperty("historyListViewModel", new FavoritesListViewModel("history", this));
    qml->setContextProperty("bookmarksListViewModel", new FavoritesListViewModel("bookmarks", this));

    root = qml->createRootObject<AbstractPane>();
    Application::instance()->setScene(root);
}

void ApplicationUI::initCardUI()
{
    invAsCard = true;

    QmlDocument *qml = QmlDocument::create("asset:///PropertiesPage.qml");
    qml->setContextProperty("myApp", this);
    qml->setContextProperty("settings", &(Settings::instance()));
    Settings::instance().read();
    onSystemLanguageChanged();

    Properties* properties = new Properties();
    qml->setContextProperty("properties", properties);

    root = qml->createRootObject<AbstractPane>();
    Application::instance()->setScene(root);
}

QString ApplicationUI::versionNumber() {
    bb::ApplicationInfo info;
    return info.version();
}

QString ApplicationUI::officialAppDescription() {
    return APP_NAME_DESCRIPTION;
}

QString ApplicationUI::officialAppDescriptionUpper()
{
    return APP_NAME_DESCRIPTION_UPPER;
}

QString ApplicationUI::officialAppName() {
    return OFFICIAL_APP_NAME;
}

QString ApplicationUI::officialBBMChannelLink(bool html)
{
    if (html) {
        return QString("<html><a href=\"bbmc:%1\">BBM Channel</a></html>").arg(BBMCHANNEL);
    }
    return QString("bbmc:%1").arg(BBMCHANNEL);
}

QString ApplicationUI::officialEMailLink(bool html) {
    if (html) {
        return QString("<html><a href=\"mailto:%1\">%1</a></html>").arg(
                    SUPPORT_EMAIL);
    }
    return QString("mailto:%1?subject=%2").arg(SUPPORT_EMAIL).arg(
                OFFICIAL_APP_NAME);
}

QString ApplicationUI::officialUrlLink(bool html) {
    return html ?
                QString("<html><a href='http://%1'>%1</a></html>").arg(URL_LINK) :
                (QString("http://%1").arg(URL_LINK));
}

QString ApplicationUI::contentNumber() {
    return OFFICIAL_APP_CONTENT;
}

QString ApplicationUI::vendorNumber() {
    return VENDOR_NUMBER;
}

QString ApplicationUI::cascadesTheme;

void ApplicationUI::prepareApplication()
{
    QSettings settings(ORGANIZATION, SHORT_APP_NAME);
    settings.beginGroup("MainSettings");
    cascadesTheme = settings.value("cascadesTheme", "dark").toString();
    settings.endGroup();
    setenv("CASCADES_THEME", cascadesTheme.toStdString().c_str(), 1);
}

int ApplicationUI::currentCascadesThemeIndex() const
{
    return cascadesTheme == "dark" ? 1 : 0;
}

void ApplicationUI::setCurrentCascadesThemeIndex(int index) const
{
    cascadesTheme = (index == 1) ? "dark" : "bright";
    QSettings settings(ORGANIZATION, SHORT_APP_NAME);
    settings.beginGroup("MainSettings");
    settings.setValue("cascadesTheme",  cascadesTheme);
    settings.endGroup();
}

void ApplicationUI::cardDone(const QString& msg)
{
    CardDoneMessage message;
    message.setData(msg);
    message.setDataType("text/plain");
    message.setReason(tr("Success!"));
    invokeManager->sendCardDone(message);
}


void ApplicationUI::resized(const bb::system::CardResizeMessage&)
{
    //
}

void ApplicationUI::pooled(const bb::system::CardDoneMessage& /*doneMessage*/)
{
    //
}


void ApplicationUI::handleInvoke(const InvokeRequest& request)
{
    // Copy data from incoming invocation request to properties
    m_source = QString::fromLatin1("%1 (%2)").arg(request.source().installId()).arg(request.source().groupId());
    m_target = request.target();
    m_action = request.action();
    m_mimeType = request.mimeType();
    m_uri = request.uri().toString();
    m_data = QString::fromUtf8(request.data());
    m_uri = m_uri.remove("file://");

    if (m_target == "com.penzus.mmx.directory1") {
        QString path = m_uri.replace("penzusmmx://","");
        emit invokedDirectory1(path);
    } else {
        emit handleInvokeFinished();
    }
}

void ApplicationUI::closeCard()
{
    CardDoneMessage message;
    message.setData(OFFICIAL_APP_NAME);
    message.setDataType("text/plain");
    message.setReason(tr("Success!"));
    invokeManager->sendCardDone(message);
}

bool ApplicationUI::invokedAsApplication() const
{
    return (invokeManager->startupMode() == ApplicationStartupMode::InvokeApplication) ? true : false;
}

bool ApplicationUI::invokedAsCard() const
{
    return invAsCard;
}

QString ApplicationUI::mimeType() const
{
    return m_mimeType;
}

QString ApplicationUI::uri() const
{
    return m_uri;
}

bool checkInList(const QStringList& list, const QString& suffix) {
    for (int i = 0; i < list.size(); i++) {
        if ( suffix.compare(list.at(i), Qt::CaseInsensitive) == 0 ) {
            return true;
        }
    }
    return false;
}

void  ApplicationUI::shareFiles(const QStringList& files)
{
    if (files.size() == 1) {
        shareFile(files.at(0));
        return;
    }

    QString list;
    for (int i = 0; i < files.size(); i++) {
        if (list.isEmpty()) {
            list = list+ "[{\"uri\": \"file://" + files.at(i) + "\"}";
        } else {
            list = list+ ",{\"uri\": \"file://" + files.at(i) + "\"}";
        }
    }
    list += "]";

    m_pInvocation = Invocation::create(InvokeQuery::create().parent(this).mimeType("filelist/mixed").uri("list://").data(list.toUtf8()));
    QObject::connect(m_pInvocation, SIGNAL(armed()), this, SLOT(onArmed()));
    QObject::connect(m_pInvocation, SIGNAL(finished()), m_pInvocation, SLOT(deleteLater()));
}

void ApplicationUI::shareFile(const QString& fileName)
{
    m_pInvocation = Invocation::create(
                InvokeQuery::create().parent(this).mimeType(FileUtils::getMimeType(fileName)).invokeActionId("bb.action.SHARE").uri(QUrl::fromLocalFile(fileName))).parent(this);

    QObject::connect(m_pInvocation, SIGNAL(armed()), this, SLOT(onArmed()));
    QObject::connect(m_pInvocation, SIGNAL(finished()), m_pInvocation, SLOT(deleteLater()));
}

void ApplicationUI::onArmed() {
    m_pInvocation->trigger("bb.action.SHARE");
}

bool  ApplicationUI::createShortCut(const QString& path, const QString& imageSource)
{
    bb::platform::HomeScreen homeScreen;
    QFileInfo fi(path);
    if (!fi.exists()) {
        return false;
    }
    QUrl url;
    if (fi.isFile()) {
        url = QUrl("file://" + path);
    } else if (fi.isDir()) {
        url = QUrl("penzusmmx://" + path);
    } else {
        return false;
    }

    return homeScreen.addShortcut(QUrl(imageSource), fi.fileName(), url);
}

void ApplicationUI::processInvokeReply()
{
    // Get the reply from the sender object
    InvokeReply *reply = qobject_cast<InvokeReply*>(sender());

    // Check for errors during invocation
    switch (reply->error()) {
    case InvokeReplyError::BadRequest:
        emit  externalInvokeError("Bad request",lastFileNameForInvoke);
        break;
    case InvokeReplyError::Internal:
        emit  externalInvokeError("Iternal error",lastFileNameForInvoke);
        break;
    case InvokeReplyError::NoTarget:
        emit  externalInvokeError("No target app",lastFileNameForInvoke);
        break;
    case InvokeReplyError::TargetNotOwned:
        emit  externalInvokeError("Target not owned",lastFileNameForInvoke);
        break;
    default:
        break;
    }

    // Delete the reply later on
    reply->deleteLater();
}

void ApplicationUI::invokeApp(const QString& fileName, int index )
{
    if ((index < 0) || (index > replyResults.size())) {
        return;
    }

    bb::system::InvokeManager manager;
    bb::system::InvokeRequest request;

    request.setUri(QUrl::fromLocalFile(fileName));
    request.setMimeType(FileUtils::getMimeType(fileName));
    request.setAction("bb.action.OPEN");
    request.setTarget(replyResults[index].value("name").toString());
    InvokeTargetReply *targetReply = manager.invoke(request);

    // Setting the parent to "this" will make the manager live on after this function is destroyed
    manager.setParent(this);
    if (targetReply == NULL) {
        //
    } else {
        bool ok = connect(targetReply, SIGNAL(finished()), this,    SLOT(processInvokeReply()));
        Q_ASSERT(ok);
        Q_UNUSED(ok);
        targetReply->setParent(this);
    }
}

void  ApplicationUI::invokeCard(const QString& fileName)
{
    lastFileNameForInvoke = fileName;
    bb::system::InvokeManager manager;
    bb::system::InvokeRequest request;

    request.setUri(QUrl::fromLocalFile(fileName));

    request.setMimeType(FileUtils::getMimeType(fileName));
    InvokeTargetReply *targetReply = manager.invoke(request);

    // Setting the parent to "this" will make the manager live on after this function is destroyed
    manager.setParent(this);
    if (targetReply == NULL) {
        //
    } else {
        bool ok = connect(targetReply, SIGNAL(finished()), this,    SLOT(processInvokeReply()));
        Q_ASSERT(ok);
        Q_UNUSED(ok);
        targetReply->setParent(this);
    }
}
//////////////////////////////// ~~~INVOKE FOR EXTERNAL APP FOR GIVE FILENAME ////////////////////


void ApplicationUI::viewMediaFile(const QString& fileName)
{
    bb::system::InvokeManager manager;
    bb::system::InvokeRequest request;

    request.setUri(QUrl::fromLocalFile(fileName));

    if (FileUtils::getMimeType(fileName) == "image/gif") {
        request.setTarget("sys.pictures.card.previewer");
    } else {
        request.setTarget("sys.mediaplayer.previewer");
    }

    request.setMimeType(FileUtils::getMimeType(fileName));
    InvokeTargetReply *targetReply = manager.invoke(request);

    // Setting the parent to "this" will make the manager live on after this function is destroyed
    manager.setParent(this);

    if (targetReply == NULL) {
        //
    } else {
        targetReply->setParent(this);
    }
}

void ApplicationUI::onSystemLanguageChanged()
{
    QLocale systemLocale;
    QString locale_string;
    if (systemLocale.language() == QLocale::English) {
        locale_string = "en_US";
    }  else if (systemLocale.language() == QLocale::Arabic) {
        locale_string = "ar_EG";
    }  else if (systemLocale.language() == QLocale::Czech) {
        locale_string = "cs_CZ";
    }  else if (systemLocale.language() == QLocale::German) {
        locale_string = "de_DE";
    } else if (systemLocale.language() == QLocale::Spanish) {
        locale_string = "es_ES";
    }  else if (systemLocale.language() == QLocale::French) {
        locale_string = "fr_FR";
    }  else if (systemLocale.language() == QLocale::Hindi) {
        locale_string = "hi_IN";
    }  else if (systemLocale.language() == QLocale::Indonesian) {
        locale_string = "id_ID";
    }  else if (systemLocale.language() == QLocale::Japanese) {
        locale_string = "ja_JP";
    }  else if (systemLocale.language() == QLocale::Portuguese) {
        locale_string = "pt_PT";
    }  else if (systemLocale.language() == QLocale::Russian) {
        locale_string = "ru_RU";
    }  else if (systemLocale.language() == QLocale::Vietnamese) {
        locale_string = "vi_VN";
    }  else if (systemLocale.language() == QLocale::Chinese) {
        locale_string = "zh_CN";
    } else {
        locale_string = "";
    }
    if (!Settings::instance().getLanguage().isEmpty() ) {
        locale_string = Settings::instance().getLanguage();
    }

    if (  locale_string == "ja_JP") {
        specialThanks = false;
    }

    QString filename = QString("%1_%2").arg(SHORT_APP_NAME).arg(locale_string);
    if (translator->load(filename, "app/native/qm")) {
        QCoreApplication::instance()->installTranslator(translator);
    }
    QCoreApplication::instance()->installTranslator(translator);

}

bool ApplicationUI::specialThanksEnabled() const
{
    return specialThanks;
}

QStringList ApplicationUI::queryReplyResults() const
{
    QStringList list;
    for (int i = 0; i < replyResults.size(); i++) {
        list.append(replyResults[i].value("label").toString());
    }
    return list;
}

void ApplicationUI::queryTargets(const QString& fileName)
{
    lastFileNameForInvoke = fileName;

    QString q_mimeType = FileUtils::getMimeType(fileName);
    QString q_action = "bb.action.OPEN";
    QString q_uri = "file://" + fileName;

    int q_targetType = InvokeTarget::Application | InvokeTarget::Viewer;

    // Create a new query targets request
    InvokeQueryTargetsRequest request;

    // Setup the request properties according to the current configuration
    if (q_targetType == 0)
        request.setTargetTypes(
                    InvokeTarget::Application | InvokeTarget::Card
                    | InvokeTarget::Viewer | InvokeTarget::Service);
    else if (q_targetType == 1)
        request.setTargetTypes(InvokeTarget::Application);
    else if (q_targetType == 2)
        request.setTargetTypes(InvokeTarget::Viewer);
    else if (q_targetType == 3)
        request.setTargetTypes(InvokeTarget::Service);
    else if (q_targetType == 4)
        request.setTargetTypes(InvokeTarget::Card);

    if (!q_action.isEmpty()) {
        if (q_action == QLatin1String("__All"))
            request.setActionType(InvokeAction::All);
        else if (q_action == QLatin1String("__MenuActions"))
            request.setActionType(InvokeAction::Menu);
        else
            request.setAction(q_action);
    }

    if (!q_mimeType.isEmpty())
        request.setMimeType(q_mimeType);

    if (!q_uri.isEmpty())
        request.setUri(q_uri);

    // Start the query
    const InvokeReply *reply = invokeManager->queryTargets(request);

    // Ensure that processQueryReply() is called when the query has finished
    bool ok = connect(reply, SIGNAL(finished()), this, SLOT(processQueryReply()));
    Q_ASSERT(ok);
    Q_UNUSED(ok);
}

void ApplicationUI::processQueryReply()
{
    replyResults.clear();

    // Get the reply from the sender object
    InvokeQueryTargetsReply *reply = qobject_cast<InvokeQueryTargetsReply*>(
                sender());

    if (reply->error() == InvokeReplyError::None) { // If no error occurred ...

        // Iterate over the reported actions and targets
        foreach (const InvokeAction &action, reply->actions()){
            foreach (const InvokeTarget &target, action.targets()) {
                // Add one entry to the model for each target

                if (target.name() == QString("cis.bug.VK10")) { // Fucking app!
                    continue;
                }
                if (target.name().contains("Penzus MMX")) { // my app
                    continue;
                }

                QVariantMap entry;
                entry["label"] = target.label();
                entry["name"] = target.name();
                entry["imageSource"] = target.icon();

                replyResults.append(entry);
            }
        }

        // Signal that the query was successful
        emit processQueryReplyFinished(lastFileNameForInvoke);
    }

    // Check for errors during invocation
    switch (reply->error()) {
    case InvokeReplyError::BadRequest:
        emit  externalInvokeError("Bad request", lastFileNameForInvoke);
        break;
    case InvokeReplyError::Internal:
        emit  externalInvokeError("Iternal error", lastFileNameForInvoke);
        break;
    case InvokeReplyError::NoTarget:
        emit  externalInvokeError("No target app", lastFileNameForInvoke);
        break;
    case InvokeReplyError::TargetNotOwned:
        emit  externalInvokeError("Target not owned",lastFileNameForInvoke);
        break;
    default:
        break;
    }

    // Delete the reply later on
    reply->deleteLater();
}

void ApplicationUI::childCardDone(const bb::system::CardDoneMessage& doneMessage)
{
    if ( (doneMessage.reason() == "SaveDone") || (doneMessage.reason() == "Saved")) {
        emit fileEditingDone(doneMessage.data());
    }
}

void  ApplicationUI::openEditPictureFile(const QString& fileNamePath)
{
    QFileInfo fi(fileNamePath);

    QString tmpFileName = Settings::instance().getDefaultOutputDir() + QString("/.tmp-image.") + fi.completeSuffix();
    if (QFile::exists(tmpFileName)) {
        QFile::remove(tmpFileName);
    }

    if (!QFile::copy(fileNamePath, tmpFileName)) {
        return;
    }
    QString fileName = tmpFileName;
    bb::system::InvokeRequest request;
    QUrl invokeUri = QUrl::fromLocalFile(fileName);

    request.setTarget("sys.pictureeditor.cardeditor");
    request.setAction("bb.action.EDIT");
    request.setUri(invokeUri);
    request.setMimeType(FileUtils::getMimeType(fileName));

    InvokeTargetReply *targetReply = invokeManager->invoke(request);
    invokeManager->setParent(this);

    if (targetReply == NULL) {
        //
    } else {
        targetReply->setParent(this);
    };
}
