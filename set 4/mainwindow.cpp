#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QPainter"

const double M_PI = 3.14285714286;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    graph = std::make_shared<Graph>(Graph());


    ui->menuGraphwidget->setVisible(false);
    generateMode = false;
    textDisplayMode = false;
    graphicDisplayMode = false;
    KosarajMode = false;
    generateConnectedMode = false;
    BellmanFordMode = false;
    JohnsonMode = false;

    inci = false;
    adja = false;
    adjalist = false;
    ui->AdjacencyButton->setVisible(false);
    ui->IncidenceButton->setVisible(false);
    ui->ListButton->setVisible(false);

    ui->bellmanWidget->setVisible(false);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_generateGraphButton_clicked()
{
    if(anyMode()){
        if(generateMode){
            ui->menuGraphwidget->setVisible(false);
            generateMode = false;
            ui->modeLabel->setText(QString("Current Mode: None"));

        }
        return;
    }

    generateMode = true;
    ui->modeLabel->setText(QString("Current Mode: generating Graph"));
    //fun graph->generate
    ui->menuGraphwidget->setVisible(true);


}

void MainWindow::on_generateinMenubutton_clicked()
{
    QString str = ui->vertexText->toPlainText();
    if(!str[0].isDigit()){
        ui->errorlabel->setText("Error: Not a number or number < 0 in vertex num");
        return;
    }
    int dig = str.toInt();
    if(dig < 2 || dig > 15){
        ui->errorlabel->setText("Error: Try another num of vertex [2,15]");
        return;
    }

    QString str2 = ui->probabilityText->toPlainText();
    if(!str2[0].isDigit()){
        ui->errorlabel->setText("Error: Not a number or number < 0 in probability num");
        return;
    }
    double dig2 = str2.toDouble();
    if(dig2 < 0 || dig2 > 1){
        ui->errorlabel->setText("Error: Try another num of probability [0,1]");
        return;
    }


    if(generateMode){
        graph->generateDirectedGraph(dig,dig2);

        ui->menuGraphwidget->setVisible(false);
        generateMode = false;
        graphicDisplayMode = true;
        ui->modeLabel->setText(QString("Current Mode: Displaying graph"));
        update();
     }
    else if(generateConnectedMode){
        graph->generateStrongConnected(dig,dig2);
        ui->menuGraphwidget->setVisible(false);
        ui->modeLabel->setText(QString("Current Mode: Displaying graph with wages"));
        update();
    }



}

void MainWindow::on_cancelinMenubutton_clicked()
{
    ui->menuGraphwidget->setVisible(false);
    generateMode = false;
    ui->modeLabel->setText(QString("Current Mode: None"));
}

void MainWindow::on_drawGraphbutton_clicked()
{
    if(generateMode || generateConnectedMode || !graph->isGenerated())
       return;



    if(graphicDisplayMode){
        ui->modeLabel->setText(QString("Current Mode: None"));
        graphicDisplayMode = false;
    }
    else{
        if(textDisplayMode){
            ui->AdjacencyButton->setVisible(false);
            ui->IncidenceButton->setVisible(false);
            ui->ListButton->setVisible(false);
        }
        resetModes();
        ui->modeLabel->setText(QString("Current Mode: Displaying graph"));
        graphicDisplayMode = true;

    }
    update();

}

void MainWindow::on_drawRepresentationsButton_clicked()
{
    if(generateMode || generateConnectedMode || !graph->isGenerated())
       return;

    if(textDisplayMode){
        ui->modeLabel->setText(QString("Current Mode: None"));
        inci = false;
        adja = false;
        adjalist = false;

        ui->AdjacencyButton->setVisible(false);
        ui->IncidenceButton->setVisible(false);
        ui->ListButton->setVisible(false);
        textDisplayMode = false;
    }
    else{
        resetModes();
        ui->AdjacencyButton->setVisible(true);
        ui->IncidenceButton->setVisible(true);
        ui->ListButton->setVisible(true);
        ui->modeLabel->setText(QString("Current Mode: Displaying representations"));
        textDisplayMode = true;
    }
    update();
}











