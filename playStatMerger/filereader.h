#ifndef FILEREADER_H
#define FILEREADER_H

#include <QXmlStreamReader>

#include "filedatastructs.h"

class FileReader : public QXmlStreamReader
{
private:
    QMap<QString,EntryStatistics *> entries;  // holds a map pairing an entry's ID to it's playback data
    QMap<QString,EntryStatistics *> readFile();

    QList<qreal> counts;  // holds each count value for faster access - no association with entry ids

    // meta data
    QString fileName;
    QString filePath;
    QString versionNumber;
    QString mappingString;
    int fileSize = 0;
    int entryCount = 0;
    int totalPlays = 0;

public:
    FileReader();
    FileReader(QFile file);
    FileReader(QString filePath);
    QMap<QString, EntryStatistics *> getEntries() const;
    QString getFileName() const;
    QString getFilePath() const;
    int getFileSize() const;
    int getEntryCount() const;
    QString getVersion() const;
    QString getMapping() const;
    int getTotalPlays() const;
    QList<qreal> getCounts() const;
};

#endif // FILEREADER_H
