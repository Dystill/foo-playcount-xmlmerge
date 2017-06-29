#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <QXmlStreamWriter>

#include "filedatastructs.h"

class FileWriter : public QXmlStreamWriter
{
private:
    /**
     * @brief writeFile uses QXmlStreamWriter functions to write XML data
     *
     * Writes the full parent tag and uses writeEntry to write each entry
     *
     * @param data Struct containing all the necessary information to write the file
     */
    void writeFile(MergeData data);

    /**
     * @brief writeEntry writes the formatted XML for a single entry
     * @param id        The entry's ID attribute value
     * @param entryData The entry's data to be written as addtitional attribute values
     */
    void writeEntry(QString id, EntryStatistics entryData);
public:
    /**
     * @brief FileWriter empty constructor
     */
    FileWriter();

    /**
     * @brief FileWriter MergeData constructor
     *
     * Prepares the QXmlStreamWriter to write to a given file destination
     *
     * @param output    The path of the file that should be written to
     * @param data      The data to use in writing the playback export data
     */
    FileWriter(QString output, MergeData data);
};

#endif // FILEWRITER_H
