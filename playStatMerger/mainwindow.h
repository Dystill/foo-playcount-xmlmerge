#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QListWidgetItem>
#include <filereader.h>

namespace Ui {
class MainWindow;
}

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

private:
    Ui::MainWindow *ui;
    QMap<QString, FileData *> files;  // maps file path to the xml parsing object

    FileData *exportFileData(FileReader *reader);   // extracts data obtained from an export file reader

};

#endif // MAINWINDOW_H
