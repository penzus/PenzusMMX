#include <QFileInfo>
#include <QDir>
#include <QDirIterator>
#include <QDebug>
#include "fileutils.h"

QString FileUtils::cardString;
QString FileUtils::dropBoxString;
QString FileUtils::boxString;
QString FileUtils::oneDriveString;
QString FileUtils::deviceString;
QStringList FileUtils::fileOperationPaths;

bb::FileSystemInfo FileUtils::fileSystemInfo;

QHash<QString, QString> FileUtils::mimeHash;
QHash<QString, QString> FileUtils::mimeImagesHash;


bool FileUtils::addOtherFile(const QString& newPath)
{
    QFileInfo fi(newPath);
    if ((fi.suffix() == "doc") || (fi.suffix() == "xls")  || (fi.suffix() == "ppt")  || (fi.suffix() == "txt")) {
        QFile file(QString(":/assets/data/template.%1").arg(fi.suffix()));
        if (!file.open(QIODevice::ReadOnly )) {
            qDebug() << "Cannot open template file"<< file.fileName() << "!";
            return false;
        }
        QByteArray templateData = file.readAll();
        if (!QFile::exists(newPath)) {
            QFile targetFile(newPath);
            if (targetFile.open(QIODevice::WriteOnly)) {
                qint64 written = targetFile.write(templateData);
                targetFile.flush();
                if (written != templateData.size()) {
                    QFile::remove(newPath);
                    return false;
                } else {
                    return true;
                }
            }
        } else {
            return false;
        }

    } else {
        QFile targetFile(newPath);
        if (targetFile.open(QIODevice::WriteOnly)) {
            return true;
        }
    }

    return false;
}

bool FileUtils::addFile(const QString& path, int index)
{
    QString extension = "txt";
    switch (index) {
    case 0: extension = "doc"; break;
    case 1: extension = "xls"; break;
    case 2: extension = "ppt"; break;
    case 3: extension = "txt"; break;
    default:
        break;
    }

    QString newPath;
    QFile file(QString(":/assets/data/template.%1").arg(extension));
    if (!file.open(QIODevice::ReadOnly )) {
        return false;
    }

    QByteArray templateData = file.readAll();
    for  (int i = 1; i < 100; i++) {
        QString counter = (i == 1) ? "" : (" " + QString::number(i));
        newPath = mergePathWithName(path, tr("New file") + counter + "." + extension);
        if (!QFile::exists(newPath)) {
            QFile targetFile(newPath);
            if (targetFile.open(QIODevice::WriteOnly)) {
                qint64 written = targetFile.write(templateData);
                targetFile.flush();
                if (written != templateData.size()) {
                    QFile::remove(newPath);
                    return false;
                } else {
                    return true;
                }
            }
        }
    }
    return false;
}

void FileUtils::initializeHash()
{
    QFile file(":/assets/data/filetypes.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        QString lineString = QString(line.data());
        lineString = lineString.simplified();
        QStringList sl = lineString.split("=",QString::SkipEmptyParts);
        if (sl.size() == 2) {
            mimeHash.insert(sl.at(0),sl.at(1));
        }
    }

    QString imagesDir = "app/native/assets/images/mimetypes/system";
    QDirIterator it(imagesDir, QDirIterator::NoIteratorFlags);
    while (it.hasNext()) {
        QFileInfo fi(it.next());
        if (fi.suffix() == "png") {
            mimeImagesHash.insert( fi.baseName(), QString("asset:///images/mimetypes/system/%1").arg(fi.fileName()));
        }
    }
}

FileUtils::~FileUtils()
{

}

bool FileUtils::pathIsMountedLocalFileSystem(const QString& path)
{
    // check cloud file systems
    if (path.startsWith(oneDriveString)) return false;
    if (path.startsWith(boxString)) return false;
    if (path.startsWith(dropBoxString)) return false;

    if (path.startsWith(cardString)) return true;
    if (path.startsWith(deviceString)) return true;

    // i don't know this file system
    return false;
}

FileUtils::FileUtils(QObject *parent) : QObject(parent)
{
    deviceString  =   "/accounts/1000/shared";
    cardString  =     "/accounts/1000/removable/sdcard";
    dropBoxString =   "/accounts/1000/shared/Dropbox";
    boxString     =   "/accounts/1000/shared/Box";
    oneDriveString =  "/accounts/1000/shared/OneDrive";

    initializeHash();

    connect(&fileOperationThread, SIGNAL( operationStateChanged(int, QString) ), this, SLOT( operationStateChangedSlot(int, QString)  ));
    connect(&fileOperationThread, SIGNAL( operationThreadFinished(int) ), this, SLOT( operationThreadFinishedSlot(int)  ));
    connect(&fileOperationThread, SIGNAL( operationError(QString) ), this, SLOT( operationErrorSlot(QString)  ));
}

