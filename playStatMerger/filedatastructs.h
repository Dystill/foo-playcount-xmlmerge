#ifndef FILEDATASTRUCTS_H
#define FILEDATASTRUCTS_H

#include <QMap>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>

#include <QDebug>

/**
 * @brief The EntryStatistics struct to hold data for an individual entry
 */
struct EntryStatistics {
    int count = 0;
    int rating = 0;
    QString firstPlayed = 0;
    QString lastPlayed = 0;
    QString added = 0;
};

/**
 * @brief The FileData struct holds extra information for a single export file.
 *
 * Includes a map for all EntryStatistics.
 */
struct FileData {

    QString fileName;
    QString filePath;

    QMap<QString,EntryStatistics *> entries;    // entry id : EntryStatistics

    QString versionNumber;
    QString mappingString;

    int fileSize = 0;
    int entryCount = 0;
    int totalPlays = 0;

    qreal average = 0;
    qreal deviation = 0;

};

/**
 * @brief The MergeData struct holds data merging a set of FileData
 *
 * Can be seen as an abbreviated version of FileData
 */
struct MergeData {
    QMap<QString,EntryStatistics *> entries;    // entry id : EntryStatistics

    QString versionNumber;
    QString mappingString;
};

/**
 * @brief The PlaybackXmlName namespace holds XML tag strings for easy access
 */
namespace PlayBackXmlName
{
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
}

#endif // FILEDATASTRUCTS_H
