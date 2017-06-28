#include "filewriter.h"

FileWriter::FileWriter()
{
}

FileWriter::FileWriter(QString output, MergeData data)
{
    QFile file(output);

    if(file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        this->setDevice(&file);
        this->writeFile(data);
    }
    else {
        qDebug() << "Could not open file";
    }
}

void FileWriter::writeFile(MergeData data) {
    qDebug() << "TODO: write file";
    this->writeStartElement("PlayBackStatistics");

    this->writeComment("File created by merging other playback export files.");

    this->writeEndElement();
}

