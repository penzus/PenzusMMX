#ifndef TAGEDITOR_H
#define TAGEDITOR_H

#include <QFileInfo>
#include <QString>

#include "taginfo.h"

class TagEditor : public QObject
{
    Q_OBJECT
public:
    TagEditor();

    Q_INVOKABLE TagInfo* tagInfo() const {
        return &ti;
    }

    Q_INVOKABLE QString getBitrate() const;
    Q_INVOKABLE QString getChannels() const;
    Q_INVOKABLE QString getLength() const;
    Q_INVOKABLE QString getSampleRate() const;
    Q_INVOKABLE QString internalImageCover() const;
    Q_INVOKABLE bool hasCoverSupport() const;
    Q_INVOKABLE bool isValid() const;
    Q_INVOKABLE bool save();
    Q_INVOKABLE bool setFileName (const QString& fileName);
    Q_INVOKABLE bool setInternalImageCover(const QString& imageFileName);

protected:
    void clear();

    QFileInfo fi;
    QString bitrate;
    QString channels;
    QString getInternalImageCoverPrivate() const;
    QString length;
    QString sampleRate;
    bool setInternalImageCoverPrivate(const QString& imageFileName);
    bool validState;
    mutable TagInfo ti;
};

#endif // TAGEDITOR_H