//drawing
void MainWindow::paintEvent(QPaintEvent *e)
{

    QPainter painter(this);

    if(graphicDisplayMode)
        drawGraph(painter);
    else if(textDisplayMode)
        drawRepresentations(painter);
    else if(KosarajMode){
        drawStrongComponents(painter);
    }
    else if(generateConnectedMode){
        drawStrongConnected(painter);
    }
    else if(BellmanFordMode){
        drawBellman(painter);
    }
    else if(JohnsonMode){
        drawJohnsonMatrix(painter);
    }

}
void MainWindow::drawGraph(QPainter &painter)
{

    QSize size = ui->drawingwidget->size();
    QFont font;
    font.setPixelSize(25-graph->getSize()/2);

    int centreX = size.width()/2;
    int centreY = size.height()/2;

    double base_degrees = (360. / graph->getSize()) * M_PI / 180;
    painter.setPen(Qt::black);
    painter.setFont(font);
    painter.setBrush(Qt::white);

    int R = 50 - graph->getSize()/2;


    painter.setBrush(Qt::black);
    std::vector<std::pair<int,int>> connections = graph->getConnections();
    std::map<std::pair<int,int>,int> wages = graph->getWages();


    for(auto & pr : connections){
        QPoint p1(0.9*centreX + cos(pr.first*base_degrees)*centreX/1.5 + R/2, 0.9*centreY + sin(pr.first*base_degrees)*centreY/1.5 + R/1.5);
        QPoint p2(0.9*centreX + cos(pr.second*base_degrees)*centreX/1.5 + R/2, 0.9*centreY + sin(pr.second*base_degrees)*centreY/1.5 + R/1.5);
        painter.drawLine(p1,p2);

        double angle = atan2(p2.y() - p1.y(), p2.x() - p1.x());
        QPoint po0 = p2 - QPoint(cos(angle) * R/2, sin(angle) * R/2);

        angle += -M_PI/32.;

        QPoint po1(p2.x() - R * cos(angle + M_PI/16),p2.y() - R * sin(angle + M_PI/16));
        angle -= 2 * M_PI/32.;
        QPoint po2(p2.x() - R * cos(angle + M_PI/16),p2.y() - R * sin(angle + M_PI/16));

        QPolygon poli;
        poli << po0 << po1 << po2;
        painter.drawPolygon(poli);

        if(graph->isStrongConnected()){
            QPoint p3 = (p1+p2)/2;
            p3 = (p3 + p2)/2;
            painter.setPen(Qt::red);
            painter.drawText(p3,QString::number(wages[pr]));
            painter.setPen(Qt::black);
        }



    }

    painter.setBrush(Qt::white);
    for(int i=0;i<graph->getSize();++i){
        painter.drawEllipse(0.9*centreX + cos(i*base_degrees)*centreX/1.5, 0.9*centreY + sin(i*base_degrees)*centreY/1.5 + R/8,R,R);

        if(i < 9)
            painter.drawText(0.9*centreX + cos(i*base_degrees)*centreX/1.5 + R/2 - 5, 0.9*centreY + sin(i*base_degrees)*centreY/1.5 + R/1.5 + R/8,QString::number(i+1));
        else
            painter.drawText(0.9*centreX + cos(i*base_degrees)*centreX/1.5 + R/2 - 10, 0.9*centreY + sin(i*base_degrees)*centreY/1.5 + R/1.5 + R/8,QString::number(i+1));

    }


    //



    QPoint pos = ui->rightwidget->pos();
    QSize rwidget = ui->rightwidget->size();
    int x_start = pos.x();
    int y_start = pos.y();

    painter.drawLine(x_start+10, y_start + 10, x_start + 10, y_start + rwidget.height() - 10);
    painter.drawLine(x_start+rwidget.width()-10, y_start + 10, x_start+rwidget.width()-10, y_start + rwidget.height() - 10);
    painter.drawLine(x_start+10, y_start + 10, x_start+rwidget.width()-10,y_start+10);
    painter.drawLine(x_start+10, y_start + rwidget.height() - 10, x_start+rwidget.width()-10,y_start + rwidget.height() - 10);

    if(!graph->isStrongConnected()){
        font.setPixelSize(10);
        painter.setFont(font);
        int it = 0;
        double step = rwidget.width() / 3.;
        int x_start_org = x_start;

        for(auto & pr: connections){
            painter.drawText(x_start + 20, y_start + 30,QString::number(pr.first+1) + " -> " + QString::number(pr.second+1));
            if(it == 2){
                y_start += 15;
                x_start = x_start_org;
                it = 0;
            }
            else{
                x_start += step;
                ++it;
            }


        }
    }
    else{
        font.setPixelSize(10);
        painter.setFont(font);
        int it = 0;
        double step = rwidget.width() / 3.;
        int x_start_org = x_start;
        std::map<std::pair<int,int>,int> wages = graph->getWages();

        for(auto & pr: connections){
            painter.drawText(x_start + 20, y_start + 30,QString::number(pr.first+1) + " -> " + QString::number(pr.second+1) + "[" +QString::number(wages[pr]) +"]");
            if(it == 2){
                y_start += 15;
                x_start = x_start_org;
                it = 0;
            }
            else{
                x_start += step-5;
                ++it;
            }
        }
    }



}
void MainWindow::drawRepresentations(QPainter &painter)
{
    if(adja){
        QSize size = ui->drawingwidget->size();
        painter.setPen(Qt::black);
        QFont font;
        font.setPixelSize(12);
        painter.setFont(font);
        std::vector<Vertex> vertexes = graph->getVertexes();


        int const VERTEXNUM = graph->getSize();

        int stepX = (size.width()-40) / VERTEXNUM;
        int stepY = (size.height()-40) /VERTEXNUM;

        //krawedzie
        for(int i=0;i<VERTEXNUM+1;++i){
            painter.drawLine(QPoint(40,stepY*i+40),QPoint(stepX*VERTEXNUM+40,stepY*i+40));
            painter.drawLine(QPoint(stepX*i + 40,40),QPoint(stepX*i+40,stepY*VERTEXNUM+40));
        }
        //numeracja
        for(int i=0;i<VERTEXNUM;++i){
            painter.drawText(QPoint((stepX*i + 40)  + stepX/2,20),QString::number(i+1));
            painter.drawText(QPoint(20,(stepY*i+40) + stepY/2),QString::number(i+1));
        }

        font.setPixelSize(18);
        painter.setFont(font);
        for(int i=0;i<VERTEXNUM;++i){

            std::vector<int> neigh = vertexes[i].getNeigh();
            int neighIt = 0;

            for(int j=0;j<VERTEXNUM;++j){
                if(!neigh.empty() && j == neigh[neighIt]){
                    painter.drawText(QPoint(stepX*j+40 + stepX/2.5,stepY*i+40 + stepY/1.6),"1");
                    if(neighIt < neigh.size()-1)
                        ++neighIt;
                }
                else{
                    painter.drawText(QPoint(stepX*j+40 + stepX/2.5-5,stepY*i+40 + stepY/1.6),"0");
                }
            }
        }





    }
    else if(inci){

        QSize size = ui->drawingwidget->size();
        painter.setPen(Qt::black);
        QFont font;
        font.setPixelSize(12);
        painter.setFont(font);
        std::vector<std::pair<int,int>> connections = graph->getConnections();


        int const VERTEXNUM = graph->getSize();

        int stepX = (size.width()-40) / connections.size();
        int stepY = (size.height()-40) /VERTEXNUM;

        //krawedzie
        for(int i=0;i<VERTEXNUM+1;++i){
            painter.drawLine(QPoint(40,stepY*i+40),QPoint(stepX*connections.size()+40,stepY*i+40));
        }
        for(int i=0;i<connections.size()+1;++i){
            painter.drawLine(QPoint(stepX*i + 40,40),QPoint(stepX*i+40,stepY*VERTEXNUM+40));
        }
        //numeracja
        for(int i=0;i<VERTEXNUM;++i){
            painter.drawText(QPoint(20,(stepY*i+40) + stepY/2),QString::number(i+1));
        }
        for(int i=0;i<connections.size();++i){
            painter.drawText(QPoint((stepX*i + 40)  + stepX/2,20),QString::number(i+1));
        }

        //pomocniczy matrix
        int **arr = new int*[VERTEXNUM];
        for(int i=0;i<VERTEXNUM;++i) arr[i] = new int[connections.size()];

        for(int i=0;i<VERTEXNUM;++i)
            for(int j=0;j<connections.size();++j)
                arr[i][j] = 0;

        int iter = 0;
        for(auto &pr : connections){
            arr[pr.first][iter] = 1;
            arr[pr.second][iter] = -1;
            ++iter;
        }




        font.setPixelSize(16);
        painter.setFont(font);
        for(int i=0;i<VERTEXNUM;++i){
            for(int j=0;j<connections.size();++j){
                painter.drawText(QPoint(stepX*j+40 + stepX/2.5,stepY*i+40 + stepY/1.8),QString::number(arr[i][j]));
            }
        }

        for(int i=0;i<VERTEXNUM;++i) delete[] arr[i];
        delete[] arr;

    }
    else if(adjalist){
        QSize size = ui->drawingwidget->size();
        painter.setPen(Qt::black);
        QFont font;
        font.setPixelSize(12);
        painter.setFont(font);
        std::vector<Vertex> vertexes = graph->getVertexes();

        int const VERTEXNUM = graph->getSize();
        int stepX = (size.width()-40) / VERTEXNUM;
        int stepY = (size.height()-40) /VERTEXNUM;
        int stepX_org = stepX;

        for(int i=0;i<VERTEXNUM+1;++i){
            painter.drawLine(QPoint(40,stepY*i+40),QPoint(stepX*VERTEXNUM+40,stepY*i+40));
        }
        painter.drawLine(QPoint(40,40),QPoint(40,stepY*VERTEXNUM+40));
        painter.drawLine(QPoint(stepX*VERTEXNUM + 40,40),QPoint(stepX*VERTEXNUM+40,stepY*VERTEXNUM+40));

        font.setPixelSize(stepY/4);
        painter.setFont(font);

        for(int i=0;i<VERTEXNUM;++i){
            std::vector<int> v = vertexes[i].getNeigh();
            painter.drawText(QPoint(40 + stepX/2.5,stepY*i+40 + stepY/1.8),QString::number(i+1) + ":");
            for(auto &n : v){
                painter.drawText(QPoint(stepX+40 + stepX/2.5,stepY*i+40 + stepY/1.8),QString::number(n+1));
                stepX+=stepY/8;
            }
            stepX = stepX_org;

        }

    }

}
void MainWindow::drawStrongComponents(QPainter &painter)
{
    QSize size = ui->drawingwidget->size();
    QFont font;
    font.setPixelSize(25-graph->getSize()/2);

    int centreX = size.width()/2;
    int centreY = size.height()/2;

    double base_degrees = (360. / graph->getSize()) * M_PI / 180;
    painter.setPen(Qt::black);
    painter.setFont(font);
    painter.setBrush(Qt::white);

    int R = 50 - graph->getSize()/2;

    painter.setBrush(Qt::black);
    std::vector<std::pair<int,int>> connections = graph->getConnections();
    for(auto & pr : connections){
        QPoint p1(0.9*centreX + cos(pr.first*base_degrees)*centreX/1.5 + R/2, 0.9*centreY + sin(pr.first*base_degrees)*centreY/1.5 + R/1.5);
        QPoint p2(0.9*centreX + cos(pr.second*base_degrees)*centreX/1.5 + R/2, 0.9*centreY + sin(pr.second*base_degrees)*centreY/1.5 + R/1.5);
        painter.drawLine(p1,p2);

        double angle = atan2(p2.y() - p1.y(), p2.x() - p1.x());
        QPoint po0 = p2 - QPoint(cos(angle) * R/2, sin(angle) * R/2);

        angle += -M_PI/32.;

        QPoint po1(p2.x() - R * cos(angle + M_PI/16),p2.y() - R * sin(angle + M_PI/16));
        angle -= 2 * M_PI/32.;
        QPoint po2(p2.x() - R * cos(angle + M_PI/16),p2.y() - R * sin(angle + M_PI/16));

        QPolygon poli;
        poli << po0 << po1 << po2;
        painter.drawPolygon(poli);
    }



    QBrush brushes[] = {Qt::red, Qt::blue, Qt::green, Qt::magenta, Qt::yellow, Qt::cyan, Qt::lightGray};


    int colorIt = 0;
    for(auto &comp : graph->getStrongComponents()){
        painter.setBrush(brushes[colorIt]);
        for(auto &n : comp){
            painter.drawEllipse(0.9*centreX + cos(n*base_degrees)*centreX/1.5, 0.9*centreY + sin(n*base_degrees)*centreY/1.5 + R/8,R,R);
            if(n < 9)
                painter.drawText(0.9*centreX + cos(n*base_degrees)*centreX/1.5 + R/2 - 5, 0.9*centreY + sin(n*base_degrees)*centreY/1.5 + R/1.5+ R/8,QString::number(n+1));
            else
                painter.drawText(0.9*centreX + cos(n*base_degrees)*centreX/1.5 + R/2 - 10, 0.9*centreY + sin(n*base_degrees)*centreY/1.5 + R/1.5+ R/8,QString::number(n+1));
        }
        if(colorIt < 6)
            ++colorIt;
    }



}
void MainWindow::drawStrongConnected(QPainter &painter)
{

    if(graph->isStrongConnected()){
        QSize size = ui->drawingwidget->size();
        QFont font;
        font.setPixelSize(25-graph->getSize()/2);

        int centreX = size.width()/2;
        int centreY = size.height()/2;

        double base_degrees = (360. / graph->getSize()) * M_PI / 180;
        painter.setPen(Qt::black);
        painter.setFont(font);
        painter.setBrush(Qt::white);

        std::map<std::pair<int,int>,int> wages = graph->getWages();
        int R = 50 - graph->getSize()/2;

        painter.setBrush(Qt::black);
        std::vector<std::pair<int,int>> connections = graph->getConnections();
        for(auto & pr : connections){
            QPoint p1(0.9*centreX + cos(pr.first*base_degrees)*centreX/1.5 + R/2, 0.9*centreY + sin(pr.first*base_degrees)*centreY/1.5 + R/1.5);
            QPoint p2(0.9*centreX + cos(pr.second*base_degrees)*centreX/1.5 + R/2, 0.9*centreY + sin(pr.second*base_degrees)*centreY/1.5 + R/1.5);
            painter.drawLine(p1,p2);

            double angle = atan2(p2.y() - p1.y(), p2.x() - p1.x());
            QPoint po0 = p2 - QPoint(cos(angle) * R/2, sin(angle) * R/2);

            angle += -M_PI/32.;

            QPoint po1(p2.x() - R * cos(angle + M_PI/16),p2.y() - R * sin(angle + M_PI/16));
            angle -= 2 * M_PI/32.;
            QPoint po2(p2.x() - R * cos(angle + M_PI/16),p2.y() - R * sin(angle + M_PI/16));

            QPolygon poli;
            poli << po0 << po1 << po2;
            painter.drawPolygon(poli);


            QPoint p3 = (p1+p2)/2;
            p3 = (p3 + p2)/2;
            painter.setPen(Qt::red);
            painter.drawText(p3,QString::number(wages[pr]));
            painter.setPen(Qt::black);

        }

        painter.setBrush(Qt::white);
        for(int i=0;i<graph->getSize();++i){
            painter.drawEllipse(0.9*centreX + cos(i*base_degrees)*centreX/1.5, 0.9*centreY + sin(i*base_degrees)*centreY/1.5 + R/8,R,R);

            if(i < 9)
                painter.drawText(0.9*centreX + cos(i*base_degrees)*centreX/1.5 + R/2 - 5, 0.9*centreY + sin(i*base_degrees)*centreY/1.5 + R/1.5 + R/8,QString::number(i+1));
            else
                painter.drawText(0.9*centreX + cos(i*base_degrees)*centreX/1.5 + R/2 - 10, 0.9*centreY + sin(i*base_degrees)*centreY/1.5 + R/1.5 + R/8,QString::number(i+1));

        }


        QPoint pos = ui->rightwidget->pos();
        QSize rwidget = ui->rightwidget->size();
        int x_start = pos.x();
        int y_start = pos.y();

        painter.drawLine(x_start+10, y_start + 10, x_start + 10, y_start + rwidget.height() - 10);
        painter.drawLine(x_start+rwidget.width()-10, y_start + 10, x_start+rwidget.width()-10, y_start + rwidget.height() - 10);
        painter.drawLine(x_start+10, y_start + 10, x_start+rwidget.width()-10,y_start+10);
        painter.drawLine(x_start+10, y_start + rwidget.height() - 10, x_start+rwidget.width()-10,y_start + rwidget.height() - 10);

        font.setPixelSize(10);
        painter.setFont(font);
        int it = 0;
        double step = rwidget.width() / 3.;
        int x_start_org = x_start;


        for(auto & pr: connections){
            painter.drawText(x_start + 20, y_start + 30,QString::number(pr.first+1) + " -> " + QString::number(pr.second+1) + "[" +QString::number(wages[pr]) +"]");
            if(it == 2){
                y_start += 15;
                x_start = x_start_org;
                it = 0;
            }
            else{
                x_start += step-5;
                ++it;
            }
        }
    }


}





