#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QTimer>
#include <QtCharts/QValueAxis>

extern void scheduler_background_process(QString dbname, QString hostname, QString username, QString password)
{
    QSqlDatabase handle = QSqlDatabase::addDatabase("QMYSQL");
    handle.setDatabaseName(dbname);
    handle.setHostName(hostname);
    handle.setUserName(username);
    handle.setPassword(password);

    if(!handle.open()) {
        qDebug() << "scheduler_background_process connect database error";
    }

    while(1) {
        // 1. query from database
        // 2. find cron
        // 3. generate command and write to task table
    }
}

static void addStyleSheet()
{
    QFile f(":qdarkstyle/style.qss");
    if (!f.exists())
    {
        printf("Unable to set stylesheet, file not found\n");
    }
    else
    {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        qApp->setStyleSheet(ts.readAll());
    }
}

void MainWindow::init()
{
    addStyleSheet();
    this->window()->showFullScreen();
    ui->tabWidget->setCurrentIndex(0);
    ui->tabWidget->tabBar()->hide();
}


bool MainWindow::connect_database(QString dbname, QString hostname, QString username, QString password)
{
    dbHandle = QSqlDatabase::addDatabase("QMYSQL");
    dbHandle.setDatabaseName(dbname);
    dbHandle.setHostName(hostname);
    dbHandle.setUserName(username);
    dbHandle.setPassword(password);

    if(!dbHandle.open()) {
        qDebug() << "func=connDatabase,hostname=" << hostname << ",username="
                 << username << ",err=" << dbHandle.lastError().text();
        return false;
    }

    return true;
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonExit_clicked()
{
    qApp->closeAllWindows();
}

void MainWindow::on_pushButtonLogin_clicked()
{
    // 1. check if user exist, if not Just clean all fields and give warnings.
    ui->tabWidget->tabBar()->show();
    QString username = ui->lineEditUsername->text().trimmed();
    QString password = ui->lineEditPassword->text().trimmed();
    QString database = ui->lineEditDatabase->text().trimmed();
    QString hostname = ui->lineEditHostname->text().trimmed();

    connect_database(database, hostname, username, password);

    ui->tabWidget->removeTab(0);

    QTimer *timer = new QTimer(this);
    model = new QSqlTableModel(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(show_current_tasks()));
    timer->start(5000);

    chart = new QtCharts::QChart();
    chart->setMargins(QMargins(0, 0, 0, 0));
    series = new QtCharts::QLineSeries();

    for(int i = 0; i < 24; i++) {
        series->append(i, qrand() % 20);
    }

    chart->addSeries(series);
    chart->setAnimationOptions(QtCharts::QChart::SeriesAnimations);
    chart->legend()->hide();
    chart->createDefaultAxes();
    chart->setTheme(QtCharts::QChart::ChartThemeDark);

    QtCharts::QValueAxis *axisX = new QtCharts::QValueAxis;
    axisX->setRange(0, 23);
    axisX->setTickCount(24);
    axisX->setMinorTickCount(0);

    QtCharts::QValueAxis *axisY = new QtCharts::QValueAxis;
    axisY->setRange(0, 100);
    axisY->setTickCount(11);
    axisY->setMinorTickCount(0);

    chart->setAxisX(axisX, series);
    chart->setAxisY(axisY, series);

    ui->graphicsView->setChart(chart);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setWindowTitle("current");
    ui->graphicsView->show();

    QTimer *timerChart = new QTimer(this);
    connect(timerChart, SIGNAL(timeout()), this, SLOT(showMonitorChart()));
    timerChart->start(60000);

    model->setTable("tb_task");
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    model->setSort(0, Qt::DescendingOrder);
    model->select();
    ui->tableViewMonitor->setModel(model);
}

void MainWindow::show_current_tasks()
{
    model->setTable("tb_task");
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    model->setSort(0, Qt::DescendingOrder);
    model->select();
    ui->tableViewMonitor->setModel(model);
}

void MainWindow::showMonitorChart()
{
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    series->clear();
    for(int i = 0; i < 30; i++) {
        series->append(i, qrand() % 20);
    }
    ui->graphicsView->show();
}
