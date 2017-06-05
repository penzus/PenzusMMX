#ifndef TAGINFO_H
#define TAGINFO_H

#include <QString>
#include <QObject>

class TagInfo: public QObject
{
    Q_OBJECT
public:
    TagInfo();

    Q_PROPERTY(QString album READ getAlbum WRITE setAlbum)
    Q_PROPERTY(QString artist READ getArtist WRITE setArtist)
    Q_PROPERTY(QString comment READ getComment WRITE setComment)
    Q_PROPERTY(QString fileName READ getFileName WRITE setFileName)
    Q_PROPERTY(QString genre READ getGenre WRITE setGenre)
    Q_PROPERTY(QString title READ getTitle WRITE setTitle)
    Q_PROPERTY(bool useAlbum READ getUseAlbum WRITE setUseAlbum)
    Q_PROPERTY(bool useArtist READ getUseArtist WRITE setUseArtist)
    Q_PROPERTY(bool useComment READ getUseComment WRITE setUseComment)
    Q_PROPERTY(bool useFileName READ getUseFileName WRITE setUseFileName)
    Q_PROPERTY(bool useGenre READ getUseGenre WRITE setUseGenre)
    Q_PROPERTY(bool useTitle READ getUseTitle WRITE setUseTitle)
    Q_PROPERTY(bool useTrack READ getUseTrack WRITE setUseTrack)
    Q_PROPERTY(bool useYear READ getUseYear WRITE setUseYear)
    Q_PROPERTY(int track READ getTrack WRITE setTrack)
    Q_PROPERTY(int year READ getYear WRITE setYear)

    QString getAlbum() const;
    QString getArtist() const;
    QString getComment() const;
    QString getFileName() const;
    QString getGenre() const;
    QString getTitle() const;
    bool getUseAlbum() const;
    bool getUseArtist() const;
    bool getUseComment() const;
    bool getUseFileName() const;
    bool getUseGenre() const;
    bool getUseTitle() const;
    bool getUseTrack() const;
    bool getUseYear() const;
    int getTrack() const;
    int getYear() const;
    void clear();
    void read();
    void save();
    void setAlbum(const QString& album);
    void setArtist(const QString& artist);
    void setComment(const QString& comment);
    void setFileName(const QString& fileName);
    void setGenre(const QString& genre);
    void setTitle(const QString& title);
    void setTrack(int track);
    void setUseAlbum(bool set);
    void setUseArtist(bool set);
    void setUseComment(bool set);
    void setUseFileName(bool set);
    void setUseGenre(bool set);
    void setUseTitle(bool set);
    void setUseTrack(bool set);
    void setUseYear(bool set);
    void setYear(int year);

protected:
    QString album;
    QString artist;
    QString comment;
    QString fileName;
    QString genre;
    QString title;
    bool useAlbum;
    bool useArtist;
    bool useComment;
    bool useFileName;
    bool useGenre;
    bool useTitle;
    bool useTrack;
    bool useYear;
    int track;
    int year;
};

#endif // TAGINFO_H