void MainWindow::on_AdjacencyButton_clicked()
{
    adja=true;
    adjalist=false;
    inci=false;
    update();
}

void MainWindow::on_IncidenceButton_clicked()
{
    adja=false;
    adjalist=false;
    inci=true;
    update();
}

void MainWindow::on_ListButton_clicked()
{
    adja=false;
    adjalist=true;
    inci=false;
    update();
}

void MainWindow::on_KosarajButton_clicked()
{
    if(anyMode()){
       if(KosarajMode){
           KosarajMode=false;
            ui->modeLabel->setText(QString("Current Mode: None"));
            update();
        }
       return;
    }



    if(graph->isGenerated()){
        ui->modeLabel->setText(QString("Current Mode: Strongest Components"));
        KosarajMode = true;
        if(!graph->doneKosaraju())
            graph->Kosaraju();
        update();
    }



}



void MainWindow::on_generateStrongbutton_clicked()
{
    if(anyMode()){
       if(generateConnectedMode){
           generateConnectedMode=false;
           ui->menuGraphwidget->setVisible(false);
            ui->modeLabel->setText(QString("Current Mode: None"));
            update();
        }
       return;
    }


    generateConnectedMode = true;
    graph->disableConnected();
    ui->modeLabel->setText(QString("Current Mode: generating strongly connected Graph"));
    ui->menuGraphwidget->setVisible(true);


}

