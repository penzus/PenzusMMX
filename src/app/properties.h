#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <QStringList>
#include <QObject>
#include <QFileInfo>
#include <QtGui/QImageReader>

#include "tageditor.h"

class DirectoryContentsWorker;

class Properties: public QObject
{
    Q_OBJECT
public:
    Properties();
    ~Properties();

    Q_INVOKABLE QString directoryImageCover() const;
    Q_INVOKABLE QString getAbsoluteFilePath() const;
    Q_INVOKABLE QString getAbsoluteFilePathWithPrefixFile() const;
    Q_INVOKABLE QString getBaseName() const;
    Q_INVOKABLE QString getCreated() const;
    Q_INVOKABLE QString getFileName() const;
    Q_INVOKABLE QString getHumanReadableFileSize() const;
    Q_INVOKABLE QString getHumanReadableFileSizeBytes() const;
    Q_INVOKABLE QString getHumanReadablePath() const;
    Q_INVOKABLE QString getModified() const;
    Q_INVOKABLE QString getRightsDescription() const;
    Q_INVOKABLE QString getSize() const;
    Q_INVOKABLE QString getType() const;
    Q_INVOKABLE QString internalImageCover() const;
    Q_INVOKABLE TagEditor* tagEditor() const;
    Q_INVOKABLE bool hasInternalCoverSupport() const;
    Q_INVOKABLE bool hasNoMultimediaProperty() const;
    Q_INVOKABLE bool isAudioFile() const;
    Q_INVOKABLE bool isAudioFileWithEditableTags() const;
    Q_INVOKABLE bool isDir() const;
    Q_INVOKABLE bool isFile() const;
    Q_INVOKABLE bool isImage() const;
    Q_INVOKABLE bool isMultipleItems() const;
    Q_INVOKABLE bool isVideoFile() const;
    Q_INVOKABLE bool setDirectoryImageCover(const QString& fileName);
    Q_INVOKABLE bool setInternalImageCover(const QString& fileName);
    Q_INVOKABLE bool setNoMultimediaProperty(bool enable) const;
    Q_INVOKABLE int imageHeight() const;
    Q_INVOKABLE int imageWidth() const;
    Q_INVOKABLE void setArray(const QStringList& array);
    Q_INVOKABLE void setPath(const QString& fileNamePath);

signals:
    void directoryContentInfoChanged(QString totalSize, quint64 directoriesCount, quint64 filesCount);
    void selectedItemsInfoChanged(quint64 directoriesCount, quint64 filesCount);

protected slots:
    void currentDataFromDirectoryContents(quint64 totalSize, quint64 directoriesCount, quint64 filesCount);
    void selectedItemsInfoCalculated(quint64 directoriesCount, quint64 filesCount);

protected:
    enum Type {UnknowType, CommonFile, AudioFile, AudioFileWithEditableTags, VideoFile, ImageFile, Directory, MultipleItems } propertiesType;

    QString coverFileNamePath() const;
    QString getNoMultimediaFileName() const;
    QString getPath() const;
    mutable QImageReader imageReader;
    mutable TagEditor tagEdit;
    static bool checkInList(const QStringList& list, const QString& suffix);
    void clear();

    DirectoryContentsWorker* directoryContentsWorker;
    QFileInfo fi;
};

#endif // PROPERTIES_H
