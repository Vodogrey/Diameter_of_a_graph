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

    int max = sum[0][1];
    for (int i = 0; i < sum.size(); i++) {
        for (int j = i + 1; j < sum.size(); j++) {
            if (sum[i][j] > max)
                max = sum[i][j];
        }
    }
    return max;
}

int mathGraph::greedy()
{
    QVector<QVector<int> > sum;
    sum.resize(matrixOfSize.size()-1);
    for (int i = 0; i < sum.size(); ++i)
        sum[i].resize(matrixOfSize.size()-1);



    for (int i = 0; i < sum.size(); i++) {
        for (int j = 0; j < sum.size(); j++) {
            sum[i][j] = matrixOfSize[i][j];
        }
    }

// alg
//
//
//
//
//

    QVector<bool> visited;
    QVector<int> distance;
    visited.resize(sum.size());
    distance.resize(sum.size());


    int index, u; // st начальная точка
    int max = 0;
    for(int vert = 0; vert < sum.size(); vert++) {
        for (int i=0; i<sum.size(); i++)
        {
            distance[i]=INT_MAX;
            visited[i]=false;
        }


        distance[vert]=0;
        for (int count=0; count<sum.size()-1; count++)
        {
            int min=INT_MAX;
            for (int i=0; i<sum.size(); i++)
                if (!visited[i] && distance[i]<=min)
                {
                    min=distance[i]; index=i;
                }
            u=index;
            visited[u]=true;
            for (int i=0; i<sum.size(); i++)
                if (!visited[i] && sum[u][i] && distance[u]!=INT_MAX && distance[u]+sum[u][i]<distance[i])
                    distance[i]=distance[u]+sum[u][i];
        }
        for(int i = 0; i  < sum.size(); ++i) {
            if (distance[i]!=INT_MAX)
                if(max < distance[i]) {
                    max = distance[i];
                }
        }
    }
    qDebug() << "max dex" << max;

//end alg
    for (int i = 0; i < sum.size(); i++){
        for (int j = 0; j < sum.size(); j++){
            if (sum[i][j] == INT_MAX)
                sum[i][j] = 0;
        }
    }
}
