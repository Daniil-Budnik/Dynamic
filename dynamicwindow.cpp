#include "dynamicwindow.h"
#include "ui_dynamicwindow.h"

DynamicWindow::DynamicWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::DynamicWindow)
{
    ui->setupUi(this);

    // Указатели на графы
    GraphDisplay = ui->G1;
    GraphFeigenbaum = ui->G2;
    GraphFluctuations = ui->G3;

    // Включаем интерактивные режимы
    GraphInteractive(true, GraphDisplay);
    GraphInteractive(true, GraphFeigenbaum);
    GraphInteractive(true, GraphFluctuations);

    // Настраиваем цветовую палетру
    GraphDesign(GraphDisplay);
    GraphDesign(GraphFeigenbaum);
    GraphDesign(GraphFluctuations);

    // Задаем масштаб и обновляем графы
    on_B_Clear_clicked();

}

DynamicWindow::~DynamicWindow(){ delete ui; }


// Интерактив графиков (позволяет мышкой изменять масштаб и.т.д.)
void DynamicWindow::GraphInteractive(bool X, QCustomPlot *G){
    if(X){
        G->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                               QCP::iSelectLegend | QCP::iSelectPlottables);
    }else{
        G->setInteractions(QCP::iSelectLegend);
    }
}

// Стиль графиков
void DynamicWindow::GraphDesign(QCustomPlot *G)
{
    QLinearGradient plotGradient;
    plotGradient.setStart(0, 0);
    plotGradient.setFinalStop(0, 350);
    plotGradient.setColorAt(0, QColor(255, 255, 255));
    plotGradient.setColorAt(1, QColor(240, 240, 240));

    QLinearGradient axisRectGradient;
    axisRectGradient.setStart(0, 0);
    axisRectGradient.setFinalStop(0, 350);
    axisRectGradient.setColorAt(0, QColor(245, 245, 245));
    axisRectGradient.setColorAt(1, QColor(255, 255, 255));

    G->xAxis->grid()->setSubGridVisible(true);
    G->yAxis->grid()->setSubGridVisible(true);
    G->xAxis->grid()->setZeroLinePen(Qt::NoPen);
    G->yAxis->grid()->setZeroLinePen(Qt::NoPen);
    G->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    G->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    G->setBackground(plotGradient);
    G->axisRect()->setBackground(axisRectGradient);
    G->rescaleAxes();

    G->xAxis->setTickLabelFont(QFont(QFont().family(), 12));
    G->yAxis->setTickLabelFont(QFont(QFont().family(), 12));
}

// Начать исследование
void DynamicWindow::on_B_Begin_clicked(){

    Y_OLD = ui->Ed_1->text().toDouble();    // Начальное условие
    R_PARAMS = ui->Ed_2->text().toDouble(); // Параметр r

    on_B_Clear_clicked();                   // Очищаем графы в начальный момент
    Lameray_Parabol();                      // Генерируем параболу начальных условий
    Lameray_Line();                         // Генерируем вспомогательную линию

    // Первая точка (начальное условие)
    GraphDisplay->addGraph();
    GraphFluctuations->graph(0)->addData(N++, Y_OLD);

    // Обновляем графы
    GraphDisplay->replot();
    GraphFeigenbaum->replot();
    GraphFluctuations->replot();

    EnablePlot = true;                      // Для проверка включения
}

