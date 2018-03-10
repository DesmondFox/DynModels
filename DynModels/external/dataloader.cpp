#include "dataloader.h"
#include <QFile>
#include <QTextStream>

DataLoader::DataLoader(QObject *parent)
    : QObject(parent),
      m_modelID(0), m_start(0.0),
      m_end(0.0), m_step(0.0)
{

}

void DataLoader::setPopulationValues(const QList<qreal> &values)
{
    m_populations = values;
}

void DataLoader::setModelID(const quint8 &modelID)
{
    m_modelID   = modelID;
}

void DataLoader::setCoefValues(const QList<qreal> &values)
{
    m_coefs = values;
}

void DataLoader::setIntegrationSettings(const qreal &start, const qreal &end, const qreal &step)
{
    m_start = start;
    m_end   = end;
    m_step  = step;
}

bool DataLoader::save(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly))
        return false;

    QTextStream strm(&file);
    strm << QString("IntStart=%1\n").arg(m_start);
    strm << QString("IntEnd=%1\n").arg(m_end);
    strm << QString("IntStep=%1\n").arg(m_step);
    strm << QString("ModelID=%1\n").arg(m_modelID);

    QString coefString("Coefs=");
    for (const qreal &value : m_coefs)
        coefString += QString::number(value)+"/";
    coefString.remove(coefString.length()-1, 1);
    coefString.append("\n");
    /*
     * ............................................................................................................................
...........................................-:+=%%%%%%%===%%%%%%=+:-.........................................................
......................................*=%%=======++++++++++++++======%=*-...................................................
..................................+%%====+++++++++++++++++++++++++++++====%*................................................
..............................-=%====++++++++**********************++++++++==%+.............................................
............................+%%==+++++=%%%@@@@@@@@%+**************=@@@@@@@%%%===%:..........................................
..........................=%===+++++*@@@@@@@@@@@@@@#*************%#@@@@@@@@@@@@+==%*........................................
........................=%==+++++****%#@@####@%=+*******************+=%@######%++++==:......................................
...............:::**..*%===++++*******++************************************+**++++++==.....................................
......-**:..-*:...-*=%%==+++*****************************************++++++++++*++++++==:...................................
.....*-.-:+=:--..-*=%%=++++*************+++++++++++++*************++++++++++++++++++++++=*..................................
....+:-..:+=-.:**=%%===++************+++++++++++++++++++********+++++++++++====++++++++++=*.................................
....+*:::+%*---*=+:-...-*=********++++=+++++++******++++++****+++++*****++++=====+++++++++=*................................
....+-..-:+=:*+=:...--....-++****++++=%@###########@=**++=+**++++****%##############@=+*+++=##:.............................
....=*:-:*=+.-+*-***=@*:--:=@%%@#########################%=**+++=#############################+.............................
....+*::**+=-:=::=%@####@@#############@@@############################@#######################+.............................
.....=:---:+%+@+%=+++**+:--*%@#######%@@@@@@@@@@@@@@@##############@@@@@@@@@@@@@@@@@######++=%@.............................
....:+:---:+@#@%@=++*+=:-.-++**#####@@@@@@@@@@@@@@@@@@@@#####@####@@@@@@@@@@@@@@@@@@@####=++==#:............................
...-=*:---*+%@%%:-:*=*--:-:=+**%####@@@@@@@@@@@@@@@@@@@@@####@####@@@@@@@@@@@@@@@%@@@####*+++=#=............................
...=*::--:*=@=*-.....---:*=*****#####@@@@@@@@@@@@@@@@@@%@###@=####@@@@@@@@@@@@@@%%%@####=*+++=+.............................
..*+*:---:*==:::......-*=*******=#####@@@@@@@@@@@@@@@%%@####**#####@@@@@@@@@@@@%%%@####=***++=+.............................
..=+*::-:*+%*:**:---:*=+*********=######@@@@@@@@@@@@%%@####****@#####@@@@@%%%%%%@#####+****+==+.............................
..=+**::*+%+:*+++===+************++@######@@@@@@@@@@#####%******+#########@@########+******+=%+.............................
:*:*++++=%%*+=@%===++************++++@#################%**********+##############@+********+=%*.............................
.:+:-..-*=%+.:@%%===++*****************+%###########%*****************+%@@@@@%++++=%%=++**++=%-.............................
......::**-...@%%%==++**++++%@@%==+++++++++++++++**************++++++++++++==%%@##%+++++**++%=...............--.............
..............=@%%%==++*+++==++=%%+*:+%%%%%%%%==++****++++++++++===%%====::*=*+@=+********+=%*............*+:---:...........
..............:@%%%%=++***********==:*=+::--**::---:::---::*:---:*:--::**:*+%+%+**+++****++=%.............%+:...-...........
...............%@%%%==++************+=@=**::=**::::**:::::++::::***::*+=+***=+**++++++++++=%:............-@=*:-:*...........
...............:@@%%%=+++**************+*---=+*****%=*****%=:-:-:*----:*--:+**+++++++++++=%+.............:@+:...:...........
................+@%%%==+++++*****++++****++-*:--.-.:-..---::--..-:--.-:*:++*++++++++++++=%=..............%%+:.-::...........
.................=@%%%==+++++++*++++++++****++:---.::---.-::--.-:*---:*+**+++==+++++++==%=...............@%+:-..:...........
..................%@%%%==+++++++++++++=++++****+++**::---:*::--::++++***+++==+++++++===%=................##%+:-.-:-.........
...................=@%%===+++++++++++++====+++++******++++++++*******+++=%%=++++++====%*................:@@%@#=*:::::***:...
....................*@@%%===+++++++++++++=%%==++++++************++++++=%=+++++++====%=..................=@=@#%+:-.......-*-.
......................%@%%=====+++++++++++++=%%==++++++++++++++++++=%=+++++++=====%%-.................+*@%=%#@=:-.......-::.
.......................-%@%%======++++++++++++++==%%%==========%%==++++========%%%:..................:::=%%=%%%=====+====*..
.........................-%@%%%=======++++++++++++++++=======================%%%:.................. .*-:+==%===+*:---::*++..
............................+@%%%%=======================================%%%%+.......................:*+%%%@%%=+:........-:.
..............................-=@@%%%%===============================%%%%%+...........................:*#%==@@%=*------.-::.
..................................:%@%%%%%%%%==================%%%%%%%*-.................................@@@%=+*+++++==::...
.......................................:+=@@%%%%%%%%%%%%%%%%%%@%+:...... .................................%@=:....-...-:....
...................................................----...................................................:#%:-........:....
............................................................................................................-***+*++**:.....
............................................................................................................................
*/


    strm << coefString;

    QString populsString("Populations=");
    for (const qreal &value : m_populations)
        populsString += QString::number(value)+"/";
    populsString.remove(populsString.length()-1, 1);
    populsString.append("\n");
    strm << populsString;
    file.close();
    return true;
}

