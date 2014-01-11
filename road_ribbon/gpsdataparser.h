#ifndef GPSDATAPARSER_H
#define GPSDATAPARSER_H

#include <QRegExp>
#include <QString>
#include <QFile>
#include <QTextStream>

#include <QVector>
#include <QDebug>

#include "vector2d.h"

class GPSDataParser
{
public:
    GPSDataParser();
    GPSDataParser(QString filename);

    void toJS(QString filename);
    void parse(QString filename);

    QVector<Vector2d> points;

protected:
    double str2deg(QString str);
    void processLine(QString line);
};

#endif // GPSDATAPARSER_H
