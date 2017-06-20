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

    groupRadioButtons();

    // set output directory to "Documents" folder
    ui->lineEditFilePath_Output->setText(defaultOutputFileDir);
    ui->lineEditFilePath_OutputName->setText(defaultOutputFileName);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::groupRadioButtons() {
    // add the merge type radio buttons to a single group and assign each a MergeType id
    mergeTypeButtonGroup.addButton(ui->radioButton_Add, AddPlaycounts);
    mergeTypeButtonGroup.addButton(ui->radioButton_Largest, UseLargest);
    mergeTypeButtonGroup.addButton(ui->radioButton_Smallest, UseSmallest);
}

MergeData MainWindow::mergeFileData(QList<FileData *> fileData, int mergeType)
{
    MergeData data;

    // if there is more than 1 file
    if(fileData.size() > 1) {

        ///
        /// compare version and mapping data for all files
        ///

        bool sameVersion = true,
             sameMapping = true;

        // compare version numbers and mappings for each file
        for(int i = 1; i < fileData.size(); i++) {
            if(fileData.at(i)->versionNumber.compare(fileData.at(i-1)->versionNumber) != 0 && sameVersion) {
                sameVersion = false;
            }
            if(fileData.at(i)->mappingString.compare(fileData.at(i-1)->mappingString) != 0 && sameMapping) {
                sameMapping = false;
            }
        }

        // if either the version numbers or mappings are NOT equal
        if(!(sameVersion && sameMapping)) {
            qDebug() << "Version:" << sameVersion;
            qDebug() << "Mapping:" << sameMapping;

            // create a readable error string to tell to the user what isn't the same across the files
            QString errorString = "The " +
                    QString(sameVersion ? "" : "version") +                         // versions aren't the same
                    QString(sameVersion || sameMapping ? "" : " and ") +            // both aren't the same
                    QString(sameMapping ? "" : "mapping") +                         // mappings aren't the same
                    " values for some of the selected files are not equal.\nContinue?";

            qDebug() << errorString;
        }


        ///
        /// merge the data based on selected merge type
        ///

        switch (mergeType) {
        case AddPlaycounts: // add radio button
            qDebug() << "Add playcounts";

            break;
        case UseLargest: // largest radio button
        case UseSmallest: // smallest radio button
        default:
            QMessageBox::information(this,"","Not yet supported.");
            break;
        }
    }
    // if there is only 1 file
    else if(fileData.size() == 1) {
        // copy data into the MergeData struct
        data.entries = files.values().at(0)->entries;
        data.versionNumber = files.values().at(0)->versionNumber;
        data.mappingString = files.values().at(0)->mappingString;
    }

    // return the MergeData struct
    return data;
}

void MainWindow::displayItemInfo(QListWidgetItem *item) {

    QString filePath = item->text();
    FileData *data = files.value(filePath);

    // take data for the selected file and display it in the corresponding fields
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

    // obtain parsed data from the file reader
    data->fileName = reader->getFileName();
    data->filePath = reader->getFilePath();

    data->entries = reader->getEntries();   // qmap of all the entry statistics

    data->versionNumber = reader->getVersion();
    data->mappingString = reader->getMapping();

    data->fileSize = reader->getFileSize();
    data->entryCount = reader->getEntryCount();

    data->totalPlays = reader->getTotalPlays();

    // calculate the average playcount and stdev
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
                                                    prevFileDir,
                                                    tr("Playback File (*.xml)"));

    // if the user did not cancel the action
    if(!filePath.isNull()) {

        // save the file's path to use next time
        QFileInfo fileInfo(filePath);
        prevFileDir = fileInfo.path();

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

    // TODO: remove displayed information when all files are removed
}

void MainWindow::on_pushButton_Refresh_clicked()
{
    // TODO: reread xml file and redisplay information
}

void MainWindow::on_pushButton_Merge_clicked()
{
    // get the specified output directory
    outputFileDir = ui->lineEditFilePath_Output->text();
    outputFileName = ui->lineEditFilePath_OutputName->text() + ".xml";

    // if the directory exists
    if(QDir(outputFileDir).exists()) {

        // if 2 or more files are loaded
        if(files.size() > 1) {

            // append directory with a slash if there isn't one
            if(!outputFileDir.endsWith('/')) outputFileDir.append('/');

            // create a qfile object for the specified output dir and file name
            QFile outputFile(outputFileDir + outputFileName);

            qDebug() << outputFile.fileName();

            // merge the file data
            MergeData merged = mergeFileData(files.values(), mergeTypeButtonGroup.checkedId());

        }
        else {
            QMessageBox::warning(this, "", "Please select 2 or more files to use.");
        }
    }
    else {
        QMessageBox::warning(this, "", "Output directory does not exist.");
    }
}
