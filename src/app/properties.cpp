#include <QDebug>

#include "properties.h"
#include "fileutils.h"
#include "settings.h"
#include "directorycontentsworker.h"

Properties::Properties()
{
    directoryContentsWorker = new DirectoryContentsWorker;
    connect(directoryContentsWorker, SIGNAL(currentData(quint64, quint64, quint64)), this, SLOT(currentDataFromDirectoryContents(quint64, quint64, quint64)  ));
    connect(directoryContentsWorker, SIGNAL(selectedItems(quint64, quint64)), this, SLOT(selectedItemsInfoCalculated(quint64, quint64)));

    clear();
}

QString  Properties::getNoMultimediaFileName() const
{
    return (getAbsoluteFilePath() + "/.nomedia");
}

bool Properties::hasNoMultimediaProperty() const
{
    if (!isDir()) {
        return false;
    }
    return QFile::exists(getNoMultimediaFileName());
}

bool Properties::setNoMultimediaProperty(bool enable) const
{
    if (!isDir()) {
        return false;
    }
    QString fileName = getNoMultimediaFileName();
    bool exists = QFile::exists(fileName);

    if (enable) {
        if (exists) {
            return true;
        } else {
            QFile file(fileName);
            if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate| QIODevice::Text)) {
                return false;
            }
            return QFile::exists(fileName);
        }
    } else {
        if (exists) {
            return QFile::remove(fileName);
        } else {
            return true;
        }
    }
    return false;
}

TagEditor* Properties::tagEditor() const
{
    return &tagEdit;
}

Properties::~Properties()
{
    directoryContentsWorker->terminate();
}

bool Properties::checkInList(const QStringList& list, const QString& suffix)
{
    for (int i = 0; i < list.size(); i++) {
        if ( suffix.compare(list.at(i), Qt::CaseInsensitive) == 0 ) {
            return true;
        }
    }
    return false;
}
void Properties::clear()
{
    propertiesType = UnknowType;
    fi.setFile("");
}

void Properties::setArray(const QStringList& array)
{
    if (array.size() == 1) {
        setPath(array.at(0));
        return;
    }

    clear();

    propertiesType = MultipleItems;
    directoryContentsWorker->setContents(array);
    directoryContentsWorker->start();
}

void Properties::setPath(const QString& fileNamePath)
{
    clear();
    fi.setFile(fileNamePath);
    if (!fi.exists()) {
        return;
    }

    if (fi.isFile()) {
        propertiesType = CommonFile;
        QString suffix = fi.suffix();
        QStringList inputExtensions;
        inputExtensions << "PNG" <<"JPG" << "GIF";
        if (checkInList(inputExtensions,suffix) ) {
            propertiesType = ImageFile;
            imageReader.setFileName(fileNamePath);
        }

        inputExtensions.clear();
        inputExtensions << "3GP" <<"3GP2" << "ASF" << "AVI" << "F4V" << "M4V" << "MKV" << "MOV" << "MP4" << "MPEG4" << "WMV";
        if (checkInList(inputExtensions,suffix) ) {
            propertiesType = VideoFile;
        }

        inputExtensions.clear();
        inputExtensions << "AAC" << "AMR" << "AWB"<<  "FLAC" << "M4A" << "MKA" << "MP3" << "WAV" << "WMA" << "OGG" << "ASF"; // ASF не в спецификации, но добавлен
        if (checkInList(inputExtensions,suffix) ) {
            propertiesType = AudioFile;
            if (tagEdit.setFileName(fileNamePath)) {
                propertiesType = AudioFileWithEditableTags;
            }
        }
        // MP4 supported by Taglib, but save is a dangerous?
        // disabled


    } else if (fi.isDir()) {
        propertiesType = Directory;
        QStringList sl;
        sl << fileNamePath;
        directoryContentsWorker->setContents(sl);
        directoryContentsWorker->start();
    }
}

void Properties::selectedItemsInfoCalculated(quint64 directoriesCount, quint64 filesCount)
{
    emit selectedItemsInfoChanged(directoriesCount, filesCount);
}

