#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QTcpSocket>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSql>
#include <QSqlError>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QMessageBox>
#include <QDateTime>
#include <QDebug>
#include <QTextStream>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QPointF>
#include <QTime>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void init();
    QSqlDatabase dbHandle;
    QSqlTableModel *model;
    bool connect_database(QString dbname, QString hostname, QString username, QString password);
    int refresh_seconds;
    QtCharts::QChart *chart;
    QtCharts::QLineSeries *series;


private slots:
    void show_current_tasks();
    void showMonitorChart();
    void on_pushButtonExit_clicked();

    void on_pushButtonLogin_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
