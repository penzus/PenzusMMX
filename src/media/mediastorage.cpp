#include "mediastorage.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/LocaleHandler>

#include "medialibrary.hpp"
#include "mediafile.hpp"

using namespace bb::cascades;

MediaStorage::MediaStorage(QObject *parent) :
        QObject(parent)
{
    mediaLibrary_ = new bbext::multimedia::MediaLibrary(this);
    connect(mediaLibrary_, SIGNAL(mediaAdded(bbext::multimedia::MediaFile)),
        this, SLOT(onMediaAdded(bbext::multimedia::MediaFile)));
    connect(mediaLibrary_, SIGNAL(mediaDeleted(bbext::multimedia::MediaFile)),
        this, SLOT(onMediaDeleted(bbext::multimedia::MediaFile)));
    connect(mediaLibrary_, SIGNAL(mediaUpdated(bbext::multimedia::MediaFile)),
        this, SLOT(onMediaUpdated(bbext::multimedia::MediaFile)));
    connect(mediaLibrary_, SIGNAL(mediaInvalidated(bbext::multimedia::MediaFile)),
        this, SLOT(onMediaInvalidated(bbext::multimedia::MediaFile)));
}

void MediaStorage::onMediaAdded(const bbext::multimedia::MediaFile &mediaFile)
{
    qDebug() << "--> onMediaAdded:" << mediaFile;
}

void MediaStorage::onMediaDeleted(const bbext::multimedia::MediaFile &mediaFile)
{
    qDebug() << "--> onMediaDeleted:" << mediaFile;
}

void MediaStorage::onMediaUpdated(const bbext::multimedia::MediaFile &mediaFile)
{
    qDebug() << "--> onMediaUpdated:" << mediaFile;
}

void MediaStorage::onMediaInvalidated(const bbext::multimedia::MediaFile &mediaFile)
{
    qDebug() << "--> onMediaInvalidated:" << mediaFile;
}

void MediaStorage::onTestQuery()
{
    qDebug() << "--> onTestQuery";
#if 0
    bbext::multimedia::MediaFile photoFile = mediaLibrary_->findMediaFile("shared/photos/test-image.jpg");
    QString photoThumbnail = mediaLibrary_->mediaThumbnail(photoFile);
    qDebug() << "Photo:" << photoFile;
    qDebug() << "  Thumbnail:" << photoThumbnail;

    bbext::multimedia::MediaFile videoFile = mediaLibrary_->findMediaFile("shared/videos/test-video.mp4");
    QString videoThumbnail = mediaLibrary_->mediaThumbnail(videoFile);
    QString videoArtwork = mediaLibrary_->mediaArtwork(videoFile);
    qDebug() << "Video:" << videoFile;
    qDebug() << "  Thumbnail:" << videoThumbnail;
    qDebug() << "  Artwork:" << videoArtwork;
#endif
    bbext::multimedia::MediaFile audioFile = mediaLibrary_->findMediaFile("shared/music/01.mp3");
    QString audioThumbnail = mediaLibrary_->mediaThumbnail(audioFile);
    QString audioArtwork = mediaLibrary_->mediaArtwork(audioFile);
    qDebug() << "Audio:" << audioFile;
    qDebug() << "  Thumbnail:" << audioThumbnail;
    qDebug() << "  Artwork:" << audioArtwork;
}