void Properties::currentDataFromDirectoryContents(quint64 totalSize, quint64 directoriesCount, quint64 filesCount)
{
    emit directoryContentInfoChanged(FileUtils::sizeToString(totalSize), directoriesCount, filesCount );
}

bool Properties::isFile() const
{
    return fi.isFile();
}

bool  Properties::hasInternalCoverSupport() const
{
    return tagEdit.hasCoverSupport();
}

QString Properties::internalImageCover() const
{
    return tagEdit.internalImageCover();
}

bool  Properties::setInternalImageCover(const QString& fileName)
{
    return tagEdit.setInternalImageCover(fileName);
}

bool  Properties::setDirectoryImageCover(const QString& fileName)
{
    QString currentCover = coverFileNamePath();
    if (QFile::exists(currentCover)) {
        QFile::remove(currentCover);
    }
    if (fileName.length() == 0) {
        return true;
    }

    QFileInfo fi(fileName);
    if (!fi.exists()) {
        return false;
    }
    return QFile::copy(fileName, currentCover );
}

QString Properties::coverFileNamePath() const
{
    if (!isFile()) {
        return QString();
    }
    return fi.absolutePath() + QString("/folder.jpg");
}

QString Properties::directoryImageCover() const
{
    if (!isFile()) {
        return QString();
    }

    QFileInfo coverFi(coverFileNamePath());
    if (coverFi.exists()) {
        return coverFi.absoluteFilePath();
    }
    return QString();
}

bool Properties::isDir() const
{
    return fi.isDir();
}

bool Properties::isImage() const
{
    return (propertiesType == ImageFile);
}

bool Properties::isVideoFile() const
{
    return (propertiesType == VideoFile);
}

bool Properties::isAudioFile() const
{
    return ((propertiesType == AudioFile) || (propertiesType == AudioFileWithEditableTags));
}

bool Properties::isAudioFileWithEditableTags() const
{
    return (propertiesType == AudioFileWithEditableTags);
}

bool Properties::isMultipleItems() const
{
    return (propertiesType == MultipleItems);
}

QString Properties::getFileName() const
{
    return fi.fileName();
}

QString Properties::getBaseName() const
{
    return fi.baseName();
}

QString Properties::getRightsDescription() const
{
    return fi.isWritable() ? tr("read/write") :  tr("read only");
}

QString Properties::getHumanReadableFileSizeBytes() const
{
    if (isFile()) {
        return QString("(%1 ").arg(fi.size()) + tr("b") + ")";
    }
    return QString();
}

QString Properties::getHumanReadableFileSize() const
{
    if (isFile()) {
        return FileUtils::sizeToString(fi.size());
    } else if (isDir()){
        return QString(tr("calculating..."));
    } else {
        //
    }
    return QString(tr("calculating..."));
}

QString Properties::getSize() const
{
    if (isFile()) {
        return QString::number(fi.size());
    } else if (isDir()){
        return QString();
    } else {
        //
    }
    return QString();
}

QString Properties::getCreated() const
{
    return Settings::instance().dateTimeI18n(fi.created());
}

QString Properties::getModified() const
{
    return Settings::instance().dateTimeI18n(fi.lastModified());
}

QString Properties::getAbsoluteFilePathWithPrefixFile() const
{
    return (QString("file://") +  getAbsoluteFilePath());
}

QString Properties::getAbsoluteFilePath() const
{
    return fi.absoluteFilePath();
}

QString Properties::getPath() const
{
    if (isMultipleItems()) {
        return directoryContentsWorker->getPath();
    } else {
        return fi.path();
    }
    return QString();
}

QString Properties::getHumanReadablePath() const
{
    return FileUtils::getHumanReadablePath( getPath() );
}

QString Properties::getType() const
{
    if (isFile()) {
        return fi.suffix().toUpper();
    } else if (isDir() ){
        return tr("Folder");
    } else {
        return tr("Files and folders");
    }
}

int Properties::imageWidth() const
{
    return imageReader.size().width();
}

int Properties::imageHeight() const
{
    return imageReader.size().height();
}
