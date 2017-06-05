#include <bb/system/Clipboard>

#include <QFile>
#include <QFileInfo>
#include <QTextStream>

#include "settings.h"
#include "texteditor.h"
#include "fileutils.h"

TextEditor::TextEditor()
{

}

QString TextEditor::lastError() const {
    return lastErrorCode;
}

bool TextEditor::fileCanLoaded(const QString& sourceFileName) const
{
    QFile file(sourceFileName);

    const quint64 unit = 1024;
    if (file.size() > (unit * unit * 30) ) {
        lastErrorCode = tr("File too big!");
        return false;
    }
    return true;
}

bool TextEditor::loadFileName(const QString& sourceFileName)
{
    lastErrorCode.clear();

    QString fileName = sourceFileName;
    QString prefix = "file://";
    fileName.replace(prefix, "");

    QFile file(fileName);
    if (!file.exists()) {
        lastErrorCode = tr("File not exists!");
        currentFile.clear();
        return false;
    }

    if (!fileCanLoaded(sourceFileName)) {
        return false;
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        lastErrorCode = tr("Cannot open file!");
        return false;
    }

    sourceData = file.readAll();
    currentFile = fileName;
    return true;
}

QString TextEditor::fileBaseName(const QString& fileName) const {
    QFileInfo fi(fileName);
    return fi.fileName();
}

QString TextEditor::fileNameData() const {
    const char* inputText = sourceData.constData();
    return QString::fromUtf8(inputText);
}

void TextEditor::resetCurrentFile()
{
    currentFile.clear();
}

bool TextEditor::saveToFileName(const QString& fileName, const QString& text)
{
    QFile file(fileName);
    if (!file.open(
                QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        lastErrorCode = tr("Can not open file for writing!");
        return false;
    };

    QByteArray codecName("UTF-8");
    QTextStream out(&file);
    out.setCodec(codecName);

    out << text;
    currentFile = fileName;
    return true;
}

void TextEditor::copyToClipboard(const QString& text) {
    bb::system::Clipboard clipboard;
    clipboard.clear();
    clipboard.insert("text/plain", text.toStdString().c_str());
}

QString TextEditor::docDirectory() const
{
    QFileInfo fi(currentFile);
    if (fi.absolutePath().length() == 0) {
        return "/accounts/1000/shared/documents";
    }
    return fi.absolutePath();
}

QString TextEditor::currentFileNamePath() const
{
    return currentFile;
}

QString TextEditor::docProperties(const QString& bufferData) const
{
    QString result;
    QFileInfo f(currentFileNamePath());
    QString docWordCount;
    QString docLineCount;
    if (f.exists()) {
        result += tr("Path:") + " " + FileUtils::getHumanReadablePath(currentFileNamePath()) + "\n\n"
                + tr("Created:") + " " + Settings::instance().dateTimeI18n(f.created()) + "\n"
                + tr("Modified: ") + " " + Settings::instance().dateTimeI18n(f.lastModified()) + "\n";
        result += tr("File size (bytes):") + " " + QString::number(f.size()) + "\n";
    }

    docWordCount = QString::number( bufferData.split(QRegExp("\\s+"),QString::SkipEmptyParts).count()  );
    docLineCount = QString::number( bufferData.split(QRegExp("[\r\n]"),QString::KeepEmptyParts).count() );

    QString bufferInfo;
    bufferInfo += tr("Word count:") + " " + docWordCount +  "\n"
            +tr("Line count:") + " " + docLineCount + "\n";

    return result + bufferInfo;
}
