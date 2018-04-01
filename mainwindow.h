#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QDialog>
#include <dialog.h>
#include <QFile>
#include <qfiledialog>
#include <QTableWidgetItem>
#include <string.h>
#include <data.h>

namespace Ui {
class MainWindow;
}




class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void updateData();
    void readFile(QString dir = "input.dat");
    bool CheckClones(bool VecNumber, QString _name);
    bool Connect(QString PortName, QString FlightName);
private slots:

    void on_ExitButton_clicked();

    void on_OpenAction_triggered();

    void on_actionStOpenAction_triggered();

    void showRelationsFlights(QModelIndex Qindex);

    void showRelationsPorts(QModelIndex Qindex);

    void on_comboBox_activated(const QString &arg1);

    void UpdateGeometry();

    void on_addButton_clicked();

    void HidePorts(bool value);

    void HideFlights(bool value);

    void on_conButton_clicked();

    void on_disconButton_clicked();

    void on_save_triggered();

private:
    short ToAdd;
    Ui::MainWindow *ui;
    QVector <Airports> VecA;
    QVector <Flights> VecF;
};

#endif // MAINWINDOW_H
