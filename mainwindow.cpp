#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    VecA.clear();
    VecF.clear();
    HidePorts(true);
    HideFlights(true);
    ui->addButton->setHidden(true);
    ToAdd = 0;
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

bool MainWindow::Connect(QString PortName, QString FlightName)
{
    int IndFlight;
    int res = 0;
    for(int i = 0; i < VecA.size(); i++)
    {
        if(VecA.at(i).getName() == PortName)
        {
            IndFlight = i;
            res++;
        }
    }
    int IndAirport;
    for(int i = 0; i < VecF.size(); i++)
    {
        if(VecF.at(i).getName() == FlightName)
        {
            IndAirport = i;
            res++;
        }
    }
    if(res != 2)
    {
        return false;
    }
    QVector<Airports>::iterator itA = VecA.begin() + IndFlight;
    itA->connect(FlightName);
    QVector<Flights>::iterator itF = VecF.begin() + IndAirport;
    itF->connect(PortName);
    return true;
}

bool MainWindow::CheckClones(bool VecNumber, QString _name)
{
    if(VecNumber)
    {
        foreach (Airports Port, VecA)
        {
            if(Port.getName() == _name)
            {
                return false;
            }
        }
    }
    else
    {
        foreach (Flights Flight, VecF)
        {
            if(Flight.getName() == _name)
            {
                return false;
            }
        }
    }
    return true;
}

