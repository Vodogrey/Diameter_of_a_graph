#pragma once
#include <qvector.h>
#include <qapplication.h>
#include <QTime>
#include <QQueue>

struct Edge {
    int a;
    int b;
    int size;
};


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
    void randomSearch();
    QVector<double> get_results(int alg, int type);

protected:
    void setEdges(int V1, int V2, int size);

private:
    QVector<QVector<int> > matrixOfSize; // матрица
    QVector<Edge> listEdges;
    QVector<int> floydResults; // вектор записи результатов вычислений флоида
    QVector<int> floydTime;
    QVector<int> dijkstraResults;
    QVector<int> dijkstraTime;
    QVector<int> randResults;
    QVector<int> randTime;
};
