#include <QDebug>
#include <QSettings>
#include "taginfo.h"
#include "common.h"

TagInfo::TagInfo()
{
    clear();
}

void TagInfo::read()
{
    QSettings settings(ORGANIZATION, SHORT_APP_NAME);
    settings.beginGroup("TagInfo");

    useFileName = settings.value("useFileName", useFileName).toBool();
    useTitle = settings.value("useTitle", useTitle).toBool();
    useArtist = settings.value("useArtist", useArtist).toBool();
    useAlbum = settings.value("useAlbum", useAlbum).toBool();
    useComment = settings.value("useComment", useComment).toBool();
    useGenre = settings.value("useGenre", useGenre).toBool();
    useYear = settings.value("useYear", useYear).toBool();
    useTrack = settings.value("useTrack", useTrack).toBool();

    fileName = settings.value("fileName", fileName).toString();
    title = settings.value("title", title).toString();
    artist = settings.value("artist", artist).toString();
    album = settings.value("album", album).toString();
    comment = settings.value("comment", comment).toString();
    genre = settings.value("genre", genre).toString();

    year = settings.value("year", year).toInt();
    track = settings.value("track", track).toInt();

    settings.endGroup();
}

void TagInfo::save()
{
    QSettings settings(ORGANIZATION, SHORT_APP_NAME);
    settings.beginGroup("TagInfo");

    settings.setValue("useFileName",useFileName);
    settings.setValue("useTitle",useTitle);
    settings.setValue("useArtist",useArtist);
    settings.setValue("useAlbum",useAlbum);
    settings.setValue("useComment",useComment);
    settings.setValue("useGenre",useGenre);
    settings.setValue("useYear",useYear);
    settings.setValue("useTrack",useTrack);
    settings.setValue("fileName",fileName);
    settings.setValue("title",title);
    settings.setValue("artist",artist);
    settings.setValue("album",album);
    settings.setValue("comment",comment);
    settings.setValue("genre",genre);
    settings.setValue("year",year);
    settings.setValue("track",track);

    settings.endGroup();
}

void TagInfo::clear()
{
    useFileName = false;
    useTitle  = false;
    useArtist  = false;
    useAlbum  = false;
    useComment  = false;
    useGenre  = false;
    useYear  = false;
    useTrack  = false;

    fileName.clear();
    title.clear();
    artist.clear();
    album.clear();
    comment.clear();
    genre.clear();
    year = 0;
    track = 0;
}

QString TagInfo::getFileName() const
{
    return fileName;
}
void TagInfo::setFileName(const QString& newFileName)
{
    fileName = newFileName;
}

void TagInfo::setTitle(const QString& newTitle)
{
    title = newTitle;
}

void TagInfo::setArtist(const QString& newArtist)
{
    artist = newArtist;
}

void TagInfo::setAlbum(const QString& newAlbum)
{
    album = newAlbum;
}

void TagInfo::setGenre(const QString& newGenre)
{
    genre = newGenre;
}

void TagInfo::setComment(const QString& newComment)
{
    comment = newComment;
}

void TagInfo::setYear(int newYear)
{
    year = newYear;
}

void TagInfo::setTrack(int newTrack)
{
    track = newTrack;
}

QString TagInfo::getTitle() const
{
    return title;
}

QString TagInfo::getArtist() const
{
    return artist;
}

QString TagInfo::getAlbum() const
{
    return album;
}

int TagInfo::getYear() const
{
    return year;
}

QString TagInfo::getComment() const
{
    return comment;
}

int TagInfo::getTrack() const
{
    return track;
}

QString TagInfo::getGenre() const
{
    return genre;
}

bool TagInfo::getUseFileName() const
{
    return useFileName;
}
bool TagInfo::getUseTitle() const
{
    return useTitle;
}

bool TagInfo::getUseArtist() const
{
    return useArtist;
}

bool TagInfo::getUseAlbum() const
{
    return useAlbum;
}
bool TagInfo::getUseGenre() const
{
    return useGenre;
}

bool TagInfo::getUseComment() const
{
    return useComment;
}

bool TagInfo::getUseYear() const
{
    return useYear;
}
bool TagInfo::getUseTrack() const
{
    return useTrack;
}

void TagInfo::setUseFileName(bool set)
{
    useFileName = set;
}

void TagInfo::setUseTitle(bool set)
{
    useTitle = set;
}

void TagInfo::setUseArtist(bool set)
{
    useArtist = set;
}

void TagInfo::setUseAlbum(bool set)
{
    useAlbum = set;
}

void TagInfo::setUseGenre(bool set)
{
    useGenre = set;
}
void TagInfo::setUseComment(bool set)
{
    useComment = set;
}

void TagInfo::setUseYear(bool set)
{
    useYear = set;
}

void TagInfo::setUseTrack(bool set)
{
    useTrack = set;
}
