#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "graph.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_generateGraphButton_clicked();

    void on_generateinMenubutton_clicked();

    void on_cancelinMenubutton_clicked();

    void on_drawGraphbutton_clicked();

    void on_drawRepresentationsButton_clicked();

    void on_AdjacencyButton_clicked();

    void on_IncidenceButton_clicked();

    void on_ListButton_clicked();

    void on_KosarajButton_clicked();

    void on_generateStrongbutton_clicked();

    void on_bellmanFordButton_clicked();

    void on_inBellmanButton_clicked();

    void on_inBellmanShowButton_clicked();

    void on_Johnsonbutton_clicked();

private:

    bool anyMode(){
        if(generateMode || textDisplayMode || graphicDisplayMode || KosarajMode || generateConnectedMode || BellmanFordMode
                || JohnsonMode)
            return true;
        return false;
    }
    void resetModes(){
        generateMode=false;textDisplayMode=false;graphicDisplayMode=false;KosarajMode=false;generateConnectedMode=false;BellmanFordMode=false;JohnsonMode=false;
    }


    Ui::MainWindow *ui;
    bool generateMode;
    bool textDisplayMode;
    bool graphicDisplayMode;
    bool KosarajMode;
    bool generateConnectedMode;
    bool BellmanFordMode;
    bool JohnsonMode;


    bool adja;
    bool adjalist;
    bool inci;

    std::shared_ptr<Graph> graph;

    void paintEvent(QPaintEvent *e);
    void drawGraph(QPainter& painter);
    void drawRepresentations(QPainter& painter);
    void drawStrongComponents(QPainter& painter);
    void drawStrongConnected(QPainter &painter);
    void drawBellman(QPainter &painter);
    void drawJohnsonMatrix(QPainter &painter);

};

#endif // MAINWINDOW_H
