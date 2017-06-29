#include "filereader.h"

FileReader::FileReader()
{
}

// constructor that takes a QFile object to read from
FileReader::FileReader(QFile file) {

    // get the file path
    this->filePath = file.fileName();

    // get file information
    QFileInfo fileInfo(this->filePath);
    this->fileName = fileInfo.fileName();   // file name

    // check if the file exists and is actually a file
    if(fileInfo.exists() && fileInfo.isFile()) {

        this->fileSize = fileInfo.size();   // save the file's size in bytes

        // attempt to open the file for reading
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {

            this->setDevice(&file);   // set xml stream reader to this file

            entries = this->readFile();   // pass to read file function

        }
        else {
            this->raiseError("Could not open the specified file.");
        }
    }
    else {
        this->raiseError("Could not find the specified file.");
    }
}

// constructor that takes a fiull file path to read from
FileReader::FileReader(QString filePath)
{
    this->filePath = filePath;

    QFileInfo fileInfo(filePath);
    this->fileName = fileInfo.fileName();

    // check if the file exists and is actually a file
    if(fileInfo.exists() && fileInfo.isFile()) {

        this->fileSize = fileInfo.size();   // save the file's size in bytes

        QFile file(filePath);

        // attempt to open the file for reading
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {

            this->setDevice(&file);   // set xml stream reader to this file

            entries = this->readFile();   // pass to read file function

        }
        else {
            this->raiseError("Could not open the specified file.");
        }
    }
    else {
        this->raiseError("Could not find the specified file.");
    }
}

QMap<QString, EntryStatistics *> FileReader::readFile()
{
    QMap<QString, EntryStatistics *> entryMap;

    // loop to eof
    while(!this->atEnd()) {

        // if the item is an entry child tag
        if(this->isStartElement() && this->name().toString() == ENTRY_TAG) {

            entryCount++;

            // add information to stat struct
            EntryStatistics *stats = new EntryStatistics;
            stats->count = this->attributes().value("", COUNT).toInt();
            stats->rating = this->attributes().value("", RATING).toInt();
            stats->firstPlayed = this->attributes().value("", FIRST_PLAYED).toString();
            stats->lastPlayed = this->attributes().value("", LAST_PLAYED).toString();
            stats->added = this->attributes().value("", ADDED).toString();

            totalPlays += stats->count; // keep track of total plays
            counts.append(stats->count);

            entryMap[this->attributes().value("", ID).toString()] = stats;  // add entry to qmap

            qDebug() << "<Entry ID=" << this->attributes().value("", ID).toString()
                     << " Count=" << stats->count
                     << " Added=" << stats->added
                     << " FirstPlayed=" << stats->firstPlayed
                     << "/>";
        }

        // else save the version and mapping data found in the parent tag
        else if(this->isStartElement() && this->name().toString() == PARENT_TAG) {
            versionNumber = this->attributes().value("", VERSION).toString();
            mappingString = this->attributes().value("", MAPPING).toString();
        }

        this->readNext();
    }

    return entryMap;
}

QList<qreal> FileReader::getCounts() const
{
    return counts;
}

QString FileReader::getFilePath() const
{
    return filePath;
}

QString FileReader::getFileName() const
{
    return fileName;
}

QMap<QString, EntryStatistics *> FileReader::getEntries() const
{
    return entries;
}

int FileReader::getTotalPlays() const
{
    return totalPlays;
}

QString FileReader::getMapping() const
{
    return mappingString;
}

QString FileReader::getVersion() const
{
    return versionNumber;
}

int FileReader::getEntryCount() const
{
    return entryCount;
}
int FileReader::getFileSize() const
{
    return fileSize;
}

