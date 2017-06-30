#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QListWidgetItem>
#include <QButtonGroup>
#include <QStandardPaths>
#include <qmath.h>
#include <algorithm>

#include "filereader.h"
#include "filewriter.h"

namespace Ui {
class MainWindow;
}

/**
 * @brief The MergeType enum
 *
 * Used to determine how entry data should be merged if two files
 * share a same entry id.
 */
enum MergeType {
    AddPlaycounts = 1,  // add play counts together
    UseLargest = 2,     // use the largest of the values
    UseSmallest = 3     // use the smallest of the values
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_Add_clicked();

    void on_pushButton_Remove_clicked();

    void on_pushButton_Refresh_clicked();

    void on_pushButton_Merge_clicked();

    /**
     * @brief displayItemInfo refreshes data displayed in the gui
     * @param item  The highlighted list item to display the file information of
     */
    void displayItemInfo(QListWidgetItem *item);

private:
    Ui::MainWindow *ui;


    // Variables

    /**
     * @brief mergeTypeButtonGroup invisible container for merge type radio buttons
     */
    QButtonGroup mergeTypeButtonGroup;

    /**
     * @brief files QMap holds all the files currently loaded
     *
     * Uses the filepath as the key to a FileData struct
     */
    QMap<QString, FileData *> files;

    /**
     * @brief defaultOutputFileDir the default output directory to initialize with
     */
    QString const defaultOutputFileDir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);

    /**
     * @brief defaultOutputFileName the default output file name to initalize with
     */
    QString const defaultOutputFileName = "file";

    /**
     * @brief outputFileDir holds the output file destination on merge
     */
    QString outputFileDir;

    /**
     * @brief outputFileName holds the output file name on merge
     */
    QString outputFileName;

    /**
     * @brief prevFileDir holds where the previously opened file was located
     */
    QString prevFileDir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);


    // Functions

    /**
     * @brief exportFileData extracts and packages data obtained from the playback export file reader.
     *
     * Function is required to prevent the obtained data from being deleted or removed from the scope.
     *
     * @param reader    The FileReader object used to obtain playback export data
     * @return FileData struct pointer that holds all the obtained data
     */
    FileData *exportFileData(FileReader *reader);

    /**
     * @brief calculateStdev calculates the standard deviation for a set of numerical data.
     * @param values    The population of data to use
     * @param average   The average of the values
     * @return qreal    Standard Deviation of the values
     */
    qreal calculateStdev(QList<qreal> values, qreal average);   // function to calculate a standard deviation

    /**
     * @brief groupRadioButtons add MergeType radio buttons to a single group
     */
    void groupRadioButtons();

    /**
     * @brief mergeFileData Merges a set of FileData objects into one MergeData object
     * @param fileData      A QList of pointers to all the FileData objects to merge
     * @param mergeType     An enum value for how to combine conflicting values
     * @return MergeData    Structure of all the merged FileData objects
     */
    MergeData mergeFileData(QList<FileData *> fileData, int mergeType);

    /**
     * @brief compareFileVersionAndMapping compares the Version and Mapping of a set of FileData objects.
     * @param fileData  A QList of pointers to all the FileData objects to compare
     * @return bool     true if Version are all equal and Mapping are all equal / false otherwise
     */
    bool compareFileVersionAndMapping(QList<FileData *> fileData);

    /**
     * @brief checkFileExistence sees if a given string is a path to an existing file
     * @param outputLocation    String of the formatted path to a file
     * @return bool             true if the file exists / false otherwise
     */
    bool checkFileExistence(QString outputLocation);

    /**
     * @brief promptUserToContinue displays a dialog asking the user if they want to continue an action
     * @param errorString   The string to display to the user in the dialog
     * @param defaultButton The button to highlight initally / default = Cancel
     * @return bool true if they chose to proceed / false if not
     */
    bool promptUserToContinue(QString errorString, QMessageBox::StandardButton defaultButton = QMessageBox::Cancel);

    /**
     * @brief copyFileDataToMergeData copies data from a FileData object to a MergeData object
     *
     * Copies the Version number, Mapping string, and all entry data.
     *
     * @param fileData      The FileData object to copy from.
     * @return MergeData    after copying the necessary data
     */
    MergeData copyFileDataToMergeData(FileData *fileData);

    /**
     * @brief parseFile creates a FileReader to parse a file in the passed in path
     * @param filePath  File path pointing to the playback export file to be parsed
     * @return bool     true if parsing was successful
     */
    bool parseFile(QString filePath);
};

#endif // MAINWINDOW_H
