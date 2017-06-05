#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <QHash>
#include <QUrl>
#include <QString>
#include <QFileInfo>
#include <QStringList>
#include <QObject>

#include <bb/core/FileSystemInfo.hpp>
#include "fileoperationthread.h"

class FileUtils : public QObject
{
    Q_OBJECT
public:
    FileUtils(QObject *parent = 0);
    ~FileUtils();

    static inline bool mimeTypeImageFromFileNamePath(const QString& fileNamePath, QString& outputFileName) {
        QFileInfo fi(fileNamePath);
        QString suffix = fi.suffix().toLower();

        outputFileName = mimeImagesHash.value(suffix);
        if (!outputFileName.isEmpty()) {
            return true;
        }

        outputFileName = QString("asset:///images/mimetypes/system/unknown.png");
        return false;
    }

    Q_INVOKABLE bool directoryIsNested( const QString& outputPath);
    Q_INVOKABLE bool initializeToStartOperation(const QStringList& list, int operation );
    Q_INVOKABLE static QString absolutePath(const QString& inputFileNamePath);
    Q_INVOKABLE static QString completeBaseName(const QString& fileName);
    Q_INVOKABLE static QString fileName(const QString& fileName);
    Q_INVOKABLE static QString fileOperationDescriptionByType(int operation);
    Q_INVOKABLE static QString getHumanReadablePath(const QString& path);
    Q_INVOKABLE static QString getPathByIndex(int index);
    Q_INVOKABLE static QString mergePathWithName(const QString& path, const QString& name);
    Q_INVOKABLE static QString renameFileItem(const QString& path, const QString& newBaseName, bool allowEditExtension);
    Q_INVOKABLE static QString sizeToString(quint64 size, bool text = true);
    Q_INVOKABLE static QStringList deleteFileItems(const QStringList& list);
    Q_INVOKABLE static QUrl pathToUrl(const QString& fileName);
    Q_INVOKABLE static bool addFile(const QString& path, int index);
    Q_INVOKABLE static bool addOtherFile(const QString& fileName);
    Q_INVOKABLE static bool deleteFileItem(const QString& path);
    Q_INVOKABLE static bool directoryInTheSameDevice( const QString& outputPath);
    Q_INVOKABLE static bool directoryIsWritable(const QString& path);
    Q_INVOKABLE static bool directoryOrFileExists(const QString& path);
    Q_INVOKABLE static bool fileNameInAsset(const QString& fileName);
    Q_INVOKABLE static bool isDir(const QString& path);
    Q_INVOKABLE static bool isTopLevelDeviceDir(const QString& path);
    Q_INVOKABLE static bool isTopLevelDir(const QString& path);
    Q_INVOKABLE static bool mkDir(const QString& path);
    Q_INVOKABLE static quint64 availableFileSystemSpace(const QString& path);
    Q_INVOKABLE void cancelOperation();
    Q_INVOKABLE void startOperation( const QString& targetDirectory);

    static QString getMimeType(const QString& fileName);
    static bool pathIsMountedLocalFileSystem(const QString& path);
    static quint64 fileInfoSize(const QString& path, QFileInfoList& outList, bool includeEmptyDirs = false);

signals:
    void operationError(QString message);
    void operationStateChanged(int progressPercent, QString message);
    void operationThreadFinished(int status);

protected slots:
    void operationErrorSlot(QString message);
    void operationStateChangedSlot(int progressPercent, QString message);
    void operationThreadFinishedSlot(int status);

protected:
    void initializeHash();
    static bool removeDir(const QString &dirName);

    FileOperationThread fileOperationThread;
    int fileOperationMode;

    static QHash<QString, QString> mimeHash;
    static QHash<QString, QString> mimeImagesHash;
    static QString boxString;
    static QString cardString;
    static QString deviceString;
    static QString dropBoxString;
    static QString oneDriveString;
    static QStringList fileOperationPaths;
    static bb::FileSystemInfo fileSystemInfo;
};

#endif // FILEUTILS_H
