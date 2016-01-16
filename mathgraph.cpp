#include "mathgraph.h"

mathGraph::mathGraph()
{

}

mathGraph::~mathGraph()
{

}
#include <qdebug.h>
void mathGraph::set_matrix(int cow, int row, int size)
{
    int tempSize = 2;
    tempSize += qMax(cow, row);
    if (tempSize > matrixOfSize.size())
        matrixOfSize.resize(tempSize);
    else
        tempSize = matrixOfSize.size();

    for (int i = 0; i < matrixOfSize.size(); ++i)
        matrixOfSize[i].resize(tempSize);


    matrixOfSize[cow][row] = size;
    matrixOfSize[row][cow] = size;
    qDebug() << matrixOfSize[cow][row];
}

void mathGraph::clear_Matrix()
{
    matrixOfSize.clear();
    matrixOfSize.resize(2);
}

int mathGraph::floid()
{
    QVector<QVector<int> > sum;
    sum.resize(matrixOfSize.size());
    for (int i = 0; i < sum.size(); ++i)
        sum[i].resize(matrixOfSize.size());



    for (int i = 0; i < sum.size(); i++) {
        for (int j = 0; j < sum.size(); j++) {
            sum[i][j] = matrixOfSize[i][j];
            if (!sum[i][j])
                sum[i][j] = 10000;
            //	qDebug() << "sum[i][j] on reverse" << i << j <<  sum[i][j];
        }
    }

    for (int k = 0; k < sum.size(); k++) {
        for (int i = 0; i < sum.size(); i++){
            for (int j = 0; j < sum.size(); j++){
                if (i != j)
                    sum[i][j] = qMin(sum[i][j], sum[i][k] + sum[k][j]);
                //  qDebug() << "sum b" << sum[i][j];
            }
        }
    }
    for (int i = 0; i < sum.size(); i++){
        for (int j = 0; j < sum.size(); j++){
            if (sum[i][j] == 10000)
                sum[i][j] = 0;
        }
    }
    int max;
    max = get_max(sum);
    floidResults << max;

    return max;
}


int mathGraph::get_max(QVector<int> vec)
{
    int max = vec[0][1];
    for (int i = 0; i < vec.size(); i++) {
        for (int j = i + 1; j < vec.size(); j++) {
            if (vec[i][j] > max)
                max = vec[i][j];
        }
    }
    return max;
}