bool FileUtils::isTopLevelDeviceDir(const QString& path)
{
    return (path == deviceString) ? true : false;
}

bool FileUtils::isTopLevelDir(const QString& path)
{
    if ((path == deviceString)) return true;
    if ((path == cardString)) return true;
    if ((path == dropBoxString)) return true;
    if ((path == boxString)) return true;
    if ((path == oneDriveString)) return true;
    return false;
}

QString FileUtils::getPathByIndex(int index)
{
    switch (index) {
    case 0: return deviceString;
    case 1: return cardString;
    case 2: return dropBoxString;
    case 3: return boxString;
    case 4: return oneDriveString;
    default:
        break;
    }
    return deviceString;
}

QString FileUtils::getHumanReadablePath(const QString& inPath)
{
    QString path = inPath;

    if (path.startsWith(cardString)) {
        path = path.replace(0,cardString.size(), tr("SD card"));

    } else if (path.startsWith(dropBoxString)) {
        path = path.replace(0,dropBoxString.size(),"DropBox");

    } else if (path.startsWith(boxString)) {
        path = path.replace(0,boxString.size(),"Box");

    } else if (path.startsWith(oneDriveString)) {
        path = path.replace(0,oneDriveString.size(),"OneDrive");

    } else if (path.startsWith(deviceString)) {
        path = path.replace(0,deviceString.size(),tr("device"));
    } else {
        qDebug() << " === I don't know this path: " << path;
    }

    return path;
}

bool FileUtils::isDir(const QString& path)
{
    QFileInfo fi(path);
    return fi.isDir();
}

QString FileUtils::completeBaseName(const QString& fileName)
{
    QFileInfo fi(fileName);
    return fi.completeBaseName();
}

QString FileUtils::fileName(const QString& fileName)
{
    QFileInfo fi(fileName);
    return fi.fileName();
}

QString FileUtils::absolutePath(const QString& inputFileNamePath)
{
    QFileInfo fi(inputFileNamePath);
    QString s = fi.absolutePath();
    return s;
}

bool FileUtils::fileNameInAsset(const QString& fileName)
{
    return fileName.startsWith("asset:///images");
}

QString FileUtils::mergePathWithName(const QString& path, const QString& name)
{
    return  path + "/" + name;
}

bool FileUtils::directoryOrFileExists(const QString& fileNamePath)
{
    QFileInfo fi(fileNamePath);
    return fi.exists();
}

bool FileUtils::mkDir(const QString& fileNamePath)
{
    QDir dir(fileNamePath);
    return dir.mkdir(fileNamePath);
}

QStringList FileUtils::deleteFileItems(const QStringList& list)
{
    QStringList failItems;
    for (int i = 0; i < list.size(); i++) {
        if (!deleteFileItem(list.at(i))) {
            failItems.append(list.at(i));
        }
    }
    return failItems;
}

bool FileUtils::deleteFileItem(const QString& fileNamePath)
{
    QFileInfo fi(fileNamePath);
    if (fi.isFile()) {
        return QFile::remove(fileNamePath);
    }
    if (fi.isDir()) {
        return removeDir(fileNamePath);
    }
    return false;
}

bool FileUtils::removeDir(const QString &dirName)
{
    bool result = true;
    QDir dir(dirName);

    if (dir.exists(dirName)) {
        Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
            if (info.isDir()) {
                result = removeDir(info.absoluteFilePath());
            }
            else {
                result = QFile::remove(info.absoluteFilePath());
            }

            if (!result) {
                return result;
            }
        }
        result = dir.rmdir(dirName);
    }
    return result;
}

QString FileUtils::renameFileItem(const QString& path, const QString& newBaseName, bool allowEditExtension)
{
    QFileInfo fi(path);
    if (!fi.exists()) {
        return QString();
    }
    QString newFileNamePath = fi.absolutePath() + "/" + newBaseName;
    if (fi.isDir()) {
        QDir dir(path);
        return dir.rename(path,newFileNamePath) ? newFileNamePath : QString();
    } else if (fi.isFile()) {
        QFile f(path);
        if (!allowEditExtension) {
            newFileNamePath = fi.absolutePath() + "/" + newBaseName + "." + fi.suffix();
        }
        return f.rename(newFileNamePath) ? newFileNamePath : QString();
    }
    return QString();
}

