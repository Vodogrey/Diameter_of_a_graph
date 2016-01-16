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

#include "mathgraph.h"

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

protected:
	void GUI();
	void Buttons();

	protected slots:
	void slot_pb_ok();
    void slot_set_table(QString Count);
    void slot_LoadData(bool checked);
    void slot_densityChanged();

private:
	QWidget* m_widget;
	QGridLayout* m_layout;
	QPushButton* m_pb_ok;
    QLabel* m_lb_countTops;
    QLabel* m_lb_density;
    QLineEdit* m_le_countTops;
    QLineEdit* m_le_density;
    QCheckBox* m_cb_floid;
    QCheckBox* m_cb_random;
    QTableWidget *m_tw_matrix;
    QLabel* m_lb_resultOut;

    mathGraph* m_math;
    int matrixSize = 0;
};
