#include "gaspedal.h"

//Comentários disponíveis na classe RPMGauge (implementação similiar).

GasPedal::GasPedal()

{

    QPixmap gasPedalPixmap(":/resources/assets/GASPEDAL.png");
    Pedal = new QGraphicsPixmapItem(gasPedalPixmap);
    Pedal->setTransformationMode(Qt::TransformationMode::SmoothTransformation);
    Pedal->setTransformOriginPoint(gasPedalPixmap.width()/2,gasPedalPixmap.height()/2);
    Pedal->setPos(170,65);
    Pedal->setScale(0.6);

}
