
/* o Construtor da classe CSV recebe o caminho do arquivo coletado pelo usuário através da interface do programa e processa os dados, linha a linha
 gerando os dados de interesse para a telemetria (Tempo, RPM, Pressão do Turbo (MAP), TPS (Throttle Position Sensor), estado do Two-step. */

#include <QFile>
#include <QString>
#include <QMessageBox>

#include "csv_read.h"

//QVector<float> CSV_read::twostep;   //declaração das variáveis estaticas.
QVector<float> CSV_read::Time;
QVector<int> CSV_read::RPM;
QVector<float> CSV_read::MAP;
QVector<float> CSV_read::TPS;
QVector<QString> CSV_read::TPSText;


CSV_read::CSV_read(const QString &filename) //o construtor da classe CSV_read recebe o caminho do arquivo coletado pelo usuário através da interface do progarama.
{
    QFile file(filename);

        if (!file.open(QIODevice::ReadOnly)) //testa se o arquivo foi aberto com sucesso, caso não, exibe uma mensagem de erro para o usuário
        {
            QMessageBox::critical(nullptr, "File", "ERROR: Cannot open file");
            return;
        }

        QString str = file.readLine();                                              //Lê a primeira linha e armazena em str.
        str.remove(QChar('"'));                                                     //remove as as aspas duplas.
        QChar separator;

        if(str.contains(QChar(';'))){                                               //define o separador de campo do arquivo .CSV a ser lido (normalmente , ou ;).
            separator = ';';
        }
            else{
                separator =',';
        }


        QStringList list = str.split(QChar(separator));                             //separa a primeira linha em uma lista de Strings quando encontra o caractere separador no arquivo.

        /*
        Percorre a lista e testa se o arquivo contém os cabeçalhos rpm, map e tps. Caso não exibe uma mensagem pro usuário: mudar o nome dessas variáveis pelo programa da injeção eletrônica.
        OBS: o valor do tempo não precisa ser encontrado porque ele sempre será a primeira coluna do arquivo. index = 0.
        Toda esta etapa inicial foi necessária porque analisando vários exemplos de arquivos de carros diferentes, constatou que a estrutura de dados variava de carro pra carro.
        Alguns tinham a variável como "RPM" outros tinham como "Rotacao", além disso as vezes em colunas diferentes também.
        Procurou-se deixar o leitor compatível com o maior número de arquivos .CSV possível.
        */


        for (int i=0; i < list.size() ; i++)
            {

                if(list[i].contains("two-step",Qt::CaseInsensitive) || list[i].contains("two step",Qt::CaseInsensitive) || list[i].contains("2 step",Qt::CaseInsensitive)){
                    index2step = i;
                    found2step= true;
                }

                if (list[i].contains("rpm",Qt::CaseInsensitive) || list[i].contains("Rotação",Qt::CaseInsensitive) || list[i].contains("Rotacao",Qt::CaseInsensitive)) {
                    indexRPM = i;
                    foundRPM = true;
                }

                if(list[i].contains("map",Qt::CaseInsensitive) || list[i].contains("turbo",Qt::CaseInsensitive)) {
                    indexMAP = i;
                    foundMAP = true;
                }

                if(list[i].contains("tps",Qt::CaseInsensitive) || list[i].contains("pedal",Qt::CaseInsensitive)) {
                    indexTPS = i;
                    foundTPS = true;
                }
            }


            if (found2step == false){
                QMessageBox::warning(nullptr,"two step","Can´t find Two step signal. Proceeding, but there is no reference to sync telemetry to the video");

            }
            if (foundRPM == false){
                QMessageBox::critical(nullptr, "RPM", "Can´t find RPM. Change variable name trought FT datalogger");
                return;
            }
            if (foundMAP == false){
                QMessageBox::critical(nullptr, "MAP", "Can´t find MAP. Change variable name trought FT datalogger");
                return;
            }
            if (foundTPS == false){
                QMessageBox::critical(nullptr, "TPS", "Can´t find TPS. Change variable name trought FT datalogger");
                return;
            }


            for (int i=0; !file.atEnd(); i++)       //percorre todo o arquivo a partir da segunda linha.
            {

                str = file.readLine();
                str.remove(QChar('"'));
                QStringList list = str.split(separator);

                Time.push_back(list[0].toFloat()*1000); //Chama o método push_back dos vetores, como o argumento sendo convertido para float e multiplicado por 1000 para ser transformado em milisegundos (unidade que o Qt usa em suas funções).
                RPM.push_back(list[indexRPM].toInt());  //Repete para as outras variáveis, convertendo para tipos convenientes.
                MAP.push_back(list[indexMAP].toFloat());
                TPS.push_back(list[indexTPS].toFloat());

                list[indexTPS].remove('-');             //Faz um ajuste na série TPS para gerar um texto formatado.
                list[indexTPS].remove('.');
                if(list[indexTPS]< 10){
                    list[indexTPS].resize(1);
                }
                list[indexTPS].resize(2);

                TPSText.push_back(list[indexTPS]);
                //twostep.push_back(list[index2step].toFloat());

            }

    return;
}