// Очистить графики
void DynamicWindow::on_B_Clear_clicked(){

    EnablePlot = false;                     // Для проверка включения
    N = 0;                                  // Номер итерации
    Y_N = 0;

    // Обнуление графов
    GraphDisplay->clearGraphs();
    GraphFeigenbaum->clearGraphs();
    GraphFluctuations->clearGraphs();

    // Генерируем графы
    GraphDisplay->addGraph();
    GraphDisplay->addGraph();
    GraphDisplay->addGraph();
    GraphFeigenbaum->addGraph();
    GraphFluctuations->addGraph();

    // Настройка цветов
    GraphDisplay->graph(0)->setPen(QPen(Qt::blue));
    GraphDisplay->graph(1)->setPen(QPen(Qt::red));
    GraphDisplay->graph(2)->setPen(QPen(Qt::green));
    GraphFeigenbaum->graph(0)->setPen(QPen(Qt::red));
    GraphFluctuations->graph(0) ->setPen(QPen(Qt::red));


    // Подписываем оси координат                    !!!!!!!!!!!!!!!!!!! ДОРАБОТАТЬ !!!!!!!!!!!!!!!!!!!
    GraphDisplay->xAxis->setLabel("X(n)");
    GraphDisplay->yAxis->setLabel("X(n+1)");
    GraphFeigenbaum->xAxis->setLabel("");
    GraphFeigenbaum->yAxis->setLabel("");
    GraphFluctuations->xAxis->setLabel("n - номер итерации");
    GraphFluctuations->yAxis->setLabel("X");

    // Задаем масштаб и обновляем графы
    on_BZ1_clicked(); on_BZ2_clicked(); on_BZ3_clicked();
}

// Произвести итерацию
void DynamicWindow::on_B_Itar_clicked()
{
    // Проверка включения
    if(EnablePlot){

        GraphDisplay->addGraph();                                               // Новая ломанная линия
        GraphDisplay->graph(2+N)->setPen(QPen(Qt::blue));                       // Задали цвет линии
        GraphDisplay->graph(1+N)->addData(Y_OLD, My_F(Y_OLD));                  // Точка Ламерей Xn и Xn+1 (1)
        GraphDisplay->graph(2+N)->addData(Y_OLD,My_F(Y_OLD));                   // Точка Ламерей Xn и Xn+1 (2)
        Y_OLD = My_F(Y_OLD);                                                    // Находим Xn+1
        Y_N = Y_OLD;                                                            // Запоминаем Xn
        GraphDisplay->graph(2+N)->addData(Y_OLD,Y_OLD);                         // Точка Ламерей Xn+1 и Xn+1
        GraphFluctuations->graph(0)->addData(N++, Y_OLD);                       // Номер итерации и Xn+1

        // Обновляем графы
        GraphDisplay->replot();
        GraphFluctuations->replot();
    }
}

// Произвести 10 итераций
void DynamicWindow::on_B_Itar10_clicked(){ for(int i = 0; i < 10; i++) on_B_Itar_clicked(); }


// Уравнение                    !!!!!!!!!!!!!!!!!!! ФУНКЦИЯ ОПИСАНА ТУТ  !!!!!!!!!!!!!!!!!!!
double DynamicWindow::My_F(double Y_Old){ return R_PARAMS * Y_Old * ( 1 - Y_Old ); }

// Генерируем вспомогательную линию
void DynamicWindow::Lameray_Line(){
    GraphDisplay->graph(1)->addData(0, 0);
    GraphDisplay->graph(1)->addData(1, 1);
    GraphDisplay->replot();
}

// Генерируем параболу начальных условий
void DynamicWindow::Lameray_Parabol(){
    for(int I = 0; I < 1000; I++) {
        GraphDisplay->graph(2)->addData((double) I/1000, My_F((double) I/1000));
    }
    GraphDisplay->replot();
}

// Параметры системы
void DynamicWindow::on_Ed_1_textChanged(const QString &arg1){ Y_OLD = arg1.toDouble(); }
void DynamicWindow::on_Ed_2_textChanged(const QString &arg1){ R_PARAMS = arg1.toDouble(); }

// Масштабирование первого графа
void DynamicWindow::on_BZ1_clicked(){
    GraphDisplay->xAxis->setRange(0, 1);
    GraphDisplay->yAxis->setRange(0, 1);
    GraphDisplay->replot();
}

// Масштабирование второго графа
void DynamicWindow::on_BZ2_clicked(){
    GraphFeigenbaum->xAxis->setRange(0, 100);
    GraphFeigenbaum->yAxis->setRange(0, 100);
    GraphFeigenbaum->replot();
}

// Масштабирование третьего графа
void DynamicWindow::on_BZ3_clicked(){
    GraphFluctuations->xAxis->setRange(0, 100);
    GraphFluctuations->yAxis->setRange(0, 1);
    GraphFluctuations->replot();
}
