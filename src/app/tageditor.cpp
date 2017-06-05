#include <QDateTime>
#include <QDebug>

#include <iostream>
#include <iomanip>

#include "tageditor.h"
#include "applicationui.h"
#include "tstring.h"
#include "fileref.h"
#include "tag.h"
#include "tpropertymap.h"
#include "fileutils.h"
#include "settings.h"

// for covers
#include <mpegfile.h>
#include <attachedpictureframe.h>
#include <id3v2tag.h>
#include <mp4file.h>
#include <mp4tag.h>
#include <mp4coverart.h>
#include <vorbisfile.h>
#include <xiphcomment.h>
#include <flacpicture.h>
#include <tbytevector.h>//ByteVector
#include <mpegfile.h>//mp3 file
#include <id3v2tag.h>//tag
#include <id3v2frame.h>//frame
#include <attachedPictureFrame.h>//attachedPictureFrame

using namespace std;

TagEditor::TagEditor()
{
    clear();
}

bool TagEditor::setFileName (const QString& fileName)
{
    clear();

    QByteArray ba = fileName.toLocal8Bit();
    TagLib::FileRef f(ba.data());

    if(!f.isNull() && f.tag()) {

        TagLib::Tag *tag = f.tag();

        ti.setTitle ( TStringToQString(tag->title()) );
        ti.setArtist ( TStringToQString(tag->artist()) );
        ti.setAlbum ( TStringToQString(tag->album()) );
        ti.setGenre ( TStringToQString(tag->genre()) );
        ti.setComment ( TStringToQString(tag->comment()) );
        ti.setTrack ( tag->track() );
        ti.setYear ( tag->year() );

        TagLib::PropertyMap tags = f.file()->properties();

        unsigned int longest = 0;
        for(TagLib::PropertyMap::ConstIterator i = tags.begin(); i != tags.end(); ++i) {
            if (i->first.size() > longest) {
                longest = i->first.size();
            }
        }

        for(TagLib::PropertyMap::ConstIterator i = tags.begin(); i != tags.end(); ++i) {
            for(TagLib::StringList::ConstIterator j = i->second.begin(); j != i->second.end(); ++j) {
                cout << left << std::setw(longest) << i->first << " - " << '"' << *j << '"' << endl;
            }
        }
    } else {
        return false;
    }

    if(!f.isNull() && f.audioProperties()) {

        TagLib::AudioProperties *properties = f.audioProperties();

        int seconds = properties->length() % 60;
        int minutes = (properties->length() - seconds) / 60;

        bitrate = QString::number(properties->bitrate());
        sampleRate = QString::number(properties->sampleRate());
        channels = QString::number(properties->channels());
        length = QString("%1:%2").arg(minutes).arg(seconds);
    }

    fi.setFile(fileName);
    validState = true;

    return true;
}
void TagEditor::clear()
{
    validState = false;
    ti.clear();
    bitrate.clear();
    sampleRate.clear();
    channels.clear();
    length.clear();
}

QString TagEditor::getBitrate() const
{
    return bitrate;
}

QString TagEditor::getSampleRate() const
{
    return sampleRate;
}

QString TagEditor::getChannels() const
{
    return channels;
}

QString TagEditor::getLength() const
{
    return length;
}

bool TagEditor::isValid() const
{
    return validState;
}

bool TagEditor::hasCoverSupport() const
{
    QString suffix = fi.suffix().toUpper();
    return ( suffix == "MP3");
}

QString TagEditor::internalImageCover() const
{
    return getInternalImageCoverPrivate();
}

bool  TagEditor::setInternalImageCover(const QString& fileName)
{
    return setInternalImageCoverPrivate(fileName);
}

#define QStringToTString2(s) TagLib::String(s.toUtf8().data(), TagLib::String::UTF8)

