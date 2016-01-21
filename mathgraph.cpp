#include "mathgraph.h"

mathGraph::mathGraph()
{
    qsrand(QTime::currentTime().msec());
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
    if(size != 0)
        setEdges(cow,row,size);
}

void mathGraph::clear_Matrix()
{
    matrixOfSize.clear();
    matrixOfSize.resize(2);
    floydResults.clear();
    floydTime.clear();
    dijkstraResults.clear();
    dijkstraTime.clear();
    randTime.clear();
    randResults.clear();
    listEdges.clear();
}

void mathGraph::floyd()
{
    QTime t;
    t.start();

    QVector<QVector<int> > sum;
    //    sum.resize(matrixOfSize.size());
    //    for (int i = 0; i < sum.size(); ++i)
    //        sum[i].resize(matrixOfSize.size());
    sum = matrixOfSize;



    for (int i = 0; i < sum.size(); i++) {
        for (int j = 0; j < sum.size(); j++) {
            // sum[i][j] = matrixOfSize[i][j];
            if (!sum[i][j])
                sum[i][j] = 10000;
        }
    }

    for (int k = 0; k < sum.size(); k++) {
        for (int i = 0; i < sum.size(); i++){
            for (int j = 0; j < sum.size(); j++){
                if (i != j)
                    sum[i][j] = qMin(sum[i][j], sum[i][k] + sum[k][j]);
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
    floydResults << max;
    floydTime << t.elapsed();
    qDebug() << "flo end";
}

void mathGraph::dijkstra()
{
    QTime t;
    t.start();

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

    QVector<bool> visited;
    QVector<int> distance;
    visited.resize(sum.size());
    distance.resize(sum.size());


    int index, u;
    int max = 0;
    for(int vert = 0; vert < sum.size(); vert++) { // vert - начальная точка
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
                if (!visited[i] && distance[i]<=min) {
                    min=distance[i];
                    index=i;
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
    dijkstraResults << max;
    dijkstraTime << t.elapsed();

    //end alg
    for (int i = 0; i < sum.size(); i++){
        for (int j = 0; j < sum.size(); j++){
            if (sum[i][j] == INT_MAX)
                sum[i][j] = 0;
        }
    }
    qDebug() << "dijk end";
}

void mathGraph::randomSearch()
{
    QTime t;
    t.start();
    bool searched = false;
    bool isEnded = false;
    int max = 0;
    if(listEdges.isEmpty()) {
        searched = true;
    }
    max = 0;
    QVector<bool> used (listEdges.size(), false);
    bool NeedRandom = true;
    while(!searched) {
        int v;
        if(NeedRandom)
            v = qrand() % matrixOfSize.size();
        isEnded = false;
        while(!isEnded) {
            for(int i = 0; i < listEdges.size(); i++) {
                int tmpV = v;
                if(listEdges.at(i).a == v && !used[i]) {
                    max += listEdges.at(i).size;
                    tmpV = listEdges.at(i).b;
                    used[i] = true;
                }
                if(tmpV != v) {
                    isEnded = true;
                    v = tmpV;
                    NeedRandom = false;
                }
                else {
                    isEnded = true;
                    if(max > 0) {
                        searched = true;
                        NeedRandom = true;
                    }

                }
            }

        }
    }
    listEdges.clear();
    qDebug() << "max" << max;
    randResults.append(max);
    randTime << t.elapsed();
    qDebug() << "rand end";

}

QVector<double> mathGraph::get_results(int alg, int type)
{
    QVector<double> tempResults;


    switch (alg) {
    case 0:
        if(type == 0)
            for(int i = 0; i < floydResults.size();i++) {
                tempResults.append((double)floydResults.at(i));
            }
        if(type == 1)
            for(int i = 0; i < floydTime.size();i++) {
                tempResults.append((double)floydTime.at(i));
            }
        return tempResults;
    case 1:
        if(type == 0)
            for(int i = 0; i < dijkstraResults.size();i++) {
                tempResults.append((double)dijkstraResults.at(i));
            }
        if(type == 1)
            for(int i = 0; i < dijkstraTime.size();i++) {
                tempResults.append((double)dijkstraTime.at(i));
            }
        return tempResults;
    case 2:
        if(type == 0)
            for(int i = 0; i < randResults.size();i++) {
                tempResults.append((double)randResults.at(i));
            }
        if(type == 1)
            for(int i = 0; i < dijkstraTime.size();i++) {
                tempResults.append((double)randTime.at(i));
            }
        return tempResults;

    default:
        break;
    }
}

void mathGraph::setEdges(int V1, int V2, int size)
{
    Edge edge;
    edge.a = V1;
    edge.b = V2;
    edge.size = size;
    //  qDebug() << "at" << V1 << V2;
    listEdges.append(edge);
}
