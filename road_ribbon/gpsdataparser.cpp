#include "gpsdataparser.h"

GPSDataParser::GPSDataParser()
{
    points.clear();
}

GPSDataParser::GPSDataParser(QString filename)
{
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
        QString line = in.readLine();

        processLine(line);
    }

    f.close();

    qDebug() << QString("Parsing done. Found %1 points").arg(points.size());
}

void GPSDataParser::toJS(QString filename)
{
    if (points.size() < 1)
    {
        qDebug() << QString("Could not find any points. Terminating...");
        return;
    }

    QFile f(filename);

    f.open(QIODevice::WriteOnly | QIODevice::Text);

    QTextStream out(&f);

    out << "window.waypoints = [\n";

    for (long long i = 0; i < points.size(); i++)
    {
        QString line = QString("[ %1, %2 ]").arg(points[i].x).arg(points[i].y);

        out << line;

        if (i < points.size() - 1)
        {
            out << ",";
        }

        out << "\n";
    }

    out << "];";

    f.close();

    qDebug() << QString("Wrote %1 points to `%2` file").arg(points.size()).arg(filename);
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

void GPSDataParser::processLine(QString line)
{
    QRegExp re("\\$(GP|GL|GA|GN)RMC,(\\d\\d)(\\d\\d)(\\d+\\.\\d+),([A-Z]),(\\d+\\d\\d\\.\\d+),([A-Z]),(\\d+\\d\\d\\.\\d+),([A-Z]),(\\d+\\.\\d+),(\\d+\\.\\d+),(\\d\\d)(\\d\\d)(\\d\\d)");

    if (re.indexIn(line) != -1)
    {
        double lat = str2deg(re.cap(6)),
               lng = str2deg(re.cap(8));

        Vector2d point(lat, lng);
        points.push_back(point);
    } else
    {
        qDebug() << QString("Line `%1` is invalid").arg(line);
    }
}
