#ifndef DYNAMICWINDOW_H
#define DYNAMICWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QString>

#include <qcustomplot.h>

QT_BEGIN_NAMESPACE
namespace Ui { class DynamicWindow; }
QT_END_NAMESPACE

class DynamicWindow : public QMainWindow
{
    Q_OBJECT

public:
    DynamicWindow(QWidget *parent = nullptr);
    ~DynamicWindow();

private slots:

    // События панели управления
    void on_B_Begin_clicked();
    void on_B_Clear_clicked();
    void on_B_Itar_clicked();
    void on_B_Itar10_clicked();

    // Событие изминения параметров
    void on_Ed_1_textChanged(const QString &arg1);
    void on_Ed_2_textChanged(const QString &arg1);

    // Событие автомасштаба
    void on_BZ1_clicked();
    void on_BZ2_clicked();
    void on_BZ3_clicked();

private:
    Ui::DynamicWindow *ui;

    // Указатели на графы
    QCustomPlot *GraphDisplay;
    QCustomPlot *GraphFeigenbaum;
    QCustomPlot *GraphFluctuations;

    bool EnablePlot = false;    // Состояние
    double N;                   // Номер итерации

    // Методы для настройки графов
    void GraphDesign(QCustomPlot *G);
    void GraphInteractive(bool X, QCustomPlot *G);

    // Мат. часть
    double Y_N = 0;
    double Y_OLD = 0.2;
    double R_PARAMS = 0.5;
    double My_F(double Y_Old);

    void Lameray_Line();       // Вспомогательная линия
    void Lameray_Parabol();    // Парабола начальных условий на диаграмме Ламерея


};
#endif // DYNAMICWINDOW_H
