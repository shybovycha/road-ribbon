#include "gpsdataparser.h"

void GPSDataParser::init()
{
    gprmcRe = QRegExp("\\$GPRMC,((\\d\\d)(\\d\\d)(\\d+\\.\\d+)),([A-Z]),((\\d+)(\\d\\d\\.\\d+)),(S|N),((\\d+)(\\d\\d\\.\\d+)),(W|E)");

    pgrmzRe = QRegExp("\\$PGRMZ,(\\d+),(m|f),(\\d+)");

    centers.clear();
}

GPSDataParser::GPSDataParser()
{
    init();
}

GPSDataParser::GPSDataParser(QString filename)
{
    init();
    parse(filename);
}

void GPSDataParser::parse(QString filename)
{
    QFile f(filename);

    if (!f.exists())
    {
        qDebug() << QString("%1 does not exist\n").arg(filename);
        return;
    }

    f.open(QIODevice::ReadOnly | QIODevice::Text);

    QTextStream in(&f);

    while (!in.atEnd())
    {
        QString line1 = in.readLine(),
                line2 = in.readLine();

        processLines(line1, line2);
    }

    f.close();

    qDebug() << QString("Parsing done. Found %1 points").arg(centers.size());
}

void GPSDataParser::toJS(QString filename)
{
    if (centers.size() < 1)
    {
        qDebug() << QString("Could not find any points. Terminating...");
        return;
    }

    QFile f(filename);

    f.open(QIODevice::WriteOnly | QIODevice::Text);

    QTextStream out(&f);

    out << "window.waypoints = [\n";

    for (long long i = 0; i < centers.size(); i++)
    {
        QString line = QString("[ %1, %2 ]").arg(centers[i].location.x).arg(centers[i].location.y);

        out << line;

        if (i < centers.size() - 1)
        {
            out << ",";
        }

        out << "\n";
    }

    out << "];";

    f.close();

    qDebug() << QString("Wrote %1 points to `%2` file").arg(centers.size()).arg(filename);
}

double GPSDataParser::str2deg(QString str)
{
    QRegExp lat_re("(\\d\\d)(\\d\\d\\.\\d+)"),
            lng_re("(\\d\\d\\d)(\\d\\d\\.\\d+)");

    double degrees = 0.0, minutes_part = 0.0;

    if (lat_re.indexIn(str) != -1)
    {
        degrees = lat_re.cap(1).toDouble();
        minutes_part = lat_re.cap(2).toDouble();

        return degrees + (minutes_part / 60.0);
    } else if (lng_re.indexIn(str) != -1)
    {
        degrees = lng_re.cap(1).toDouble();
        minutes_part = lng_re.cap(2).toDouble();

        return degrees + (minutes_part / 60.0);
    } else
    {
        return 0.0;
    }
}

void GPSDataParser::processLines(QString gprmcLine, QString pgrmzLine)
{
    if (gprmcRe.indexIn(gprmcLine) < 0)
    {
        qDebug() << QString("`%1` is not a valid GPRMC line").arg(gprmcLine);
        return;
    }

    if (pgrmzRe.indexIn(pgrmzLine) < 0)
    {
        qDebug() << QString("`%1` is not a valid PGRMZ line").arg(pgrmzLine);
        return;
    }

    Vector2d location = processGPRMCLine(gprmcLine);
    double alt = processPGRMZLine(pgrmzLine) * ALTITUDE_MAGNIFIER;
    Vector3d position = Vector3d(location.x, alt, location.y);

    // TODO: refactor
    SkeletonCenter center(location, position, Vector3d(0, 0, 0));

    centers.push_back(center);
}

Vector2d GPSDataParser::processGPRMCLine(QString line)
{
    gprmcRe.indexIn(line);

    double lat = str2deg(gprmcRe.cap(6)),
           lng = str2deg(gprmcRe.cap(10));

    return Vector2d(lat, lng);
}

double GPSDataParser::processPGRMZLine(QString line)
{
    pgrmzRe.indexIn(line);

    return pgrmzRe.cap(1).toDouble();
}
