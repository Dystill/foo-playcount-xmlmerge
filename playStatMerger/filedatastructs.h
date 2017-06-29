#ifndef FILEDATASTRUCTS_H
#define FILEDATASTRUCTS_H

#include <QMap>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>

#include <QDebug>

// data structure to hold playback statistics for each song entry in the export xml file
struct EntryStatistics {
    int count = 0;
    int rating = 0;
    QString firstPlayed = 0;
    QString lastPlayed = 0;
    QString added = 0;
};

// data structure to hold the information for a single export file
struct FileData {

    QString fileName;
    QString filePath;

    QMap<QString,EntryStatistics *> entries;

    QString versionNumber;
    QString mappingString;

    int fileSize = 0;
    int entryCount = 0;
    int totalPlays = 0;

    qreal average = 0;
    qreal deviation = 0;

};

// data structure to hold file information after merging
struct MergeData {
    QMap<QString,EntryStatistics *> entries;

    QString versionNumber;
    QString mappingString;
};

#endif // FILEDATASTRUCTS_H
