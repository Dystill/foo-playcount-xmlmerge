#include "filewriter.h"

FileWriter::FileWriter()
{
}

FileWriter::FileWriter(QString output, MergeData data)
{
    QFile file(output);

    if(file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        this->setDevice(&file);
        this->setAutoFormatting(true);
        this->writeFile(data);
    }
    else {
        qDebug() << "Could not open file";
    }
}

void FileWriter::writeFile(MergeData data) {
    qDebug() << "TODO: write file";

    // add comment saying this file was merged
    this->writeComment("File created by merging other playback export files.");

    // add PlaybackStatistics parent tag
    this->writeStartElement("PlaybackStatistics");
    this->writeAttribute("Version", data.versionNumber);    // set Version attribute
    this->writeAttribute("Mapping", data.mappingString);    // set Mapping attribute

    // go through each value in the MergeData "entries" QMap
    QMap<QString,EntryStatistics *>::iterator i;
    for(i = data.entries.begin(); i != data.entries.end(); i++) {
        writeEntry(i.key(), *i.value());
    }

    this->writeEndElement();
}

void FileWriter::writeEntry(QString id, EntryStatistics entryData) {

    writeStartElement("Entry");     // start Entry

    // required attributes
    this->writeAttribute("ID", id);
    this->writeAttribute("Count", QString::number(entryData.count));

    // optional attributes - don't add if value == 0
    if(entryData.rating != 0)
        this->writeAttribute("Rating", QString::number(entryData.rating));
    if(entryData.firstPlayed != 0)
        this->writeAttribute("FirstPlayed", entryData.firstPlayed);
    if(entryData.lastPlayed != 0)
        this->writeAttribute("LastPlayed", entryData.lastPlayed);
    if(entryData.added != 0)
        this->writeAttribute("Added", entryData.added);

    writeEndElement();  // end Entry
}
