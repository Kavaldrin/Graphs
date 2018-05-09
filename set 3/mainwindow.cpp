#include "mainwindow.h"
#include "ui_mainwindow.h"

//first time Qt, so this code is crap i know


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    graph = std::unique_ptr<Graph>(new Graph());
    showingGraph = false;
    generated = false;
    inDijaskra = false;
    dijaskraDone = false;
    showingPaths = false;
    showingWeights = false;
    matrixDone = false;
    showingMatrix = false;
    showingCentre = false;
    centreVertex = -1;
    minMaxVertex = -1;
    primDone = false;
    kruskalDone = false;
    srand(time(NULL));
    ui->widget_3->hide();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_centreButton_clicked()
{

    if(inDijaskra)
        return;


    if(showingGraph && generated){

        if(showingCentre)
            showingCentre = false;
        else{

        showingCentre = true;
        if(centreVertex == -1 && minMaxVertex == -1)
            centreVertex = graph->computeCentreVertex();
            minMaxVertex = graph->computeMinMax();
        }
    }
    update();



}

void MainWindow::on_dijaskraButton_clicked()
{
    if(!generated || showingMatrix || showingCentre || showingTree)
        return;
    inDijaskra = true;
    showingWeights = false;
    showingPaths = false;
    ui->widget_3->show();
    ui->widget_3->setEnabled(true);
    ui->label->setEnabled(true);
    ui->label_2->setEnabled(true);
    update();

}

void MainWindow::on_generateGraphButton_clicked()
{
    if(!inDijaskra && !showingMatrix && !showingCentre){
        graph->generateConnected();
        showingGraph = true;
        showingWeights = true;
        update();
        generated = true;
        dijaskraDone = false;
        matrixDone = false;
        centreVertex = -1;
        minMaxVertex = -1;
        primDone = false;
        kruskalDone = false;
        showingTree = false;
    }
}



void MainWindow::on_matrixButton_clicked()
{
    if(!generated || inDijaskra || showingTree)
        return;

    if(!matrixDone){
        graph->ComputeDistanceMatrix();

        matrixDone = true;
    }
    if(showingMatrix){
        showingMatrix = false;
        showingGraph = true;
    }
    else{
        showingGraph = false;
        showingMatrix = true;
    }
    update();





}

