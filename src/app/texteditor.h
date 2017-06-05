#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QObject>
#include <QByteArray>
#include <QString>

class TextEditor: public QObject
{
    Q_OBJECT
public:
    TextEditor();

    Q_INVOKABLE QString currentFileNamePath() const;
    Q_INVOKABLE QString docDirectory() const;
    Q_INVOKABLE QString docProperties(const QString& bufferData) const;
    Q_INVOKABLE QString fileBaseName(const QString& fileName) const;
    Q_INVOKABLE QString fileNameData() const;
    Q_INVOKABLE QString lastError() const;
    Q_INVOKABLE bool fileCanLoaded(const QString& sourceFileName) const;
    Q_INVOKABLE bool loadFileName(const QString& fileName);
    Q_INVOKABLE bool saveToFileName(const QString& fileName, const QString& text);
    Q_INVOKABLE void copyToClipboard(const QString& text);
    Q_INVOKABLE void resetCurrentFile();

protected:
    QByteArray sourceData;
    QString currentFile;
    mutable QString lastErrorCode;
};

#endif // TEXTEDITOR_H
