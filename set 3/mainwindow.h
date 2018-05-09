#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include <QDebug>
#include "graph.h"
#include <QPainter>
#include <cmath>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    //////////////////

    std::unique_ptr<Graph> graph;
    std::pair<int,int> uniqPoints; //to draw
//////////////////////////////////
protected:
    void paintEvent(QPaintEvent *e);


private slots:
    void on_centreButton_clicked();

    void on_dijaskraButton_clicked();

    void on_generateGraphButton_clicked();

    void on_kruskalButton_clicked();

    void on_matrixButton_clicked();

    void on_primButton_clicked();



    void on_chooseDijaskraButton_clicked();

    void on_shortestPathButton_clicked();

private:
    bool showingGraph;
    int dijaskraVertex;
    int centreVertex;
    int minMaxVertex;
    bool showingWeights;
    bool inDijaskra;
    bool generated;
    bool dijaskraDone;
    bool showingPaths;
    bool matrixDone;
    bool primDone;
    bool showingMatrix;
    bool showingCentre;
    bool showingTree;
    bool kruskalDone;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
