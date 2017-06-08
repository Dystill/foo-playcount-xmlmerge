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

}

MainWindow::~MainWindow()
{
    delete ui;
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
            files[filePath] = &fileReader;

            // add file to the ui list widget and highlight it
            ui->listWidget->setCurrentItem(new QListWidgetItem(fileReader.getFilePath(), ui->listWidget));

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
        files.remove(ui->listWidget->currentItem()->text());

        // remove from the ui list
        delete ui->listWidget->currentItem();

    }
}
