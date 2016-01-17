#include "MainWindow.h"

#define STEP_SIZE 50


MainWindow::MainWindow(QWidget* parent)
    :QMainWindow(parent)
{
    GUI();
    Buttons();
    outGraph();
}

MainWindow::~MainWindow()
{

}

void MainWindow::GUI()
{
    m_layout = new QGridLayout();
    m_widget = new QWidget();
    m_pb_ok = new QPushButton();
    m_lb_countTops = new QLabel("Введите максимальное количество вершин");
    m_lb_density = new QLabel("Введите плотность графа");
    m_cb_floid = new QCheckBox("алгоритм Флоида-Уоршелла");
    m_cb_random = new QCheckBox("Зарандомить матрицу смежностей?");
    m_cb_floid->setChecked(true);
    m_cb_random->setChecked(true);
    m_le_countTops = new QLineEdit("5");
    m_le_density = new QLineEdit("0.2");
    m_tw_matrix = new QTableWidget();
    m_lb_resultOut = new QLabel();
    cPlot = new QCustomPlot();
    m_math = new mathGraph();

    m_layout->addWidget(m_tw_matrix, 0, 0, 5, 5);
    m_layout->addWidget(m_lb_countTops, 6, 0, 1, 1);
    m_layout->addWidget(m_le_countTops, 6, 1, 1, 1);
    m_layout->addWidget(m_lb_density, 7, 0, 1, 1);
    m_layout->addWidget(m_le_density, 7, 1, 1, 1);
    m_layout->addWidget(m_cb_floid, 8, 0, 1, 1);
    m_layout->addWidget(m_cb_random, 9, 0, 1, 1);
    m_layout->addWidget(m_pb_ok, 10, 0, 1, 1);
    m_layout->addWidget(m_lb_resultOut, 11, 0, 1, 1);
    m_layout->addWidget(cPlot, 12, 0, 5, 5);

    m_widget->setLayout(m_layout);
    this->setCentralWidget(m_widget);

}

void MainWindow::Buttons()
{
    connect(m_pb_ok, SIGNAL(clicked()), this, SLOT(slot_pb_ok()));
    connect(m_le_countTops, SIGNAL(textEdited(QString)), this,SLOT(slot_set_table(QString)));
    connect(m_le_density,SIGNAL(textEdited(QString)), this, SLOT(slot_densityChanged()));
    connect(this, SIGNAL(setMatrix(int,int,int)),m_math, SLOT(set_matrix(int,int,int)));
    connect(this, SIGNAL(calcFloid()), m_math, SLOT(floid()));
    connect(this, SIGNAL(getResults(int)), m_math, SLOT(get_results(int)));
}

void MainWindow::slot_pb_ok()
{
    emit clearMatrix();
    if(m_cb_floid->isChecked()) {
        int countTops = m_le_countTops->text().toInt();
        for(int i = STEP_SIZE; i <= countTops; i+=STEP_SIZE) {
            slot_set_table(QString::number(i));
            slot_LoadData(i);
            calcFloid();
            m_math->greedy();
            steps << (double)i;
        }
        outGraph();
        //m_lb_resultOut->setText(QString::number(result, 'i', 0));
    }
}

void MainWindow::slot_set_table(QString Count)
{   matrixSize = Count.toInt();
    m_tw_matrix->setRowCount(matrixSize);
    m_tw_matrix->setColumnCount(matrixSize);
}


void MainWindow::slot_densityChanged()
{
    if(m_le_density->text().toDouble() > 1)
        m_le_density->setText("1");
}

void MainWindow::slot_LoadData(int curCountTops)
{
    if(m_cb_random->isChecked()) {
        int countTops = curCountTops;
        double density = m_le_density->text().toDouble();
        int countEdges = (countTops*(countTops-1)*density)/2;
        qDebug() << countEdges << countTops << density;

        if(m_cb_random->isChecked()) {
            for (int row = 0; row < matrixSize; row++) {
                for (int col = 0; col < matrixSize; col++) {
                    QTableWidgetItem *newItem = new QTableWidgetItem(0);
                    newItem->setTextAlignment(Qt::AlignCenter);
                    m_tw_matrix->setItem(row, col, newItem);
                    int size = m_tw_matrix->item(row,col)->text().toInt();
                    emit setMatrix(row, col, size);
                }
            }
            while(countEdges) {
                int row;
                int col;
                do {
                    row = qrand() % matrixSize;
                    col = qrand() % matrixSize;
                } while(col == row);


                int  size = m_tw_matrix->item(row,col)->text().toInt();

                if(!size) {
                    QTableWidgetItem *newItem = new QTableWidgetItem(tr("%1").arg(rand()%100));
                    newItem->setTextAlignment(Qt::AlignCenter);
                    m_tw_matrix->setItem(row, col, newItem);
                    size = m_tw_matrix->item(row,col)->text().toInt();
                    emit setMatrix(row, col, size);
                    countEdges--;
                }


            }
        }
        else {
            for (int row = 0; row < matrixSize; row++) {
                for (int col = 0; col < matrixSize; col++) {
                    int size = m_tw_matrix->item(row,col)->text().toInt();
                    emit setMatrix(row, col, size);
                }
            }
        }
    }
}


void MainWindow::outGraph()
{
    QVector<double> results;
    results = getResults(0);
    int max = 0;
    for(int i = 0; i < results.size(); i++) {
        if(results.at(i) > max)
            max = results.at(i);

    }


    cPlot->addGraph(); //будем выводить результат к количеству вершин

    QPen floyd;
    floyd.setColor(QColor(Qt::red));
    //floyd.setStyle(Qt::DotLine);
    cPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::red, Qt::white, 7));
    floyd.setWidthF(5);
    cPlot->graph(0)->setPen(floyd);

    cPlot->graph(0)->setData(steps, results);
    results = getResults(1);
    cPlot->addGraph();

    QPen dijkstra;
    dijkstra.setColor(QColor(30, 40, 255, 150));
    dijkstra.setStyle(Qt::DotLine);
    dijkstra.setWidthF(4);
    cPlot->graph(1)->setPen(dijkstra);
    results = getResults(1);
    cPlot->graph(1)->setData(steps, results);
    cPlot->xAxis->setLabel("Количество вершин");
    cPlot->yAxis->setLabel("Результат");
    cPlot->xAxis->setRange(STEP_SIZE, m_le_countTops->text().toInt());
    cPlot->yAxis->setRange(0, max);
    cPlot->replot();
    cPlot->show();

}
