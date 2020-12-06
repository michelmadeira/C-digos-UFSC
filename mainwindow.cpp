#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "csv_read.h"
#include "RPMGauge.h"
#include "boostgauge.h"
#include "gaspedal.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QString>
#include <QDebug>
#include <QTimer>
#include <QDesktopServices>

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QUrl>
#include <QProcess>

/* As variáveis estáticas foram utilizadas como auxiliares para percorrer a série de dados e para salvar alguns estados por entre escopos.
 * Solução encontrada pois não consegue-se passar argumentos em SLOTS. Seria preciso usar funções lambda, mas o desenvolvimento já caminhava sem esse recurso e optou-se por mentê-lo
 * apesar de reconhecermentos que poderiam ser usados Sinais e outros recursos do QT.
*/

int MainWindow::counterSet1;
int MainWindow::counterSet2;

bool MainWindow::animationStateRpm;
bool MainWindow::animationStateBoost;


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    QPixmap logo(":/resources/assets/BG.png");          //Carrega a imagem de fundo
    QIcon icon(":/resources/assets/ICON.png");          //Carrega o ícone

    ui->labelLogo->setPixmap(logo);                     // utiliza a função membro do elemento labellogo para gerar o pixmap com a imagem de fundo.
    ui->graphicView->setVisible(false);                 // desliga os elementos da interface de acordo com esta fase inicial da execução do programa.
    ui->pushButton->setEnabled(false);
    ui->toolBar->setVisible(false);
    ui->OK->setVisible(false);
    ui->textBrowser->setVisible(false);
    ui->pushButton->setToolTip("Avaiable after file processing");
    ui->runButton->setVisible(false);
    ui->textTps->setVisible(false);
    ui->Back->setVisible(false);

    ui->labelLogo->setGeometry(MainWindow::rect());     //Seta o tamanho do elemento labellogo para coincidir com o tamanho da janela do programa.
    MainWindow::setWindowIcon(icon);                    //Seta o ícone da janela do programa como o ícone carregado.
    MainWindow::setFixedSize(MainWindow::geometry().width(),MainWindow::geometry().height()); //Seta tamanho fixo da janela, pro usuário não alterar a diagramação da interface.

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_toolButton_clicked()
{

    QString filename=QFileDialog::getOpenFileName(this,"Open File","c:\\","*.csv");  //Procura pelo arquivo .CSV, restringindo ao seu tipo.
    ui->lineEdit->setText(filename);

    if(ui->lineEdit->text() == NULL){
        return;
    }

    CSV_read csv_read(filename);
    ui->pushButton->setEnabled(true);                       //Libera o próximo passo depois que o arquivo já está pronto.

}

void MainWindow::on_pushButton_clicked()
{
    //Esta função gera uma animação incial nos mostradores. Como quando você liga o seu carro.


    ui->menubar->setVisible(false);                         //Desliga elementos da interface de acordo com a conveniência para esta fase do programa.
    ui->labelLogo->setVisible(false);
    ui->lineEdit->setVisible(false);
    ui->pushButton->setVisible(false);
    ui->TextOpenCsv->setVisible(false);
    ui->toolButton->setVisible(false);
    ui->graphicView->setVisible(true);
    ui->pushButton->setVisible(false);

    QGraphicsScene *graphic = new QGraphicsScene (this);    //Classe utilizada para compor imagens 2d em uma cena.
    RPMGauge r;
    boostGauge g;
    GasPedal p;

    graphic->addItem(p.Pedal);                              //Adiciona os elementos a cena.
    graphic->addItem(g.Gauge);
    graphic->addItem(r.Gauge);
    graphic->addItem(g.Pointer);
    graphic->addItem(r.Pointer);
    graphic->addRect(-300,-100,600,530);                    //Adiciona um retângulo auxiliar que envolve todos os itens, para evitar reescalonamento e mudança de posição da janela, quando
    graphic->items().at(0)->setOpacity(0);                  //o item "pedal" era escalanolado. Deixa o retângulo invisível.

    r.Pointer->setRotation(-47);                            //Ajusta a rotação dos ponteiros para os respectivos Zeros nos mostradores.
    g.Pointer->setRotation(-65);

    ui->graphicView->setScene(graphic);
    ui->graphicView->setStyleSheet("background:transparent;");


    MainWindow::counterSet1=-47;                            //Estado inicial dos ponteiros. Para RPM intervalo de rotação [-47, - 159]. Para MAP [ 1.5, -175]
    MainWindow::counterSet2=-65;
    MainWindow::animationStateRpm=0;                        //Variáves auxiliares de controle da animação.
    MainWindow::animationStateBoost=0;

    for(int i=10;i < 120 ;i++){
        QTimer::singleShot(CSV_read::Time[i], this, SLOT(set()));  //Seta os timers que chamam a função que modifica a rotação dos ponteiros, aproveitando-se a série tempo.
    }    

    return;


}

