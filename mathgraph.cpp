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
    floydResults.clear();
    floydTime.clear();
    dijkstraResults.clear();
    dijkstraTime.clear();
}

void mathGraph::floyd()
{
    QTime t;
    t.start();

    QVector<QVector<int> > sum;
    sum.resize(matrixOfSize.size());
    for (int i = 0; i < sum.size(); ++i)
        sum[i].resize(matrixOfSize.size());



    for (int i = 0; i < sum.size(); i++) {
        for (int j = 0; j < sum.size(); j++) {
            sum[i][j] = matrixOfSize[i][j];
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
}

void mathGraph::greedy()
{
    QVector<QVector<int> > sum;
    sum.resize(matrixOfSize.size()-1);
    for (int i = 0; i < sum.size(); ++i)
        sum[i].resize(matrixOfSize.size()-1);

    for(int i = 0; i < sum.size(); i++) {
        for(int j = 0; j < sum.size(); j++) {
            sum[i][j] = matrixOfSize[i][j];
        }
    }

    //    int i = 0;

    //    QVector<bool> used (sum.size(), false);
    //    QVector<int> dist (sum.size(), INT_MAX);
    //    QQueue<int> q;

    //    int max = 0;

    //    used[i] = true;
    //    dist[i] = 0;

    //    q.push_back(i);

    //    while (!q.isEmpty()) {
    //        int i = q.front();
    //        q.pop_back();
    //        used[i] = false;
    //        for(int j = 0; j < sum.size(); j++) {
    //            int len = sum[i][j];
    //            int v;
    //            for(int tmp = 0; tmp < sum.size(); tmp++) {
    //                if(sum[j][tmp] > 0) {
    //                    v = tmp;
    //                    break;
    //                }
    //                    else {
    //                        v = 0;
    //                    }
    //            }

    //            if(dist[v] > dist[i] + len) {
    //                dist[v] = dist[i] + len;
    //                max = dist[v];
    //                qDebug() << "max" << max;
    //            }
    //            if (!used[v]) {
    //                     q.push_back(v);
    //                     used[v] = true;
    //                   }

    //        }

    //    }





    //    for (int i = 0; i < sum.size(); i++) {
    //        for (int j = 0; j < sum.size(); j++) {
    //            if(matrixOfSize[i][j] != 0)
    //                sum[i][j] = 1;
    //        }
    //    }


    //    int max = 0;
    //    for(int i = 0; i < sum.size(); i++) {
    //        QVector<int> from(sum.size(), -1);
    //        QVector<int> used(sum.size(), 0);
    //        QVector<int> dist(sum.size());
    //        QQueue<int> q;
    //        q.push_back(i);
    //        dist[i] = 0;
    //        used[i] = true;
    //        for(int j = 0; j < sum.size()-1; j++) {
    //            while (!q.empty())
    //            {
    //                int w = q.front();
    //                q.pop_back();
    //                for (int k = 0; k < sum.size(); ++i)
    //                {
    //                    if (sum[w][k] && !used[k])
    //                    {
    //                        dist[k] = dist[w] + matrixOfSize[w][i];
    //                        from[k] = w;
    //                        if(max < dist[k] || max == 0)
    //                            max = dist[k];
    //                        q.push_back(k);
    //                        used[k] = true;
    //                    }
    //                }
    //            }
    //        }
    //    }
    //    qDebug() << max;

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
    default:
        break;
    }
}

