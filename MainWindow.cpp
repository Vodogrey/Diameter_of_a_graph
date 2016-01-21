#include "MainWindow.h"

#define STEP_SIZE 1
#define SAMPLE_SIZE 10


MainWindow::MainWindow(QWidget* parent)
    :QMainWindow(parent)
{
    GUI();
    Buttons();
    outGraph();
    matrixSize = 0;
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
    m_cb_random = new QCheckBox("Зарандомить матрицу смежностей?");
    m_cb_random->setChecked(true);
    m_le_countTops = new QLineEdit("5");
    m_le_density = new QLineEdit("0.2");
    m_tw_matrix = new QTableWidget();
    m_lb_resultOut = new QLabel();
    cPlotResults = new QCustomPlot();
    cPlotTime = new QCustomPlot();
    cPlotSolve = new QCustomPlot();
    m_math = new mathGraph();

    m_layout->addWidget(m_tw_matrix, 0, 0, 5, 5);
    m_layout->addWidget(m_lb_countTops, 6, 0, 1, 1);
    m_layout->addWidget(m_le_countTops, 6, 1, 1, 1);
    m_layout->addWidget(m_lb_density, 7, 0, 1, 1);
    m_layout->addWidget(m_le_density, 7, 1, 1, 1);
    m_layout->addWidget(m_cb_random, 9, 0, 1, 1);
    m_layout->addWidget(m_pb_ok, 10, 0, 1, 1);
    m_layout->addWidget(m_lb_resultOut, 11, 0, 1, 1);
    m_layout->addWidget(cPlotResults, 12, 0, 5, 5);
    m_layout->addWidget(cPlotTime, 18, 0, 5, 5);
    m_layout->addWidget(cPlotSolve, 23, 0, 5, 5);

    m_widget->setLayout(m_layout);
    this->setCentralWidget(m_widget);

}

void MainWindow::Buttons()
{
    connect(m_pb_ok, SIGNAL(clicked()), this, SLOT(slot_pb_ok()));
    connect(m_le_countTops, SIGNAL(textEdited(QString)), this,SLOT(slot_set_table(QString)));
    connect(m_le_density,SIGNAL(textEdited(QString)), this, SLOT(slot_densityChanged()));
    connect(this, SIGNAL(setMatrix(int,int,int)),m_math, SLOT(set_matrix(int,int,int)));
    connect(this, SIGNAL(calcFloid()), m_math, SLOT(floyd()));
    connect(this, SIGNAL(getResults(int,int)), m_math, SLOT(get_results(int,int)));
    connect(this, SIGNAL(clearMatrix()), m_math, SLOT(clear_Matrix()));
}