void MainWindow::on_bellmanFordButton_clicked()
{
    if(anyMode()){
       if(BellmanFordMode){
           BellmanFordMode=false;
           ui->modeLabel->setText(QString("Current Mode: None"));
           update();
        }
       return;
    }

    \
    if(graph->isStrongConnected()){
        graph->disableFord();
        ui->modeLabel->setText(QString("Current Mode: Bellman-Ford"));
        BellmanFordMode = true;
        ui->bellmanWidget->setVisible(true);
        update();

    }

}

void MainWindow::on_inBellmanButton_clicked()
{
    QString str = ui->bellmantext->toPlainText();
    if(!str[0].isDigit())
        return;
    int dig = str.toInt();
    if(dig < 1 || dig > graph->getSize())
       return;

    update();
    graph->BellmanFord(dig-1);
    ui->bellmanWidget->setVisible(false);
    update();

}

void MainWindow::on_inBellmanShowButton_clicked()
{
    ui->bellmanWidget->setVisible(false);
    graph->enableFord();
    update();
}

void MainWindow::drawBellman(QPainter &painter)
{
    if(graph->isBellmanDone()){
        if(!graph->BellmanFound()){

            QSize size = ui->drawingwidget->size();
            QFont font;
            font.setPixelSize(20);
            painter.setFont(font);
            painter.drawText(size.width()/4,size.height()/3,"Negative cycle, generate graph again");

            return;
        }
        std::map<std::pair<int,int>,int> wagesGraph = graph->getWages();

        QSize size = ui->drawingwidget->size();
        QFont font;
        font.setPixelSize(25-graph->getSize()/2);

        int centreX = size.width()/2;
        int centreY = size.height()/2;

        double base_degrees = (360. / graph->getSize()) * M_PI / 180;
        painter.setPen(Qt::black);
        painter.setFont(font);
        painter.setBrush(Qt::white);

        int R = 50 - graph->getSize()/2;

        painter.setBrush(Qt::black);
        std::vector<std::pair<int,int>> connections = graph->getConnections();
        for(auto & pr : connections){
            QPoint p1(0.9*centreX + cos(pr.first*base_degrees)*centreX/1.5 + R/2, 0.9*centreY + sin(pr.first*base_degrees)*centreY/1.5 + R/1.5);
            QPoint p2(0.9*centreX + cos(pr.second*base_degrees)*centreX/1.5 + R/2, 0.9*centreY + sin(pr.second*base_degrees)*centreY/1.5 + R/1.5);
            painter.drawLine(p1,p2);

            double angle = atan2(p2.y() - p1.y(), p2.x() - p1.x());
            QPoint po0 = p2 - QPoint(cos(angle) * R/2, sin(angle) * R/2);

            angle += -M_PI/32.;

            QPoint po1(p2.x() - R * cos(angle + M_PI/16),p2.y() - R * sin(angle + M_PI/16));
            angle -= 2 * M_PI/32.;
            QPoint po2(p2.x() - R * cos(angle + M_PI/16),p2.y() - R * sin(angle + M_PI/16));

            QPolygon poli;
            poli << po0 << po1 << po2;
            painter.drawPolygon(poli);


            QPoint p3 = (p1+p2)/2;
            p3 = (p3 + p2)/2;
            painter.setPen(Qt::red);
            painter.drawText(p3,QString::number(wagesGraph[pr]));
            painter.setPen(Qt::black);


        }

        painter.setBrush(Qt::white);
        for(int i=0;i<graph->getSize();++i){
            painter.drawEllipse(0.9*centreX + cos(i*base_degrees)*centreX/1.5, 0.9*centreY + sin(i*base_degrees)*centreY/1.5 + R/8,R,R);

            if(i < 9)
                painter.drawText(0.9*centreX + cos(i*base_degrees)*centreX/1.5 + R/2 - 5, 0.9*centreY + sin(i*base_degrees)*centreY/1.5 + R/1.5+ R/8,QString::number(i+1));
            else
                painter.drawText(0.9*centreX + cos(i*base_degrees)*centreX/1.5 + R/2 - 10, 0.9*centreY + sin(i*base_degrees)*centreY/1.5 + R/1.5+ R/8,QString::number(i+1));

        }


        QPoint pos = ui->rightwidget->pos();
        QSize rwidget = ui->rightwidget->size();
        int x_start = pos.x();
        int x_start_org = x_start;
        int y_start = pos.y();

        painter.drawLine(x_start+10, y_start + 10, x_start + 10, y_start + rwidget.height() - 10);
        painter.drawLine(x_start+rwidget.width()-10, y_start + 10, x_start+rwidget.width()-10, y_start + rwidget.height() - 10);
        painter.drawLine(x_start+10, y_start + 10, x_start+rwidget.width()-10,y_start+10);
        painter.drawLine(x_start+10, y_start + rwidget.height() - 10, x_start+rwidget.width()-10,y_start + rwidget.height() - 10);

        font.setPixelSize(16);
        painter.setFont(font);


        std::vector<std::vector<int>> paths = graph->getVertexes()[graph->getBellmanIndex()].getPaths(); // XD
        std::map<int,int> wages = graph->getVertexes()[graph->getBellmanIndex()].getWeights();

        for(int i=0;i<graph->getSize();++i){
            QString str;
            for(int j=paths[i].size()-1;j >= 0;--j){
                str.append(QString::number(paths[i][j]+1) + " ");
            }

            painter.drawText(x_start + 20, y_start + 30,QString::number(i+1) + ": " +str);
            x_start+=paths[i].size() * 16;
            painter.setPen(Qt::red);
            painter.drawText(x_start + 40, y_start + 30,"[" + QString::number(wages[i]) + "]");
            painter.setPen(Qt::black);

            y_start+=20;
            x_start = x_start_org;

        }
    }
}

