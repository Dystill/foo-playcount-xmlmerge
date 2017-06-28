#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <QXmlStreamWriter>

#include "filedatastructs.h"

class FileWriter : public QXmlStreamWriter
{
private:
    void writeFile(MergeData data);

public:
    FileWriter();
    FileWriter(QString output, MergeData data);
};

#endif // FILEWRITER_H
