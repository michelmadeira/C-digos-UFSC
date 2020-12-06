#ifndef CSV_READ_H
#define CSV_READ_H

#include <QString>
#include <QVector>

class CSV_read{

    private:

        int indexTime=0, indexRPM, indexMAP, indexTPS, index2step;  //variáves usadas para armazenar o índice em que os dados estão no arquivo .CSV.

    public:

        static QVector<float> Time;  //variáveis do tipo estáticas. Usada para que o gasto computacional
        static QVector<int> RPM;     //em se ler o arquivo, seja feito uma vez só. Uma vez executado o objeto gerado estará disponível
        static QVector<float> MAP;   //em todas as partes do código, e não somente no escopo da instanciação de um objeto da classe CSV_read.
        static QVector<float> TPS;
        static QVector<QString> TPSText;
        //static QVector<float> twostep; //Não foi necessário o uso da série Two Step, no entando ela encontra-se disponível para uma implementação futura.

        bool foundRPM,foundMAP,foundTPS,found2step; //variáveis usadas para armazenar se o construtor encontrou as séries de dados (RPM, MAP, TPS, 2STEP no arquivo).

        CSV_read(const QString&);

};

#endif // CSV_READ_H