void MainWindow::slot_pb_ok()
{
    emit clearMatrix();
    slot_set_table(m_le_countTops->text());
    clear();
    int countTops = m_le_countTops->text().toInt();
    for(int i = STEP_SIZE; i <= countTops; i+=STEP_SIZE) {
        slot_set_table(QString::number(i));
        for(int k = 0; k < SAMPLE_SIZE; k++) {
            slot_LoadData(i);
            calcFloid();
            m_math->dijkstra();
            m_math->randomSearch();
        }
        steps << (double)i;
    }
    outGraph();
    //m_lb_resultOut->setText(QString::number(result, 'i', 0));
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
    calcCentr();
    calcChance();
    cPlotResults->clearGraphs();
    cPlotTime->clearGraphs();

    double max = 0.0;

    for(int i = 0; i < FloydCentr.size(); i++) {
        if(FloydCentr.at(i) > max || max == 0.0) {
            max = FloydCentr.at(i);
        }
    }
    for(int i = 0; i < RandCentr.size(); i++) {
        if(RandCentr.at(i) > max || max == 0.0) {
            max = RandCentr.at(i);
        }
    }
// results
    QPen floyd;
    floyd.setColor(QColor(Qt::red));
    floyd.setWidthF(6);

    QPen dijkstra;
    dijkstra.setColor(QColor(30, 40, 255, 150));
    dijkstra.setStyle(Qt::DotLine);
    dijkstra.setWidthF(5);

    QPen randPen;
    randPen.setColor(QColor(Qt::yellow));
    randPen.setStyle(Qt::DashDotDotLine);
    randPen.setWidthF(4);


    cPlotResults->addGraph(); //будем выводить результат к количеству вершин

    //floyd.setStyle(Qt::DotLine);
    cPlotResults->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::red, Qt::white, 7));
    cPlotResults->graph(0)->setPen(floyd);

    cPlotResults->graph(0)->setData(steps, FloydCentr);

    cPlotResults->addGraph();
    cPlotResults->graph(1)->setPen(dijkstra);
    cPlotResults->graph(1)->setData(steps, DijkstraCentr);

    cPlotResults->addGraph();
    cPlotResults->graph(2)->setPen(randPen);
    cPlotResults->graph(2)->setData(steps, RandCentr);
    cPlotResults->xAxis->setLabel("Количество вершин");
    cPlotResults->yAxis->setLabel("Результат");
    cPlotResults->xAxis->setRange(STEP_SIZE, m_le_countTops->text().toInt());
    cPlotResults->yAxis->setRange(0, max);



    cPlotResults->replot();
    cPlotResults->show();

//time
    //results = getResults(0,1);
    cPlotTime->addGraph();
    cPlotTime->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::red, Qt::white, 7));
    cPlotTime->graph(0)->setPen(floyd);
    cPlotTime->graph(0)->setData(steps, FloydCentrTime);

    max = 0;
    for(int i = 0; i < FloydCentrTime.size(); i++) {
        if(FloydCentrTime.at(i) > max)
            max = FloydCentrTime.at(i);

    }

    for(int i = 0; i < DijkstraCentrTime.size(); i++) {
        if(DijkstraCentrTime.at(i) > max)
            max = DijkstraCentrTime.at(i);

    }

    for(int i = 0; i < RandCentrTime.size(); i++) {
        if(RandCentrTime.at(i) > max)
            max = RandCentrTime.at(i);

    }


    cPlotTime->addGraph();
    cPlotTime->graph(1)->setPen(dijkstra);
    //  results = getResults(1,1);
    cPlotTime->graph(1)->setData(steps, DijkstraCentrTime);

    cPlotTime->addGraph();
    cPlotTime->graph(2)->setPen(randPen);
    cPlotTime->graph(2)->setData(steps, RandCentrTime);




    cPlotTime->xAxis->setLabel("Количество вершин");
    cPlotTime->yAxis->setLabel("Время");
    cPlotTime->xAxis->setRange(STEP_SIZE, m_le_countTops->text().toInt());
    cPlotTime->yAxis->setRange(0, max);

    cPlotTime->replot();
    cPlotTime->show();

//solve
    cPlotSolve->addGraph();
    cPlotSolve->graph(0)->setLineStyle(QCPGraph::lsStepCenter);
    cPlotSolve->graph(0)->setPen(dijkstra);
    cPlotSolve->graph(0)->setData(steps, chanceDijkstra);

    cPlotSolve->addGraph();
    cPlotSolve->graph(1)->setLineStyle(QCPGraph::lsStepCenter);
    cPlotSolve->graph(1)->setPen(randPen);
    cPlotSolve->graph(1)->setData(steps, chanceRand);

    cPlotSolve->xAxis->setLabel("Количество вершин");
    cPlotSolve->yAxis->setLabel("Вероятность успеха");
    cPlotSolve->xAxis->setRange(STEP_SIZE, m_le_countTops->text().toInt());
    cPlotSolve->yAxis->setRange(0, 1.3);

    cPlotSolve->replot();
    cPlotSolve->show();

}