void MainWindow::set()
{
    // Executa a animação inicial dos mostradores

    QGraphicsScene *graphic = new QGraphicsScene (this);

    RPMGauge r;
    boostGauge g;
    GasPedal p;

    r.Pointer->setRotation(MainWindow::counterSet1);
    g.Pointer->setRotation(MainWindow::counterSet2);

    if (MainWindow::counterSet1 > 159)
        MainWindow::animationStateRpm=1;

    if (animationStateRpm){
        if(MainWindow::counterSet1 > -47)
            MainWindow::counterSet1-=4;
        else
            ui->runButton->setVisible(true); //Libera o próximo passo quando a animação acabar.

    }
    else
       MainWindow::counterSet1+=4;

    if (MainWindow::counterSet2 > 175)
        MainWindow::animationStateBoost=1;

    if (animationStateBoost){
        if(MainWindow::counterSet2 > -65)
            MainWindow::counterSet2-=4.8;
    }
    else
       MainWindow::counterSet2+=4.8;

    graphic->addItem(p.Pedal);
    graphic->addItem(g.Gauge);
    graphic->addItem(r.Gauge);
    graphic->addItem(g.Pointer);
    graphic->addItem(r.Pointer);
    graphic->addRect(-300,-100,600,530);
    graphic->items().at(0)->setOpacity(0);

    ui->graphicView->setScene(graphic);    

}

void MainWindow::on_runButton_clicked()
{
    ui->runButton->setVisible(false);
    ui->textTps->setVisible(true);

    MainWindow::counterSet1=0;                                          //Seta a variável para o corresponder ao índice zero no arquivo.

    for(int i=0; i < CSV_read::Time.size(); i++){                       //Seta os timers que chamam a função que modifica a rotação dos ponteiros, para toda linha do vetor Tempo.
        QTimer::singleShot(CSV_read::Time[i], this, SLOT(run()));
    }
}

void MainWindow::run()
{

    QGraphicsScene *graphic = new QGraphicsScene (this);

    RPMGauge r;
    boostGauge g;
    GasPedal p;

    r.Pointer->setRotation((0.022888*CSV_read::RPM[MainWindow::counterSet1])-47);  //Funções do primeiro grau que coordenam a animação.
    g.Pointer->setRotation((96*CSV_read::MAP[MainWindow::counterSet1])+31);
    p.Pedal->setScale((-0.002*CSV_read::TPS[MainWindow::counterSet1])+0.6);
    ui->textTps->setText("% " + CSV_read::TPSText[MainWindow::counterSet1]);


    if (CSV_read::Time[counterSet1] == CSV_read::Time.last()){          //Libera o botão voltar quando todo o arquivo foi percorrido.
        ui->Back->setVisible(true);
    }

    MainWindow::counterSet1+=1;

    graphic->addItem(p.Pedal);
    graphic->addItem(g.Gauge);
    graphic->addItem(r.Gauge);
    graphic->addItem(g.Pointer);
    graphic->addItem(r.Pointer);
    graphic->addRect(-300,-100,600,530);
    graphic->items().at(0)->setOpacity(0);

    ui->graphicView->setScene(graphic);
    ui->graphicView->setStyleSheet("background:transparent;");

}


void MainWindow::on_actionAbout_triggered()
{
    ui->labelLogo->setVisible(false);           //Exibe as informações sobre o programa.
    ui->textBrowser->setVisible(true);
    ui->OK->setVisible(true);
    ui->lineEdit->setVisible(false);
    ui->TextOpenCsv->setVisible(false);
    ui->pushButton->setVisible(false);
    ui->toolButton->setVisible(false);

}

void MainWindow::on_OK_clicked()
{
    ui->labelLogo->setVisible(true);            //Volta para a tela inicial.
    ui->lineEdit->setVisible(true);
    ui->TextOpenCsv->setVisible(true);
    ui->pushButton->setVisible(true);
    ui->toolButton->setVisible(true);
    ui->textBrowser->setVisible(false);
    ui->OK->setVisible(false);

}

void MainWindow::on_actionReadme_triggered()
{
    QDesktopServices::openUrl(QUrl("https://drive.google.com/file/d/1cX4v_yBvv1gS5j-fMZtUBD2hePU4LPDx/view?usp=sharing"));  //Abre o arquivo externo README.
}

void MainWindow::on_Back_clicked()
{
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());    //Reseta o programa.
    exit(0);

}
