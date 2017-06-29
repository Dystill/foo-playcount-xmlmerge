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

enum MergeType {
    AddPlaycounts = 1,
    UseLargest = 2,
    UseSmallest = 3
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

    void displayItemInfo(QListWidgetItem *item);    // shows file data in the ui

    void on_pushButton_Merge_clicked();

private:
    Ui::MainWindow *ui;
    QButtonGroup mergeTypeButtonGroup;  // invisible container for merge type radio buttons

    QMap<QString, FileData *> files;  // maps file path to the xml parsing object

    // default output location
    QString const defaultOutputFileDir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString const defaultOutputFileName = "file";

    // holds the output destination while merging
    QString outputFileDir;
    QString outputFileName;

    // hold where the previously opened file was located
    QString prevFileDir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);

    FileData *exportFileData(FileReader *reader);   // extracts data obtained from an export file reader

    qreal calculateStdev(QList<qreal> values, qreal average);   // function to calculate a standard deviation
    void groupRadioButtons();   // function to add radio buttons to groups

    MergeData mergeFileData(QList<FileData *> fileData, int mergeType);
    bool compareFileVersionAndMapping(QList<FileData *> fileData);
    // QMap<QString, EntryStatistics *> addPlayCountEntries(QList<FileData *> fileData);

    bool checkFileExistence(QString outputLocation);

    bool promptUserToContinue(QString errorString, QMessageBox::StandardButton defaultButton = QMessageBox::Cancel);
};

#endif // MAINWINDOW_H