void MainWindow::readFile(QString dir)
{
    QString res = "";
    QString name = "";
    VecA.clear();
    VecF.clear();
    QSet <QString> Settmp;
    QFile file(dir);
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream inputStream(&file);

        ui->errBrowser->setText("");
        bool ReadAirport = false;
        bool ReadFlight = false;
        bool ReadConnect = false;
        bool ReadContinue = false;

        short StepAirport = 0;
        short StepFlight = 0;
        short StepConnects = 0;

        QString AirportName = "";
        QString AirportCity = "";

        QString FlightName = "";
        QTime LandTime;
        QTime TakeOffTime;
        QString FlightModel = "";

        QString ConnectionPort = "";

        while(!inputStream.atEnd())
        {
            res = inputStream.readLine();
            if(ReadContinue)
            {
                if(ReadAirport)
                {
                    if(StepAirport == 0)
                    {
                        AirportName = res;
                        StepAirport++;
                    }
                    else if(StepAirport == 1)
                    {
                        AirportCity = res;
                        StepAirport++;
                        if(CheckClones(0, res))
                        {
                            Airports *Port = new Airports();
                            Port->setName(AirportName);
                            Port->setCity(AirportCity);
                            VecA.push_back(*Port);
                        }
                        ReadContinue = false;
                        StepAirport = 0;
                    }
                }
                else if(ReadFlight)
                {
                    if(StepFlight == 0)
                    {
                        FlightName = res;
                        StepFlight++;
                    }
                    else if(StepFlight == 1)
                    {
                        if(CheckTime(res))
                        {
                            TakeOffTime = QTime::fromString(res, "hh:mm");
                        }
                        else
                        {
                            ui->errBrowser->setText("Ошибка\nполучения\nвремени");
                            return;
                        }
                        StepFlight++;
                    }
                    else if(StepFlight == 2)
                    {
                        if(CheckTime(res))
                        {
                            LandTime = QTime::fromString(res, "hh:mm");
                        }
                        else
                        {
                            ui->errBrowser->setText("Ошибка\nполучения\nвремени");
                            return;
                        }
                        StepFlight++;
                    }
                    else if(StepFlight == 3)
                    {
                        FlightModel = res;
                        StepFlight++;
                        if(CheckClones(0, res))
                        {
                            Flights *Flight = new Flights();
                            Flight->setName(FlightName);
                            Flight->setTakeOffTime(TakeOffTime);
                            Flight->setLandTime(LandTime);
                            Flight->setModelType(FlightModel);
                            VecF.push_back(*Flight);
                        }
                        ReadContinue = false;
                        StepFlight = 0;
                    }
                }
                else if(ReadConnect)
                {
                    if(StepConnects == 0)
                    {
                        ConnectionPort = res;
                        StepConnects++;
                    }
                    else
                    {
                        if(res == ">airports")
                        {
                            ReadAirport = true;
                            ReadFlight = false;
                            ReadConnect = false;
                            ReadContinue = true;
                            StepAirport = 0;
                        }
                        else if(res == ">flights")
                        {
                            ReadAirport = false;
                            ReadFlight = true;
                            ReadConnect = false;
                            ReadContinue = true;
                            StepFlight = 0;
                        }
                        else if(res == ">connections")
                        {
                            ReadAirport = false;
                            ReadFlight = false;
                            ReadConnect = true;
                            ReadContinue = true;
                            StepConnects = 0;
                        }
                        else if(res == "")
                        {
                            ConnectionPort = "";
                            StepConnects = 0;
                        }
                        else
                        {
                            if(!Connect(ConnectionPort, res))
                            {
                                ui->errBrowser->setText("Ошибка\nполучения\nсвязей");
                                return;
                            }
                        }
                    }
                }
            }
            else
            {
                if(res == ">airports")
                {
                    ReadAirport = true;
                    ReadFlight = false;
                    ReadConnect = false;
                    ReadContinue = true;
                    StepAirport = 0;
                }
                else if(res == ">flights")
                {
                    ReadAirport = false;
                    ReadFlight = true;
                    ReadConnect = false;
                    ReadContinue = true;
                    StepFlight = 0;
                }
                else if(res == ">connections")
                {
                    ReadAirport = false;
                    ReadFlight = false;
                    ReadConnect = true;
                    ReadContinue = true;
                    StepConnects = 0;
                }
                else if(res == "")
                {
                    ReadContinue = true;
                    StepAirport = 0;
                    StepFlight = 0;
                    StepConnects = 0;
                }
                else
                {
                    ui->errBrowser->setText("Ошибка\nчтения\nфайла");
                    return;
                }
            }
        }
        if(inputStream.status() != QTextStream::Ok)
        {
            ui->errBrowser->setText("Ошибка\nоткрытия\nфайла");
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
    ui->PortsWidget->clear();
    ui->FlightWidget->clear();
    ui->firstList->setRowCount(0);
    ui->firstList->setColumnCount(0);

    if(VecA.empty())
    {
        return;
    }

    for(QVector<Airports>::iterator it = VecA.begin(); it != VecA.end(); it++)
    {
        QString Name = it->getName();
        ui->PortsWidget->addItem(Name);
        QObject::connect(ui->PortsWidget, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(showRelationsPorts(QModelIndex)));
    }
    for(QVector<Flights>::iterator it = VecF.begin(); it != VecF.end(); it++)
    {
        QString Name = it->getName();
        ui->FlightWidget->addItem(Name);
        QObject::connect(ui->FlightWidget, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(showRelationsFlights(QModelIndex)));
    }

}

void MainWindow::showRelationsFlights(QModelIndex Qindex)
{
    ui->firstList->setRowCount(0);
    int row = Qindex.row();
    QSet <QString> tmp;
    ui->firstList->setColumnCount(2);
    QStringList list;
    list << "Аэропорт" << "Город";
    ui->firstList->setHorizontalHeaderLabels(list);
    ui->firstList->setColumnWidth(0, (ui->firstList->width() - qApp->style()->pixelMetric(QStyle::PM_ScrollBarExtent)) / 2);
    Flights Obj = VecF.at(row);
    tmp = Obj.getConnections();
    for(QSet<QString>::Iterator it = tmp.begin(); it != tmp.end(); it++)
    {
        ui->firstList->insertRow(ui->firstList->rowCount());
        ui->firstList->setItem(ui->firstList->rowCount() - 1, 0, new QTableWidgetItem(*it));
        int IndAirport = 0;
        for(int i = 0; i < VecA.size(); i++)
        {
            if(VecA.at(i).getName() == *it)
            {
                IndAirport = i;
            }
        }
        ui->firstList->setItem(ui->firstList->rowCount() - 1, 1, new QTableWidgetItem(VecA.at(IndAirport).getCity()));
        //QObject::connect(ui->firstList, SIGNAL(doubleClicked( QModelIndex )), this, SLOT(showRelations( QModelIndex )));
    }
    ui->firstList->horizontalHeader()->setStretchLastSection(1);
}

void MainWindow::showRelationsPorts(QModelIndex Qindex)
{
    ui->errBrowser->setText("");
    ui->firstList->setRowCount(0);
    int row = Qindex.row();
    QSet <QString> tmp;
    ui->firstList->setColumnCount(4);
    QStringList list;
    list << "Рейс" << "Отбытие" << "Прибытие" << "Модель";
    ui->firstList->setHorizontalHeaderLabels(list);
    ui->firstList->setColumnWidth(0, (ui->firstList->width() - qApp->style()->pixelMetric(QStyle::PM_ScrollBarExtent)) / 4);
    ui->firstList->setColumnWidth(1, (ui->firstList->width() - qApp->style()->pixelMetric(QStyle::PM_ScrollBarExtent)) / 5);
    ui->firstList->setColumnWidth(2, (ui->firstList->width() - qApp->style()->pixelMetric(QStyle::PM_ScrollBarExtent)) / 5);
    Airports Obj = VecA.at(row);
    tmp = Obj.getConnections();
    for(QSet<QString>::Iterator it = tmp.begin(); it != tmp.end(); it++)
    {
        ui->firstList->insertRow(ui->firstList->rowCount());
        ui->firstList->setItem(ui->firstList->rowCount() - 1, 0, new QTableWidgetItem(*it));
        int IndFlight = 0;
        for(int i = 0; i < VecF.size(); i++)
        {
            if(VecF.at(i).getName() == *it)
            {
                IndFlight = i;
            }
        }
        ui->firstList->setItem(ui->firstList->rowCount() - 1, 1, new QTableWidgetItem(VecF.at(IndFlight).getTakeOffTime().toString("hh:mm")));
        ui->firstList->setItem(ui->firstList->rowCount() - 1, 2, new QTableWidgetItem(VecF.at(IndFlight).getLandTime().toString("hh:mm")));
        ui->firstList->setItem(ui->firstList->rowCount() - 1, 3, new QTableWidgetItem(VecF.at(IndFlight).getModelType()));
        //QObject::connect(ui->firstList, SIGNAL(doubleClicked( QModelIndex )), this, SLOT(showRelations( QModelIndex )));
    }
    ui->firstList->horizontalHeader()->setStretchLastSection(1);
}

void MainWindow::on_actionStOpenAction_triggered()
{
    ui->errBrowser->setText("");
    readFile();
}

void MainWindow::HidePorts(bool value)
{
    ui->PortNameLabel->setHidden(value);
    ui->PortCityLabel->setHidden(value);
    ui->PortNameEdit->setHidden(value);
    ui->PortCityEdit->setHidden(value);
}

void MainWindow::HideFlights(bool value)
{
    ui->PortNameLabel->setHidden(value);
    ui->PortNameEdit->setHidden(value);
    ui->FlightTakeOffLabel->setHidden(value);
    ui->FlightLandLabel->setHidden(value);
    ui->FlightModelLabel->setHidden(value);
    ui->FlightModelEdit->setHidden(value);
    ui->TakeOffEdit->setHidden(value);
    ui->LandEdit->setHidden(value);
}

void MainWindow::UpdateGeometry()
{
    if(ToAdd == 1)
    {
        HideFlights(true);
        HidePorts(false);
        ui->addButton->setHidden(false);
    }
    else if(ToAdd == 2)
    {
        HidePorts(true);
        HideFlights(false);
        ui->addButton->setHidden(false);
    }
    else
    {
        HidePorts(true);
        HideFlights(true);
        ui->addButton->setHidden(true);
    }
}

void MainWindow::on_comboBox_activated(const QString &arg1)
{
    ui->errBrowser->setText("");
    if(arg1 == "Аэропорт")
    {
        ToAdd = 1;
    }
    else if(arg1 == "Рейс")
    {
        ToAdd = 2;
    }
    else
    {
        ToAdd = 0;
    }
    UpdateGeometry();
}

void MainWindow::on_addButton_clicked()
{
    ui->errBrowser->setText("");
    if(ToAdd == 1)
    {
        QString name = ui->PortNameEdit->text();
        if(CheckClones(1, name))
        {
            if(name.isEmpty())
            {
                ui->errBrowser->setText("Не было\nуказано\nназвание");
                return;
            }
            if(ui->PortCityEdit->text().isEmpty())
            {
                ui->errBrowser->setText("Не был\nуказан\nгород");
                return;
            }
            Airports *port = new Airports;
            port->setName(name);
            port->setCity(ui->PortCityEdit->text());
            VecA.push_back(*port);
        }
        else
        {
            ui->errBrowser->setText("Такой\nаэропорт\nуже есть");
        }
    }
    else if(ToAdd == 2)
    {
        QString name = ui->PortNameEdit->text();
        if(CheckClones(0, name))
        {
            if(name.isEmpty())
            {
                ui->errBrowser->setText("Не было\nуказано\nназвание");
                return;
            }
            if(ui->FlightModelEdit->text().isEmpty())
            {
                ui->errBrowser->setText("Не была\nуказана\nмодель");
                return;
            }
            Flights *flight = new Flights;
            flight->setName(name);
            flight->setTakeOffTime(ui->TakeOffEdit->time());
            flight->setLandTime(ui->LandEdit->time());
            flight->setModelType(ui->FlightModelEdit->text());
            VecF.push_back(*flight);
        }
        else
        {
            ui->errBrowser->setText("Такой\nрейс\nуже есть");
        }
    }
    updateData();
}

void MainWindow::on_conButton_clicked()
{
    ui->errBrowser->setText("");
    if(!Connect(ui->ConPortEdit->text(), ui->ConFlightEdit->text()))
    {
        ui->errBrowser->setText("Не удалось\nнайти\nэлементы");
    }
    updateData();
}

void MainWindow::on_disconButton_clicked()
{
    ui->errBrowser->setText("");
    int IndFlight;
    QString PortName = ui->ConPortEdit->text();
    QString FlightName = ui->ConFlightEdit->text();
    int res = 0;
    for(int i = 0; i < VecA.size(); i++)
    {
        if(VecA.at(i).getName() == PortName)
        {
            IndFlight = i;
            res++;
        }
    }
    int IndAirport;
    for(int i = 0; i < VecF.size(); i++)
    {
        if(VecF.at(i).getName() == FlightName)
        {
            IndAirport = i;
            res++;
        }
    }
    if(res != 2)
    {
        ui->errBrowser->setText("Не удалось\nнайти\nэлементы");
        return;
    }
    QVector<Airports>::iterator itA = VecA.begin() + IndFlight;
    itA->disconnect(FlightName);
    QVector<Flights>::iterator itF = VecF.begin() + IndAirport;
    itF->disconnect(PortName);
    updateData();
}

void MainWindow::on_save_triggered()
{
    //QString dir = QFileDialog::getOpenFileName();
    FILE *fout = fopen("output.txt", "w");
    if(fout)
    {
        fprintf(fout, ">airports\n");
        for(int i = 0; i < VecA.size(); i++)
        {
            string Name = VecA.at(i).getName().toStdString();
            string City = VecA.at(i).getCity().toStdString();
            fprintf(fout, "%s\n%s\n", Name.c_str(), City.c_str());
            if(i < VecA.size() - 1)
            {
                fprintf(fout, "\n");
            }
        }
        fprintf(fout, ">flights\n");
        for(int i = 0; i < VecF.size(); i++)
        {
            fprintf(fout, "%s\n%s\n%s\n%s\n", VecF.at(i).getName().toStdString().c_str(),
                                      VecF.at(i).getTakeOffTime().toString("hh:mm").toStdString().c_str(),
                                      VecF.at(i).getLandTime().toString("hh:mm").toStdString().c_str(),
                                      VecF.at(i).getModelType().toStdString().c_str());
            if(i < VecF.size() - 1)
            {
                fprintf(fout, "\n");
            }
        }
        fprintf(fout, ">connections\n");
        for(int i = 0; i < VecA.size(); i++)
        {
            fprintf(fout, "%s\n", VecA.at(i).getName().toStdString().c_str());
            foreach (QString FlightName, VecA.at(i).getConnections())
            {

                fprintf(fout, "%s\n", (FlightName.toStdString().c_str()));
            }
            if(i < VecA.size() - 1)
            {
                fprintf(fout, "\n");
            }
        }
        fclose(fout);
    }
}