void MainWindow::calcChance()
{
    QVector<double> resultsFlo;
    resultsFlo = getResults(0,0);

    QVector<double> resultsDijk;
    resultsDijk = getResults(1,0);

    QVector<double> resultsRand;
    resultsRand = getResults(2,0);

    int tmp = 0;
    int plus = 0;
    for(int i = 0; i < resultsFlo.size(); i++) {
        tmp++;
        if(resultsFlo[i] != resultsDijk[i])
            plus++;
        if(tmp == SAMPLE_SIZE) {
            chanceDijkstra.push_back((double)1-(plus/SAMPLE_SIZE));
            plus = 0;
            tmp = 0;
        }
    }

    plus = 0;
    tmp = 0;

    for(int i = 0; i < resultsFlo.size(); i++) {
        tmp++;
        if(resultsFlo[i] != resultsRand[i])
            plus++;
        if(tmp == SAMPLE_SIZE) {
            chanceRand.push_back((double)1-(double)((double)plus/(double)SAMPLE_SIZE));
          //      qDebug() << chanceRand.back() << "chance" << (double)1-(double)((double)plus/(double)SAMPLE_SIZE);
            plus = 0;
            tmp = 0;
        }
    }
}


void MainWindow::calcCentr()
{
    QVector<double> results;
    results = getResults(0,0);
    double max = 0.0;
    int tmp = 0;
//floy
    for(int i = 0; i < results.size(); i++) {
        tmp++;
        max+=(double)results.at(i);
        if(tmp == SAMPLE_SIZE) {
            max /= SAMPLE_SIZE;
            FloydCentr.push_back((double)max);
            max = 0.0;
            tmp = 0;
        }

    }
//dijk
    max = 0.0;
    tmp = 0;
    results = getResults(1,0);
    for(int i = 0; i < results.size(); i++) {
        tmp++;
        max+=(double)results.at(i);
        if(tmp == SAMPLE_SIZE) {
            max /= SAMPLE_SIZE;
            DijkstraCentr.push_back((double)max);
            max = 0.0;
            tmp = 0;
        }
    }
//rand
    max = 0.0;
    tmp = 0;
    results = getResults(2,0);
    qDebug() << "size" << results.size();
    for(int i = 0; i < results.size(); i++) {
        tmp++;
        max+=(double)results.at(i);
        if(tmp == SAMPLE_SIZE) {
            max /= SAMPLE_SIZE;
            RandCentr.push_back((double)max);
            max = 0.0;
            tmp = 0;
        }
    }

    //// TIME
    ///
    max = 0.0;
    tmp = 0;
    results = getResults(0,1);

    for(int i = 0; i < results.size(); i++) {
        tmp++;
        max+=(double)results.at(i);
        if(tmp == SAMPLE_SIZE) {
            max /= SAMPLE_SIZE;
            FloydCentrTime.push_back((double)max);
            max = 0.0;
            tmp = 0;
        }

    }
    max = 0.0;
    tmp = 0;
    results = getResults(1,1);
    for(int i = 0; i < results.size(); i++) {
        tmp++;
        max+=(double)results.at(i);
        if(tmp == SAMPLE_SIZE) {
            max /= SAMPLE_SIZE;
            DijkstraCentrTime.push_back((double)max);
            max = 0.0;
            tmp = 0;
        }
    }

    max = 0.0;
    tmp = 0;
    results = getResults(2,1);
    for(int i = 0; i < results.size(); i++) {
        tmp++;
        max+=(double)results.at(i);
        if(tmp == SAMPLE_SIZE) {
            max /= SAMPLE_SIZE;
            RandCentrTime.push_back((double)max);
            max = 0.0;
            tmp = 0;
        }
    }
}


void  MainWindow::clear()
{
    steps.clear();
    chanceDijkstra.clear();
    chanceRand.clear();

    FloydCentr.clear();
    DijkstraCentr.clear();
    FloydCentrTime.clear();
    DijkstraCentrTime.clear();

    RandCentr.clear();
    RandCentrTime.clear();
}
