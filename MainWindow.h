#pragma once


#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <qgridlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qdebug.h>
#include <qcheckbox.h>
#include <qlineedit.h>
#include <qtablewidget.h>
#include <QPen>

#include "mathgraph.h"
#include <qcustomplot.h>

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = 0);
	~MainWindow();
signals:
    void setMatrix(int, int, int);
    void clearMatrix();
    int calcFloid();
    QVector<double> getResults(int, int);

protected:
	void GUI();
	void Buttons();
    void outGraph();
    void calcChance();
    void calcCentr();
    void clear();

	protected slots:
	void slot_pb_ok();
    void slot_set_table(QString Count);
    void slot_LoadData(int curCountTops);
    void slot_densityChanged();

private:
	QWidget* m_widget;
	QGridLayout* m_layout;
	QPushButton* m_pb_ok;
    QLabel* m_lb_countTops;
    QLabel* m_lb_density;
    QLineEdit* m_le_countTops;
    QLineEdit* m_le_density;
    QCheckBox* m_cb_random;
    QTableWidget *m_tw_matrix;
    QLabel* m_lb_resultOut;

    mathGraph* m_math;
    int matrixSize;

    QCustomPlot* cPlotResults;
    QCustomPlot* cPlotTime;
    QCustomPlot* cPlotSolve;

    QVector<double> steps;
    QVector<double> chanceDijkstra;
    QVector<double> chanceRand;

    QVector<double> FloydCentr;
    QVector<double> DijkstraCentr;
    QVector<double> FloydCentrTime;
    QVector<double> DijkstraCentrTime;
    QVector<double> RandCentr;
    QVector<double> RandCentrTime;
};
