#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_ExitButton_clicked()
{
    Dialog *DialogWindow= new Dialog();
    DialogWindow->exec();

    if(DialogWindow->GetClose())
    {
        this->close();
    }
}
