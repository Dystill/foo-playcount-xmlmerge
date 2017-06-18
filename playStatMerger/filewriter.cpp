#include "filewriter.h"

FileWriter::FileWriter()
{
}

FileWriter::FileWriter(QFile outputFile, FileData fileData)
    : QXmlStreamWriter(&outputFile)
{
    this->writeFile(fileData);
}

void FileWriter::writeFile(FileData fileData) {
    qDebug() << "TODO: write file";
}

