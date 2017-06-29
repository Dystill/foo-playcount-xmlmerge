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

    // add PlaybackStatistics parent tag
    this->writeStartElement("PlaybackStatistics");
    this->writeAttribute("Version", data.versionNumber);    // set Version attribute
    this->writeAttribute("Mapping", data.mappingString);    // set Mapping attribute

    // go through each value in the MergeData "entries" QMap
    QMap<QString,EntryStatistics *>::iterator i;
    for(i = data.entries.begin(); i != data.entries.end(); i++) {
        qDebug() << i.key();
    }

    // add comment saying this file was merged
    this->writeComment("File created by merging other playback export files.");



    this->writeEndElement();
}

void FileWriter::writeEntry(QString id, EntryStatistics entryData) {



}