QString FileUtils::getMimeType(const QString& fileName)
{
    QFileInfo fi(fileName);
    QString mimeType = mimeHash.value(fi.suffix());
    if (!mimeType.isEmpty()) {
        return mimeType;
    }
    return "application/octet-stream";
}

QUrl FileUtils::pathToUrl(const QString& fileName)
{
    return  QUrl::fromLocalFile(fileName);
}

bool FileUtils::initializeToStartOperation(const QStringList& list, int mode )
{
    if (fileOperationThread.isRunning()) {
        return false;
    }

    fileOperationPaths =  list;
    fileOperationMode = mode;
    return true;
}

void FileUtils::startOperation( const QString& targetDirectory)
{
    fileOperationThread.prepare(fileOperationPaths, targetDirectory, fileOperationMode);
    fileOperationThread.start();
}

void FileUtils::operationStateChangedSlot(int progressPercent, QString messageString)
{
    emit operationStateChanged(progressPercent, messageString);
}

void FileUtils::operationErrorSlot(QString message)
{
    emit operationError(message);
}

void FileUtils::operationThreadFinishedSlot(int status)
{
    emit operationThreadFinished(status);
}

void FileUtils::cancelOperation()
{
    fileOperationThread.cancelOperation();
}

bool FileUtils::directoryIsWritable(const QString& path)
{
    QFileInfo fi(path);
    if(fi.isDir() && fi.isWritable()){
        return true;
    }
    return false;
}

quint64 FileUtils::availableFileSystemSpace(const QString& path)
{
    return fileSystemInfo.availableFileSystemSpace(path);
}

bool FileUtils::directoryInTheSameDevice( const QString& outputPath)
{
    QString foundString;
    if (outputPath.startsWith(cardString)) foundString = cardString;
    if (outputPath.startsWith(dropBoxString)) foundString = dropBoxString;
    if (outputPath.startsWith(boxString))    foundString = boxString;
    if (outputPath.startsWith(oneDriveString)) foundString = oneDriveString;
    if (outputPath.startsWith(deviceString)) foundString = deviceString;

    QString inputPath;
    if (fileOperationPaths.size() > 0) {
        inputPath = fileOperationPaths.at(0);
    }
    if (foundString.length() == 0) {
        return false;
    }

    return inputPath.startsWith(foundString);
}

bool FileUtils::directoryIsNested( const QString& outputPath)
{
    for (int i = 0; i < fileOperationPaths.size(); i++) {
        QString inputPath = fileOperationPaths.at(i);
        QFileInfo fiInput(inputPath);
        if (fiInput.absolutePath() == outputPath) {
            return true;
        }

        if ( (outputPath.length() >= inputPath.length()) && ( outputPath.startsWith(inputPath)) ) {
            return true;
        }
    }
    return false;
}

QString FileUtils::fileOperationDescriptionByType(int operation)
{
    switch (operation) {
    case 1: return tr("Copying");
    case 2: return tr("Moving");
    default:
        ;
    }
    return tr("Unknow operation");
}

quint64 FileUtils::fileInfoSize(const QString& path, QFileInfoList& outList, bool includeEmptyDirs )
{
    outList.clear();
    QFileInfo fiParent(path);
    if (!fiParent.exists()) {
        return 0;
    }

    if (fiParent.isFile()) {
        outList.append(fiParent);
        return fiParent.size();
    } else if (fiParent.isDir()) {
        quint64 size = 0;
        QDir::Filters flags = QDir::Files | QDir::NoDotAndDotDot;
        if (includeEmptyDirs) {
            flags |= QDir::Dirs;
            outList.append(fiParent);
        }

        QDirIterator it(path, flags, QDirIterator::Subdirectories);
        while (it.hasNext()) {
            QFileInfo fi(it.next());
            size += fi.size();
            outList.append(fi);
        }
        return size;
    } else {
        //
    }

    return 0;
}

QString FileUtils::sizeToString(quint64 size, bool text)
{
    if ( 1024 > size ) {
        return QString("%1").arg(size) + (text ? QObject::tr("B") : "");
    }
    if ((((quint64)1024 * 1024) > size) ) { //KB
        return QString("%1").arg((double)size/1024, 0, 'f', 1) + (text ? QObject::tr("KB") : "");
    }
    if ( ((quint64)1024 * 1024 * 1024) > size) { //MB
        double data = (double)size / ((quint64)1024 * 1024);
        return QString("%1").arg(data, 0, 'f', 1) + (text ? QObject::tr("MB") : "");
    } else {
        double data = (double)size / ((quint64)1024 * 1024 * 1024);
        return QString("%1").arg(data, 0, 'f', 1) + (text ? QObject::tr("GB") : "");
    }
}
