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

    // add comment saying this file was merged
    this->writeComment("File created by merging other playback export files.");

    // add PlaybackStatistics parent tag
    this->writeStartElement(PlayBackXmlName::PARENT_TAG);
    this->writeAttribute(PlayBackXmlName::VERSION, data.versionNumber);    // set Version attribute
    this->writeAttribute(PlayBackXmlName::MAPPING, data.mappingString);    // set Mapping attribute

    // go through each value in the MergeData "entries" QMap
    QMap<QString,EntryStatistics *>::iterator i;
    for(i = data.entries.begin(); i != data.entries.end(); i++) {
        writeEntry(i.key(), *i.value());
    }

    this->writeEndElement();
}

void FileWriter::writeEntry(QString id, EntryStatistics entryData) {

    writeStartElement(PlayBackXmlName::ENTRY_TAG);     // start Entry

    // required attributes
    this->writeAttribute(PlayBackXmlName::ID, id);
    this->writeAttribute(PlayBackXmlName::COUNT, QString::number(entryData.count));

    // optional attributes - don't add if value == 0
    if(entryData.rating != 0)
        this->writeAttribute(PlayBackXmlName::RATING, QString::number(entryData.rating));
    if(entryData.firstPlayed != 0)
        this->writeAttribute(PlayBackXmlName::FIRST_PLAYED, entryData.firstPlayed);
    if(entryData.lastPlayed != 0)
        this->writeAttribute(PlayBackXmlName::LAST_PLAYED, entryData.lastPlayed);
    if(entryData.added != 0)
        this->writeAttribute(PlayBackXmlName::ADDED, entryData.added);

    writeEndElement();  // end Entry
}