bool TagEditor::save()
{
    QByteArray ba = fi.absoluteFilePath().toUtf8();
    TagLib::FileRef f(ba.data());


    if(!f.isNull() && f.tag()) {
        TagLib::Tag *tag = f.tag();

        tag->setTitle(QStringToTString2(ti.getTitle()));
        tag->setArtist(QStringToTString2(ti.getArtist()));
        tag->setAlbum(QStringToTString2(ti.getAlbum()));
        tag->setComment(QStringToTString2(ti.getComment()));
        tag->setGenre(QStringToTString2(ti.getGenre()));
        tag->setYear(ti.getYear());
        tag->setTrack(ti.getTrack());

        return f.file()->save();
    }
    return false;
}

class ImageFile : public TagLib::File
{
public:
    ImageFile(const char *file) : TagLib::File(file)
    {
    }
    TagLib::ByteVector data()
    {
        return readBlock(length());
    }
private:
    virtual TagLib::Tag *tag() const { return 0; }
    virtual TagLib::AudioProperties *audioProperties() const { return 0; }
    virtual bool save() { return false; }
};

using namespace TagLib::ID3v2 ;

QString TagEditor::getInternalImageCoverPrivate() const
{
    QString outputFileName;
    QByteArray ba = fi.absoluteFilePath().toUtf8();

    TagLib::MPEG::File mp3File(ba.data());


    if(mp3File.ID3v2Tag()) {
        TagLib::ID3v2::AttachedPictureFrame * pictureFrame;
        TagLib::ID3v2::FrameList listOfMp3Frames = mp3File.ID3v2Tag()->frameListMap()["APIC"];//look for picture frames only
        if(!listOfMp3Frames.isEmpty()) {
            TagLib::ID3v2::FrameList::ConstIterator it= listOfMp3Frames.begin();

            for(; it != listOfMp3Frames.end() ; it++) {

                pictureFrame = static_cast<TagLib::ID3v2::AttachedPictureFrame *> (*it);//cast Frame * to AttachedPictureFrame*
                QString mimeType = TStringToQString(pictureFrame->mimeType());

                QString suffix = "jpg";
                if (mimeType == "image/jpg") suffix = "jpg";
                else if (mimeType == "image/png") suffix = "png";

                outputFileName = Settings::instance().getTmpDir() + QString("/.tmp-image-%1.").arg(time(0)) + suffix;

                QFile file(outputFileName);
                file.open(QIODevice::WriteOnly | QIODevice::Truncate);
                file.write(pictureFrame->picture().data(), pictureFrame->picture().size());
                file.close();
                break;
            }
        } else {
            outputFileName.clear();
        }
    } else {
        outputFileName.clear();
    }
    return outputFileName;
}


bool  TagEditor::setInternalImageCoverPrivate(const QString& imageFileName)
{
    QByteArray ba = fi.absoluteFilePath().toUtf8();
    TagLib::String fileName = ba.data();

    QString suffix = fi.suffix().toUpper();
    TagLib::String fileType = suffix.toUtf8().data();

    ImageFile imageFile(imageFileName.toUtf8().data() );

    TagLib::ByteVector imageData = imageFile.data();
    bool result = false;

    if (fileType == "MP3") {
        TagLib::MPEG::File audioFile(ba.data());
        TagLib::ID3v2::Tag *tag = audioFile.ID3v2Tag(true);
        TagLib::ID3v2::AttachedPictureFrame *frame = new TagLib::ID3v2::AttachedPictureFrame;

        TagLib::ID3v2::FrameList listOfMp3Frames = audioFile.ID3v2Tag()->frameListMap()["APIC"];//look for picture frames only
        if(!listOfMp3Frames.isEmpty()) {
            TagLib::ID3v2::FrameList::ConstIterator it= listOfMp3Frames.begin();
            for(; it != listOfMp3Frames.end() ; it++) {
                TagLib::ID3v2::AttachedPictureFrame * pictureFrame = static_cast<TagLib::ID3v2::AttachedPictureFrame *> (*it);
                tag->removeFrame(pictureFrame);
            }
        }
        QFileInfo fi(imageFileName);
        if (fi.exists()) {
            frame->setMimeType("image/jpeg");
            frame->setPicture(imageData);
            tag->addFrame(frame);
        }

        result = audioFile.save();
    } else {
        qDebug() << " is unsupported.";
    }

    return result;
}
