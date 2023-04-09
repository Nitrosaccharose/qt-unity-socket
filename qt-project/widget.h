#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QChartView>
#include <QLineSeries>
#include <QSplineSeries>
#include <QValueAxis>
#include <QTimer>

QT_CHARTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE
//https://wenku.baidu.com/view/4959451680c4bb4cf7ec4afe04a1b0717ed5b34c.html
//https://blog.csdn.net/weixin_43487953/article/details/107409852
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void Init();
private slots:
    void on_pushButton_Clear_clicked();

    void on_pushButton_StartTimer_clicked();

    void slotTimeout();
private:
    Ui::Widget *ui;
    QValueAxis *m_axisX;
    QValueAxis *m_axisY;

    QValueAxis *m_spaxisX;
    QValueAxis *m_spaxisY;

    QLineSeries *m_series;
    QSplineSeries *m_spSeries;
    QChart *m_chart;
    QChart *m_spchart;
    QTimer *timer;

};
#endif // WIDGET_H
