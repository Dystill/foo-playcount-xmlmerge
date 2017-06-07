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
    int rating;
    int firstPlayed;
    int lastPlayed;
    int added;
};

class FileReader : public QXmlStreamReader
{
private:
    QMap<QString,EntryStatistics *> entries;  // holds a map pairing an entry's ID to it's playback data
    QMap<QString,EntryStatistics *> readFile(QString filePath);

    // XML tag strings
    const QString PARENT_TAG = "PlaybackStatistics";
    const QString VERSION = "Version";
    const QString MAPPING = "Mapping";

    const QString ENTRY_TAG = "Entry";
    const QString ID = "ID";
    const QString COUNT = "Count";
    const QString RATING = "Rating";
    const QString ADDED = "Added";
    const QString FIRST_PLAYED = "FirstPlayed";
    const QString LAST_PLAYED = "LastPlayed";

    // meta data
    QString versionNumber;
    QString mappingString;
    int fileSize = 0;
    int entryCount = 0;
    int totalPlays = 0;

public:
    FileReader();
    FileReader(QString filePath);
    QMap<QString, EntryStatistics *> getEntries() const;
    int getFileSize() const;
    int getEntryCount() const;
    QString getVersion() const;
    QString getMapping() const;
    int getTotalPlays() const;
};

#endif // FILEREADER_H