void MainWindow::on_kruskalButton_clicked()
{
    if(!generated || inDijaskra)
        return;

    if(showingGraph || showingMatrix){
        showingTree = true;
        showingGraph = false;
        showingMatrix = false;
        if(!kruskalDone){
            graph->generateTreeKruskal();
            kruskalDone = true;
        }
    }else{
        showingTree = false;
        showingGraph = true;
    }
    update();
}
void MainWindow::on_primButton_clicked()
{
    if(!generated || inDijaskra)
        return;

    if(showingGraph || showingMatrix){
        showingTree = true;
        showingGraph = false;
        showingMatrix = false;
        if(!primDone){
            graph->generateTreePrim();
            primDone = true;
        }
    }else{
        showingTree = false;
        showingGraph = true;
    }
    update();


}
void MainWindow::paintEvent(QPaintEvent *e)
{





    if(showingGraph){

        std::vector<std::pair<int,int>> pairs= graph->getPairs();
        std::map<std::pair<int,int>,int> weights = graph->getWeights();
        std::vector<Vertex> vertexes = graph->getVertexes();

        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        QBrush brush(QColor(255,255,255));
        QPen(QColor(0,0,0));
        painter.setBackground(brush);
        QSize size = ui->widget->size();

        int centreX = size.width()/2;
        int centreY = size.height()/2;
        const int numCirc = graph->getVertexNum();
        double angle = (360. / numCirc) * 3.14159265359 / 180.;

        QPoint p1;
        QPoint p2;



        QFont font;
        font.setPixelSize(18);
        painter.setFont(font);

        for(auto &pr : pairs){
            p1.setX(centreX + cos(angle * pr.first)*centreX/1.3);
            p1.setY(centreY + sin(angle * pr.first)*centreY/1.3);

            p2.setX(centreX + cos(angle * pr.second)*centreX/1.3);
            p2.setY(centreY + sin(angle * pr.second)*centreY/1.3);

            painter.setPen(Qt::black);
            painter.drawLine(p1,p2);
            QString temp_str(std::to_string(weights[pr]).c_str());
            painter.setPen(Qt::red);
            painter.drawText((p1+p2)/2,temp_str);

        }

        //yolo
        painter.setPen(Qt::black);
        int ratio = 20 - numCirc/5;

        painter.setBrush(brush);
        for(int i=0;i<numCirc;++i){
            QString temp_str(std::to_string((i+1)).c_str());
            painter.drawEllipse(QPoint(centreX+cos(angle*i)*centreX/1.3,centreY+sin(angle*i)*centreY/1.3),ratio,ratio);
            if(i < 9)
                painter.drawText(centreX+cos(angle*i)*centreX/1.3 - ratio/4,centreY+sin(angle*i)*centreY/1.3 + ratio/3,temp_str);
            else
                painter.drawText(centreX+cos(angle*i)*centreX/1.3 - ratio/4-5,centreY+sin(angle*i)*centreY/1.3 + ratio/3,temp_str);

        }

        painter.setPen(Qt::red);
        if(showingCentre){
            //centrum
            painter.drawEllipse(QPoint(centreX+cos(angle*centreVertex)*centreX/1.3,centreY+sin(angle*centreVertex)*centreY/1.3),ratio,ratio);
            QString temp_str(std::to_string((centreVertex+1)).c_str());
            if(centreVertex < 9)
                painter.drawText(centreX+cos(angle*centreVertex)*centreX/1.3 - ratio/4,centreY+sin(angle*centreVertex)*centreY/1.3 + ratio/3,temp_str);
            else
                painter.drawText(centreX+cos(angle*centreVertex)*centreX/1.3 - ratio/4-5,centreY+sin(angle*centreVertex)*centreY/1.3 + ratio/3,temp_str);
            painter.drawText(centreX+cos(angle*centreVertex)*centreX/1.3 - ratio/4-25,centreY+sin(angle*centreVertex)*centreY/1.3 + ratio/3-30,QString("Centre"));

            //minmax
            painter.setPen(Qt::blue);
            if(centreVertex == minMaxVertex){

                painter.drawText(centreX+cos(angle*centreVertex)*centreX/1.3 - ratio/4+30,centreY+sin(angle*centreVertex)*centreY/1.3 + ratio/3-30,QString("MinMax"));
            }
            else{


                painter.drawEllipse(QPoint(centreX+cos(angle*minMaxVertex)*centreX/1.3,centreY+sin(angle*minMaxVertex)*centreY/1.3),ratio,ratio);
                QString temp_str(std::to_string((minMaxVertex+1)).c_str());
                if(minMaxVertex < 9)
                    painter.drawText(centreX+cos(angle*minMaxVertex)*centreX/1.3 - ratio/4,centreY+sin(angle*minMaxVertex)*centreY/1.3 + ratio/3,temp_str);
                else
                    painter.drawText(centreX+cos(angle*minMaxVertex)*centreX/1.3 - ratio/4-5,centreY+sin(angle*minMaxVertex)*centreY/1.3 + ratio/3,temp_str);
                painter.drawText(centreX+cos(angle*minMaxVertex)*centreX/1.3 - ratio/4-25,centreY+sin(angle*minMaxVertex)*centreY/1.3 + ratio/3-30,QString("MinMax"));
            }
        }


    //prawe rysowanie //////drawing weights//////////
        if(showingWeights){

            painter.setPen(Qt::black);
            QSize buttonSize = ui->dijaskraButton->size();
            int minY = buttonSize.height() * 9;
            painter.drawLine(QPoint(centreX+centreX+23,minY-10),QPoint(centreX+centreX+175,minY-10));
            painter.drawLine(QPoint(centreX+centreX+23,minY-10),QPoint(centreX+centreX+23,2*centreY));
            painter.drawLine(QPoint(centreX+centreX+175,minY-10),QPoint(centreX+centreX+175,2*centreY));
            painter.drawLine(QPoint(centreX+centreX+23,2*centreY),QPoint(centreX+centreX+175,2*centreY));
            font.setPixelSize(12);
            painter.setFont(font);
            int iteratorX = 0,iteratorY = 0;
            int temp_it = 0;
            QString qstring;
            std::string stdstring;
            for(auto &pr : pairs){

                qstring.clear();
                stdstring = std::to_string(pr.first+1) + " " +std::to_string(pr.second+1) + " : " + std::to_string(weights[pr]);
                qstring.append(stdstring.c_str());
                painter.drawText(QPoint(2*centreX + 30,minY+10) + QPoint(iteratorX,iteratorY),qstring);
                ++temp_it;
                if(temp_it == 2){
                    iteratorY+=20;
                    iteratorX = 0;
                    temp_it = 0;
                }else{
                    iteratorX+=70;
                }
            }
         }
           // end prawe rysowanie //////drawing weights//////////
        if(showingPaths){
            painter.setPen(Qt::black);
            QSize buttonSize = ui->dijaskraButton->size();
            int minY = buttonSize.height() * 9;
            painter.drawLine(QPoint(centreX+centreX+23,minY-10),QPoint(centreX+centreX+175,minY-10));
            painter.drawLine(QPoint(centreX+centreX+23,minY-10),QPoint(centreX+centreX+23,2*centreY));
            painter.drawLine(QPoint(centreX+centreX+175,minY-10),QPoint(centreX+centreX+175,2*centreY));
            painter.drawLine(QPoint(centreX+centreX+23,2*centreY),QPoint(centreX+centreX+175,2*centreY));
            font.setPixelSize(12);
            painter.setFont(font);
            int iteratorX = 0,iteratorY = 0;
            QString qstring;
            std::string stdstring;

            std::vector<std::vector<int>> paths = vertexes[dijaskraVertex].getPaths();
            std::map<int,int> weightsDij = vertexes[dijaskraVertex].getWeights();

            for(int i=0;i<graph->getVertexNum();++i){
                stdstring = std::to_string(i+1) + ": ";
                qstring.append(stdstring.c_str());
                painter.drawText(QPoint(2*centreX + 30,minY+10) + QPoint(iteratorX,iteratorY),qstring);
                qstring.clear();
                iteratorX+=20;

                if(paths[i].empty()){
                    stdstring = "Starting vertex";
                    qstring.append(stdstring.c_str());
                    painter.drawText(QPoint(2*centreX + 30,minY+10) + QPoint(iteratorX,iteratorY),qstring);
                    qstring.clear();
                    iteratorX+=90;
                }


                for(auto &neigh : paths[i]){
                    stdstring = std::to_string(neigh+1) + " ";
                    qstring.append(stdstring.c_str());
                    painter.drawText(QPoint(2*centreX + 30,minY+10) + QPoint(iteratorX,iteratorY),qstring);
                    qstring.clear();
                    iteratorX+=20;
                    if(iteratorX > 100){
                        iteratorX = 0;
                        iteratorY+=20;
                    }
                }

                painter.setPen(Qt::red);
                stdstring = "[" + std::to_string(weightsDij[i]) + "]";
                qstring.append(stdstring.c_str());
                painter.drawText(QPoint(2*centreX + 30,minY+10) + QPoint(iteratorX,iteratorY),qstring);
                qstring.clear();
                iteratorY+=20;
                iteratorX=0;
                painter.setPen(Qt::black);


            }

        }
    }


    //////// MATRIX
    if(showingMatrix){
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        QBrush brush(QColor(255,255,255));
        QPen(QColor(0,0,0));
        painter.setBackground(brush);
        QSize size = ui->widget->size();
        painter.setPen(Qt::black);
        QFont font;
        font.setPixelSize(12);
        painter.setFont(font);
        std::vector<Vertex> vertexes = graph->getVertexes();


        int const VERTEXNUM = graph->getVertexNum();

        int stepX = (size.width()-40) / VERTEXNUM;
        int stepY = (size.height()-40) /VERTEXNUM;

        std::string stdstring;
        QString qstring;

        //krawedzie
        for(int i=0;i<VERTEXNUM+1;++i){
            painter.drawLine(QPoint(40,stepY*i+40),QPoint(stepX*VERTEXNUM+40,stepY*i+40));
            painter.drawLine(QPoint(stepX*i + 40,40),QPoint(stepX*i+40,stepY*VERTEXNUM+40));
        }
        //numeracja
        for(int i=0;i<VERTEXNUM;++i){
            stdstring = std::to_string(i+1);
            qstring.append(stdstring.c_str());
            painter.drawText(QPoint((stepX*i + 40)  + stepX/2,20),qstring);
            painter.drawText(QPoint(20,(stepY*i+40) + stepY/2),qstring);
            qstring.clear();
        }

        font.setPixelSize(18);
        painter.setFont(font);
        for(int i=0;i<VERTEXNUM;++i){
            std::map<int,int> weights = vertexes[i].getWeights();
            for(int j=0;j<VERTEXNUM;++j){
                stdstring = std::to_string(weights[j]);
                qstring.append(stdstring.c_str());
                if(weights[j] < 10)
                    painter.drawText(QPoint(stepX*j+40 + stepX/2.5,stepY*i+40 + stepY/1.8),qstring);
                else
                    painter.drawText(QPoint(stepX*j+40 + stepX/2.5-5,stepY*i+40 + stepY/1.6),qstring);
                qstring.clear();
            }
        }


    }


    //drawing tree
    if(showingTree){

        std::vector<std::pair<int,int>> tree = graph->getTreeConnections();
        std::map<std::pair<int,int>,int> weights = graph->getWeights();

        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        QBrush brush(QColor(255,255,255));
        QPen(QColor(0,0,0));
        painter.setBackground(brush);
        QSize size = ui->widget->size();

        int centreX = size.width()/2;
        int centreY = size.height()/2;
        const int numCirc = graph->getVertexNum();
        double angle = (360. / numCirc) * 3.14159265359 / 180.;

        QPoint p1;
        QPoint p2;



        QFont font;
        font.setPixelSize(18);
        painter.setFont(font);

        for(auto &pr : tree){
            p1.setX(centreX + cos(angle * pr.first)*centreX/1.3);
            p1.setY(centreY + sin(angle * pr.first)*centreY/1.3);

            p2.setX(centreX + cos(angle * pr.second)*centreX/1.3);
            p2.setY(centreY + sin(angle * pr.second)*centreY/1.3);

            painter.setPen(Qt::black);
            painter.drawLine(p1,p2);
            QString temp_str(std::to_string(weights[pr]).c_str());
            painter.setPen(Qt::red);
            painter.drawText((p1+p2)/2,temp_str);

        }

        painter.setPen(Qt::black);
        int ratio = 20 - numCirc/5;

        painter.setBrush(brush);
        for(int i=0;i<numCirc;++i){
            QString temp_str(std::to_string((i+1)).c_str());
            painter.drawEllipse(QPoint(centreX+cos(angle*i)*centreX/1.3,centreY+sin(angle*i)*centreY/1.3),ratio,ratio);
            if(i < 9)
                painter.drawText(centreX+cos(angle*i)*centreX/1.3 - ratio/4,centreY+sin(angle*i)*centreY/1.3 + ratio/3,temp_str);
            else
                painter.drawText(centreX+cos(angle*i)*centreX/1.3 - ratio/4-5,centreY+sin(angle*i)*centreY/1.3 + ratio/3,temp_str);

        }

    }

}



void MainWindow::on_chooseDijaskraButton_clicked()
{
    std::string temp_str = ui->textEdit->toPlainText().toStdString();
    int temp_int;
    if(isdigit(temp_str[0])){
        temp_int = std::stoi(temp_str);
        if(temp_int < graph->getVertexNum()+1 && temp_int > 0){
            dijaskraVertex = temp_int - 1;
            ui->widget_3->setEnabled(false);
            ui->widget_3->hide();
            showingWeights = true;
            inDijaskra = false;

            graph->Dijaskra(dijaskraVertex);

            update();
            dijaskraDone = true;

        }
    }
    else if(temp_str.empty()){
        ui->widget_3->setEnabled(false);
        ui->widget_3->hide();
        showingWeights = true;
        showingPaths = false;
        inDijaskra = false;
        update();

    }
}

void MainWindow::on_shortestPathButton_clicked()
{
    if(!dijaskraDone)
        return;

    ui->widget_3->setEnabled(false);
    ui->widget_3->hide();
    showingWeights = false;
    showingPaths = true;
    update();
}
