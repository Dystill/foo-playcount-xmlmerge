#include "filereader.h"

FileReader::FileReader()
{
}

FileReader::FileReader(QString filePath)
{
    // check if the file exists and is actually a file
    QFileInfo fileInfo(filePath);
    if(fileInfo.exists() && fileInfo.isFile()) {
        this->fileSize = fileInfo.size();   // save the file's size

        entries = this->readFile(filePath);   // pass to read file function
    }
    else {
        this->raiseError("Could not find specified file.");
    }
}

QMap<QString, EntryStatistics> FileReader::readFile(QString filePath)
{
    QFile file(filePath);

    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        this->setDevice(&file);   // set xml stream reader to this file

        while(!this->atEnd()) {
            qDebug() << this->tokenString() << this->name().toString();
            this->readNext();
        }
    }
    else {
        this->raiseError("File could not be opened.");
    }
}

QMap<QString, EntryStatistics> FileReader::getEntries() const
{
    return entries;
}

int FileReader::getFileSize() const
{
    return fileSize;
}

