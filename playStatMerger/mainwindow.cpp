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

    // if there is atleast 1 file
    if(fileData.size() >= 1) {

        // copy entries from first file in FileData list
        data.entries = fileData.at(0)->entries;

        // take the first file's version and mapping (assumed to be the same for all files)
        data.versionNumber = fileData.at(0)->versionNumber;
        data.mappingString = fileData.at(0)->mappingString;
    }

    // if there are additional files
    if(fileData.size() > 1) {

        // get the set of entries from the first file
        QMap<QString,EntryStatistics *> merged = data.entries;

        // go through each file
        for(int i = 1; i < fileData.size(); i++) {

            QMap<QString,EntryStatistics *> current = fileData.at(i)->entries;  // get the next file's entries map

            // iterate through each value in the map
            QMap<QString,EntryStatistics *>::iterator entry;
            for(entry = current.begin(); entry != current.end(); entry++) {

                // if the merged list already contains the entry id
                if(merged.contains(entry.key())) {

                    // use mergeType parameter to determine how to combine playcounts
                    switch (mergeType) {
                    case AddPlaycounts: // add radio button
                        merged[entry.key()]->count += entry.value()->count; // add playcount to merged value
                        break;
                    case UseLargest: // largest radio button
                        merged[entry.key()]->count =
                                std::max(merged.value(entry.key())->count,
                                         entry.value()->count);            // take the largest count value
                        break;
                    case UseSmallest: // smallest radio button
                        merged[entry.key()]->count =
                                std::min(merged.value(entry.key())->count,
                                         entry.value()->count);            // take the smallest count value
                        break;
                    default:
                        QMessageBox::information(this,"","No radio button was selected. Defaulting to add playcounts.");
                        merged[entry.key()]->count += entry.value()->count; // add playcount to merged value
                        break;
                    }

                    // rating
                    merged[entry.key()]->rating =
                            std::max(merged.value(entry.key())->rating,
                                     entry.value()->rating);            // take the largest rating value

                    // date added
                    merged[entry.key()]->added =
                            std::min(merged.value(entry.key())->added,
                                     entry.value()->added);             // take the smallest added value

                    // first played
                    merged[entry.key()]->firstPlayed =
                            std::min(merged.value(entry.key())->firstPlayed,
                                     entry.value()->firstPlayed);       // take the smallest firstPlayed value

                    // last played
                    merged[entry.key()]->lastPlayed =
                            std::min(merged.value(entry.key())->lastPlayed,
                                     entry.value()->lastPlayed);        // take the smallest lastPlayed value
                }
                // else this is a new entry
                else {
                    merged[entry.key()] = entry.value();    // add the entire new entry to the merged list
                }
            }

            data.entries = merged;  // move merged entries to the MergeData struct
        }
    }

    // return the MergeData struct
    return data;
}

/*
QMap<QString,EntryStatistics *> MainWindow::addPlayCountEntries(QList<FileData *> fileData) {

    QMap<QString,EntryStatistics *> merged;   // holds the combined values for all lists

    // if there's at least 1 file
    if(fileData.size() >= 1) {
        merged = fileData.at(0)->entries;    // get the set of entries from the first file

        // go through each file
        for(int i = 1; i < fileData.size(); i++) {

            QMap<QString,EntryStatistics *> current = fileData.at(i)->entries;  // get the next file's entries map

            // iterate through each value in the map
            QMap<QString,EntryStatistics *>::iterator entry;
            for(entry = current.begin(); entry != current.end(); entry++) {

                // if the merged list already contains the entry id
                if(merged.contains(entry.key())) {
                    merged[entry.key()]->count += entry.value()->count; // add playcount to merged value
                    merged[entry.key()]->rating =
                            std::max(merged.value(entry.key())->rating,
                                     entry.value()->rating);            // take the largest rating value
                    merged[entry.key()]->added =
                            std::min(merged.value(entry.key())->added,
                                     entry.value()->added);             // take the smallest added value
                    merged[entry.key()]->firstPlayed =
                            std::min(merged.value(entry.key())->firstPlayed,
                                     entry.value()->firstPlayed);       // take the smallest firstPlayed value
                    merged[entry.key()]->lastPlayed =
                            std::min(merged.value(entry.key())->lastPlayed,
                                     entry.value()->lastPlayed);        // take the smallest lastPlayed value
                }
                // else this is a new entry
                else {
                    merged[entry.key()] = entry.value();    // add the entire new entry to the merged list
                }
            }
        }
    }

    return merged;
}
*/

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

/**
 * @brief MainWindow::calculateStdev
 * @param values
 * @param average
 * @return return a calculated standard deviation for the listed values and the provided average
 */
qreal MainWindow::calculateStdev(QList<qreal> values, qreal average) {

    qreal varSum = 0;

    // calculate deviation of each value from the mean
    foreach(int i, values) {
        varSum += qPow(i - average, 2);
    }

    // return standard deviation
    return qSqrt(varSum / values.count());
}

/**
 * @brief MainWindow::compareFileVersionAndMapping
 * @param QList<FileData *> fileData
 * @return bool value for whether or not the files in the list should be merged
 */
bool MainWindow::compareFileVersionAndMapping(QList<FileData *> fileData) {

    // check if there is more than 1 file
    if(fileData.size() > 1) {

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

            // create a readable error string to tell to the user what isn't the same across the files
            QString errorString = "The " +
                    QString(sameVersion ? "" : "version") +                         // versions aren't the same
                    QString(sameVersion || sameMapping ? "" : " and ") +            // both aren't the same
                    QString(sameMapping ? "" : "mapping") +                         // mappings aren't the same
                    " values for some of the selected files are not equal.\nContinue?";

            // display dialog asking the user how to continue
            return promptUserToContinue(errorString);

        }
        else {  // return true if the version numbers and mappings are all the same
            return true;
        }
    }
    else {  // return true if there is only one file
        return true;
    }
}

bool MainWindow::checkFileExistence(QString outputLocation)
{
    // TODO: function to check if file at location exists
    QFile file(outputLocation);

    if(file.exists()) {
        return true;
    }

    return false;
}

bool MainWindow::promptUserToContinue(QString errorString, QMessageBox::StandardButton defaultButton) {
    // display dialog asking the user how to continue
    QMessageBox::StandardButton clicked =
            QMessageBox::question(this, "", errorString,
                                  QMessageBox::Ok | QMessageBox::Cancel,
                                  defaultButton);

    // return true or false depending on what button was pressed
    switch(clicked) {
    case QMessageBox::Ok:       // true if ok
        return true;
    case QMessageBox::Cancel:   // false if cancel or other
    default:
        return false;
    };
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

            // if these files can or are allowed by the user to be merged
            // compared the version number and mapping code
            if(compareFileVersionAndMapping(files.values())) {
                // append directory with a slash if there isn't one
                if(!outputFileDir.endsWith('/'))
                    outputFileDir.append('/');

                // create a qfile object for the specified output dir and file name
                QString outputLocation = outputFileDir + outputFileName;

                // merge the file data
                MergeData mergedData = mergeFileData(files.values(), mergeTypeButtonGroup.checkedId());

                // write data to file using FileWriter object
                // if output location file already exists, ask user to continue
                if(!checkFileExistence(outputLocation) || promptUserToContinue("File already exists. Overwrite?"))
                    FileWriter writer(outputLocation, mergedData);
            }
            else {
                QMessageBox::information(this, "", "Merge cancelled.");
            }

        }
        else {
            QMessageBox::warning(this, "", "Please select 2 or more files to use.");
        }
    }
    else {
        QMessageBox::warning(this, "", "Output directory does not exist.");
    }
}
