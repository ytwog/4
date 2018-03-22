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
private slots:

    void on_ExitButton_clicked();

    void on_OpenAction_triggered();

    void on_actionStOpenAction_triggered();

    void showRelations(QModelIndex index);

    void on_swapButton_clicked();

private:
    bool Swapped = false;
    Ui::MainWindow *ui;
    QVector <Data> arrA;
    QVector <Data> arrB;
};

#endif // MAINWINDOW_H
