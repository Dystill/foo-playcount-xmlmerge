#ifndef FILEREADER_H
#define FILEREADER_H

#include <QXmlStreamReader>
#include <QMap>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QDebug>

// data structure to hold playback statistics for each song entry in the export xml file
struct EntryStatistics {
    int count = 0;
    int firstPlayed;
    int lastPlayed;
    int added;
};

class FileReader : public QXmlStreamReader
{
private:
    QMap<QString,EntryStatistics> entries;  // holds a map pairing an entry's ID to it's playback data
    QMap<QString,EntryStatistics> readFile(QString filePath);

    int fileSize;

public:
    FileReader();
    FileReader(QString filePath);
    QMap<QString, EntryStatistics> getEntries() const;
    int getFileSize() const;
};

#endif // FILEREADER_H