bool DataLoader::load(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
        return false;

    while (!file.atEnd())
    {
        QString line = file.readLine();
        QString key = line.split("=").first();
        QString value = line.split("=").back();

        if (key == tr("IntStart"))
        {
            m_start = value.toDouble();
        }
        if (key == tr("IntEnd"))
        {
            m_end = value.toDouble();
        }
        if (key == tr("IntStep"))
        {
            m_step = value.toDouble();
        }
        if (key == tr("ModelID"))
        {
            m_modelID = value.toInt();
        }
        if (key == tr("Coefs"))
        {
            m_coefs.clear();
            QStringList values = value.split("/");
            for (const QString &v : values)
                m_coefs << v.toDouble();

        }
        if (key == tr("Populations"))
        {
            m_populations.clear();
            QStringList values = value.split("/");
            for (const QString &v : values)
                m_populations << v.toDouble();
        }

    }
    file.close();
    return true;
}

quint8 DataLoader::getModelID() const
{
    return m_modelID;
}

QList<qreal> DataLoader::getCoefValues() const
{
    return m_coefs;
}

QList<qreal> DataLoader::getPopulationValues() const
{
    return m_populations;
}

qreal DataLoader::getStart() const
{
    return m_start;
}

qreal DataLoader::getEnd() const
{
    return m_end;
}

qreal DataLoader::getStep() const
{
    return m_step;
}
