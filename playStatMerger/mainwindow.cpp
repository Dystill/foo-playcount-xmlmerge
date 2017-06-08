#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /*
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
        if(!fileReader.hasError()) {    // no error
            files[filePath] = &fileReader;
            new QListWidgetItem(fileReader.getFileName() + " — " + fileReader.getFilePath(), ui->listWidget);
        }
        else {  // some error
            QMessageBox::warning(this,"Error parsing file",fileReader.errorString());
        }

    }
    else {
        QMessageBox::information(this,"","File already added.");
    }
}

void MainWindow::on_pushButton_Remove_clicked()
{
    //ui->listWidget->currentItem()
}
