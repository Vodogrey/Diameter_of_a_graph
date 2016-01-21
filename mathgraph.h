#pragma once
#include <qvector.h>
#include <qapplication.h>
#include <QTime>
#include <QQueue>


class mathGraph :public QObject
{
    Q_OBJECT
public:
    mathGraph();
    ~mathGraph();

public slots:
    void set_matrix(int cow, int row, int size);
    void clear_Matrix();
    void floyd();
    void dijkstra();
    void greedy();
    QVector<double> get_results(int alg, int type);

private:
    QVector<QVector<int> > matrixOfSize; // матрица
    QVector<int> floydResults; // вектор записи результатов вычислений флоида
    QVector<int> floydTime;
    QVector<int> dijkstraResults;
    QVector<int> dijkstraTime;
};
