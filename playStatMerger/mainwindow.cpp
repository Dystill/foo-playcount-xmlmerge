#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* TODO: potentially replace list widget with a table
        QStringList tableHeaders;
        tableHeaders.append("File Name");
        tableHeaders.append("Size");

        ui->tableWidget->setHorizontalHeaderLabels(tableHeaders);
        ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    */

    connect(ui->listWidget, SIGNAL(itemClicked(QListWidgetItem *)),
            this, SLOT(displayItemInfo(QListWidgetItem *)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::displayItemInfo(QListWidgetItem *item) {

    QString filePath = item->text();
    FileData *data = files.value(filePath);

    ui->lineEditNumberEntries->setText(QString::number(data->entryCount));
    ui->lineEditVersion->setText(data->versionNumber);
    ui->lineEditMapping->setText(data->mappingString);

    ui->lineEditTotalPlays->setText(QString::number(data->totalPlays));
    ui->lineEditAverage->setText(QString::number(data->average));
    ui->lineEditDeviation->setText(QString::number(data->deviation));

    ui->lineEditFilePath->setText(filePath);

}

FileData *MainWindow::exportFileData(FileReader *reader)
{
    FileData *data = new FileData();

    data->fileName = reader->getFileName();
    data->filePath = reader->getFilePath();

    data->entries = reader->getEntries();

    data->versionNumber = reader->getVersion();
    data->mappingString = reader->getMapping();

    data->fileSize = reader->getFileSize();
    data->entryCount = reader->getEntryCount();

    data->totalPlays = reader->getTotalPlays();

    data->average = qreal(data->totalPlays) / data->entryCount;
    data->deviation = calculateStdev(reader->getCounts(), data->average);

    return data;
}

qreal MainWindow::calculateStdev(QList<qreal> values, qreal average) {

    qreal varSum = 0;

    // calculate deviation of each value from the mean
    foreach(int i, values) {
        varSum += qPow(i - average, 2);
    }

    // return standard deviation
    return qSqrt(varSum / values.count());
}

void MainWindow::on_pushButton_Add_clicked()
{
    // open file choose dialog
    QString filePath = QFileDialog::getOpenFileName(this,
                                                    tr("Open Playback Statistics Export File"),
                                                    QDir::currentPath(),
                                                    tr("Playback File (*.xml)"));

    // if the file was not already added
    if(!files.contains(filePath)) {

        // parse the file
        FileReader fileReader(filePath);

        // check if any errors were raised
        if(!fileReader.hasError()) {        // no error

            // add file to qmap with filepath as the key
            files[filePath] = exportFileData(&fileReader);

            // add file to the ui list widget and highlight it
            ui->listWidget->setCurrentItem(new QListWidgetItem(fileReader.getFilePath(), ui->listWidget));

            // display the files information in the ui
            displayItemInfo(ui->listWidget->currentItem());

        }
        else {                              // some error
            if(fileReader.error() != QXmlStreamReader::CustomError)
                QMessageBox::warning(this,"Error parsing file",fileReader.errorString());
        }

    }
    else {
        QMessageBox::information(this,"","File already added.");
    }
}

void MainWindow::on_pushButton_Remove_clicked()
{
    // if one item was selected
    //  (multi-selection is not currently enabled here)
    if(ui->listWidget->selectedItems().count() == 1) {

        // remove the currently selected file qmap
        delete files.take(ui->listWidget->currentItem()->text());

        // remove from the ui list
        delete ui->listWidget->currentItem();

        // display item info for the item highlighted after the previous one was removed
        if(ui->listWidget->selectedItems().count() == 1)
            displayItemInfo(ui->listWidget->currentItem());
    }
}

void MainWindow::on_pushButton_Refresh_clicked()
{
    // TODO: reread xml file and redisplay information
}
