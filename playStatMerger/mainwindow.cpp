#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QString filePath = QFileDialog::getOpenFileName(this,
                                                    tr("Open Playback Statistics Export File"),
                                                    QDir::currentPath(),
                                                    tr("Playback File (*.xml)"));
    FileReader fileReader(filePath);
}

MainWindow::~MainWindow()
{
    delete ui;
}
