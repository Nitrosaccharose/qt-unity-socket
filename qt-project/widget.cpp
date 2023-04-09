#include "widget.h"
#include "ui_widget.h"
#include <QBarSeries>
#include <QtDebug>
#define AXIS_MAX_X 30
#define AXIS_MAX_Y 10
static int count = 0;
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    qsrand((uint)QTime::currentTime().msec());
    Init();
    timer = new QTimer(this);
    timer->setInterval(200);

    connect(timer, &QTimer::timeout, this, &Widget::slotTimeout);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::Init()
{
    m_axisX = new QValueAxis();
    m_axisY = new QValueAxis();
    m_axisX->setTitleText("X-label");
    m_axisY->setTitleText("Y-label");
    m_axisX->setMin(0);
    m_axisY->setMin(0);
    m_axisX->setMax(AXIS_MAX_X);
    m_axisY->setMax(AXIS_MAX_Y);

    m_series = new QLineSeries();
    m_series->setPointsVisible(true);
    m_series->setName("随机数折线");


    m_chart = new QChart();
    m_chart->addAxis(m_axisY, Qt::AlignLeft);
    m_chart->addAxis(m_axisX, Qt::AlignBottom);
    m_chart->addSeries(m_series);
    m_chart->setAnimationOptions(QChart::SeriesAnimations);

    m_series->attachAxis(m_axisX);
    m_series->attachAxis(m_axisY);

    ui->graphicsView->setChart(m_chart);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);

    m_spaxisX = new QValueAxis();
    m_spaxisY = new QValueAxis();
    m_spaxisX->setTitleText("X-label");
    m_spaxisY->setTitleText("Y-label");
    m_spaxisX->setMin(0);
    m_spaxisY->setMin(0);
    m_spaxisX->setMax(AXIS_MAX_X);
    m_spaxisY->setMax(AXIS_MAX_Y);

    m_spSeries = new QSplineSeries();
    m_spSeries->setPointsVisible(true);
    m_spSeries->setName("随机数曲线");

    m_spchart = new QChart();
    m_spchart->addAxis(m_spaxisY, Qt::AlignLeft);
    m_spchart->addAxis(m_spaxisX, Qt::AlignBottom);
    m_spchart->addSeries(m_spSeries);
    m_spchart->setAnimationOptions(QChart::SeriesAnimations);//注释调可防止抖动

    m_spSeries->attachAxis(m_spaxisX);
    m_spSeries->attachAxis(m_spaxisY);

    ui->graphicsView_2->setChart(m_spchart);
    ui->graphicsView_2->setRenderHint(QPainter::Antialiasing);
}

void Widget::on_pushButton_Clear_clicked()
{
    timer->stop();
}

void Widget::on_pushButton_StartTimer_clicked()
{
    if (!timer->isActive())
    {
        timer->start();
    }
}

void Widget::slotTimeout()
{
    if (count > AXIS_MAX_X)
    {
        m_series->remove(0);
        m_chart->axes(Qt::Horizontal).first()->setMin(count - AXIS_MAX_X);
        m_chart->axes(Qt::Horizontal).first()->setMax(count);

        m_spSeries->remove(0);
        m_spchart->axes(Qt::Horizontal).first()->setMin(count - AXIS_MAX_X);
        m_spchart->axes(Qt::Horizontal).first()->setMax(count);
    }

    int yValue = qrand()%AXIS_MAX_Y;
    m_series->append(QPointF(count, yValue));
    m_series->setUseOpenGL(true);

    m_spSeries->append(QPointF(count, yValue));
    m_spSeries->setUseOpenGL(true);
    count++;
}
