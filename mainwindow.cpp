#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    arrA.clear();
    arrB.clear();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_ExitButton_clicked()
{
    Dialog *DialogWindow= new Dialog();
    DialogWindow->exec();

    ui->errBrowser->setText("");
    if(DialogWindow->GetClose())
    {
        this->close();
    }
}

void MainWindow::on_OpenAction_triggered()
{
    QString dir = QFileDialog::getOpenFileName();
    readFile(dir);
}

void MainWindow::readFile(QString dir)
{
    QString res = "";
    QString name = "";
    arrA.clear();
    arrB.clear();
    Swapped = false;
    QSet <QString> Settmp;
    QFile file(dir);
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream inputStream(&file);

        bool Rel = false;
        bool Step = false;

        ui->errBrowser->setText("");
        while(!inputStream.atEnd())
        {
            res = inputStream.readLine();
            if(res == ",;")
            {
                if(Step)
                {
                    ui->errBrowser->setText("Неверный\nформат\nфайла");
                    return;
                }
                Step = true;
            }
            else if(res == ".;")
            {
                if(Rel)
                {
                    Rel = false;
                    Data *Obj = new Data(name, Settmp);
                    name = "";
                    Settmp.clear();
                    if(Step)
                    {
                        arrB.push_back(*Obj);
                    }
                    else
                    {
                        arrA.push_back(*Obj);
                    }
                }
                else
                {
                    ui->errBrowser->setText("Неверный\nформат\nфайла");
                    return;
                }
            }
            else
            {
                if(Rel)
                {
                    Settmp.insert(res);
                }
                else
                {
                    name = res;
                    Rel = true;
                }
            }
        }
        if(inputStream.status() != QTextStream::Ok)
        {
            ui->errBrowser->setText("Ошибка\nчтения\nфайла");
        }
        updateData();
    }
    else
    {
        ui->errBrowser->setText("Ошибка\nчтения\nфайла");
    }
}



void MainWindow::updateData()
{
    ui->firstList->clear();
    ui->secondList->clear();
    if(((!Swapped) &&(arrA.empty()) || ((Swapped) && (arrB.empty()))))
    {
        return;
    }
    if(!Swapped)
    {
        int RowNumb = 0;
        ui->firstList->insertColumn(4);
        for(QVector<Data>::iterator it = arrA.begin(); it != arrA.end(); it++)
        {
            ui->firstList->insertRow(1);
            RowNumb++;
            QTableWidgetItem Item = QTableWidgetItem(it->getName());
            ui->firstList->setItem(RowNumb-1, 0, &Item);
            QObject::connect(ui->firstList, SIGNAL(doubleClicked( QModelIndex )), this, SLOT(showRelations( QModelIndex )));
        }
    }
    else
    {
        for(QVector<Data>::iterator it = arrB.begin(); it != arrB.end(); it++)
        {
            //ui->firstList->addItem(it->getName());
            QObject::connect(ui->firstList, SIGNAL(doubleClicked( QModelIndex )), this, SLOT(showRelations( QModelIndex )));
        }
    }
}

void MainWindow::showRelations(QModelIndex Qindex)
{
    int row = Qindex.row();
    ui->secondList->clear();
    Data Obj;
    if(Swapped)
    {
        Obj = arrB.at(Qindex.row());
    }
    else
    {
        Obj = arrA.at(Qindex.row());
    }
    QSet <QString> tmp = Obj.getCon();
    for(QSet<QString>::Iterator it = tmp.begin(); it != tmp.end(); it++)
    {
        //QObject::connect(ui->firstList, SIGNAL(doubleClicked( QModelIndex )), this, SLOT(showRelations( QModelIndex )));
        ui->secondList->addItem(*it);
    }
}

void MainWindow::on_actionStOpenAction_triggered()
{
    readFile();
}

void MainWindow::on_swapButton_clicked()
{
    Swapped = !Swapped;
    updateData();
}
