#pragma once
#include <qvector.h>
#include <qapplication.h>


class mathGraph :public QObject
{
    Q_OBJECT
public:
    mathGraph();
    ~mathGraph();

public slots:
    void set_matrix(int cow, int row, int size);
    void clear_Matrix();
    int floid();
    void greedy();

private:
    QVector<QVector<int> > matrixOfSize; // матрица
    QVector<int> floidResults; // вектор записи результатов вычислений флоида

};
