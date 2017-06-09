#include "filereader.h"

FileReader::FileReader()
{
}

FileReader::FileReader(QString filePath)
{
    this->filePath = filePath;

    QFileInfo fileInfo(filePath);
    this->fileName = fileInfo.fileName();

    // check if the file exists and is actually a file
    if(fileInfo.exists() && fileInfo.isFile()) {

        this->fileSize = fileInfo.size();   // save the file's size in bytes

        entries = this->readFile(filePath);   // pass to read file function

    }
    else {

        this->raiseError("Could not find specified file.");

    }
}

QMap<QString, EntryStatistics *> FileReader::readFile(QString filePath)
{
    QFile file(filePath);
    QMap<QString, EntryStatistics *> entryMap;

    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        this->setDevice(&file);   // set xml stream reader to this file

        // loop to eof
        while(!this->atEnd()) {

            // if the item is an entry child tag
            if(this->isStartElement() && this->name().toString() == ENTRY_TAG) {

                entryCount++;

                // add information to stat struct
                EntryStatistics *stats = new EntryStatistics;
                stats->count = this->attributes().value("", COUNT).toInt();
                stats->rating = this->attributes().value("", RATING).toInt();
                stats->firstPlayed = this->attributes().value("", FIRST_PLAYED).toInt();
                stats->lastPlayed = this->attributes().value("", LAST_PLAYED).toInt();
                stats->added = this->attributes().value("", ADDED).toInt();

                totalPlays += stats->count;
                counts.append(stats->count);

                entryMap[this->attributes().value("", ID).toString()] = stats;  // add entry to qmap
            }

            // else save the version and mapping data found in the parent tag
            else if(this->isStartElement() && this->name().toString() == PARENT_TAG) {
                versionNumber = this->attributes().value("", VERSION).toString();
                mappingString = this->attributes().value("", MAPPING).toString();
            }

            this->readNext();
        }
    }
    else {
        this->raiseError("File could not be opened.");
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

