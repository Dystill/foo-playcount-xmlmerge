#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <QXmlStreamWriter>

#include "filedatastructs.h"

class FileWriter : public QXmlStreamWriter
{
private:

    void writeFile(FileData fileData);
public:
    FileWriter();
    FileWriter(QFile outputFile, FileData fileData);
};

#endif // FILEWRITER_H