void MainWindow::on_Johnsonbutton_clicked()
{
    if(anyMode()){
       if(JohnsonMode){
           JohnsonMode=false;
           ui->modeLabel->setText(QString("Current Mode: None"));
           update();
        }
       return;
    }

    \
    if(graph->isStrongConnected()){
       if(!graph->doneJohnson())
            graph->Johnson();

        ui->modeLabel->setText(QString("Current Mode: Johnson"));
       JohnsonMode = true;
        update();

    }

}

void MainWindow::drawJohnsonMatrix(QPainter &painter)
{
    if(graph->doneJohnson()){
        if(!graph->JohnsonFound()){

            QSize size = ui->drawingwidget->size();
            QFont font;
            font.setPixelSize(20);
            painter.setFont(font);
            painter.drawText(size.width()/4,size.height()/3,"Negative cycle, generate graph again");

            return;
        }

        QSize size = ui->drawingwidget->size();
        painter.setPen(Qt::black);
        QFont font;
        font.setPixelSize(12);
        painter.setFont(font);
        std::vector<Vertex> vertexes = graph->getVertexes();


        int const VERTEXNUM = graph->getSize();

        int stepX = (size.width()-40) / VERTEXNUM;
        int stepY = (size.height()-40) /VERTEXNUM;

        //krawedzie
        for(int i=0;i<VERTEXNUM+1;++i){
            painter.drawLine(QPoint(40,stepY*i+40),QPoint(stepX*VERTEXNUM+40,stepY*i+40));
            painter.drawLine(QPoint(stepX*i + 40,40),QPoint(stepX*i+40,stepY*VERTEXNUM+40));
        }
        //numeracja
        for(int i=0;i<VERTEXNUM;++i){
            painter.drawText(QPoint((stepX*i + 40)  + stepX/2,20),QString::number(i+1));
            painter.drawText(QPoint(20,(stepY*i+40) + stepY/2),QString::number(i+1));
        }

        std::vector<std::vector<int>> m_johnson = graph->getJohnsonMatrix();
        font.setPixelSize(18);
        painter.setFont(font);
        for(int i=0;i<VERTEXNUM;++i){
            for(int j=0;j<VERTEXNUM;++j){
                painter.drawText(QPoint(stepX*j+40 + stepX/2.5,stepY*i+40 + stepY/1.6),QString::number(m_johnson[i][j]));
            }
        }

    }

}
