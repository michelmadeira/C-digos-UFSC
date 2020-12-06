#include "RPMGauge.h"

RPMGauge::RPMGauge()
{

    QPixmap gaugePixmap(":/resources/assets/RPM_GAUGE.png");                        // Carrega a imagem através da classe QPixmap (processamento de pixels). O endereço está relativo a pasta do programa através do recurso "Resources".
    Gauge = new QGraphicsPixmapItem(gaugePixmap);                                   // Cria um ponteiro para QGraphicsPixMapItem a partir do pixmap gerado na linha anterior.
    Gauge->setTransformationMode(Qt::TransformationMode::SmoothTransformation);     // Escolhe transformações de alta qualidade.
    Gauge->setTransformOriginPoint(gaugePixmap.width()/2,gaugePixmap.height()/2);   // Realoca o ponto de referência para transformações (ponto âncora) para o centro da imagem.
    Gauge->setPos(-190,65);

    QPixmap rpmPointerpixmap(":/resources/assets/RPM_POINTER.png");
    Pointer = new QGraphicsPixmapItem(rpmPointerpixmap);
    Pointer->setTransformationMode(Qt::TransformationMode::SmoothTransformation);
    Pointer->setTransformOriginPoint(81,56);                                        // Realoca o anchor point o centro do ponteiro. (Para que ele gire em torno de seu eixo)
    Pointer->setPos(-93,175);                                                       // Realoca o ponteiro para o centro do mostrador.

}

