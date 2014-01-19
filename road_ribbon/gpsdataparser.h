#ifndef GPSDATAPARSER_H
#define GPSDATAPARSER_H

#include <QRegExp>
#include <QString>
#include <QFile>
#include <QTextStream>

#include <QVector>
#include <QDebug>

#include "vector2d.h"
#include "vector3d.h"
#include "skeletoncenter.h"

class GPSDataParser
{
public:
    GPSDataParser();
    GPSDataParser(QString filename);

    void toJS(QString filename);
    void parse(QString filename);

    QVector<SkeletonCenter> centers;

protected:
    double str2deg(QString str);
    void processLines(QString gprmcLine, QString pgrmzLine);
    Vector2d processGPRMCLine(QString line);
    double processPGRMZLine(QString line);

    void init();

    QRegExp gprmcRe;
    QRegExp pgrmzRe;

    const double ALTITUDE_MAGNIFIER = 0.1;
};

#endif // GPSDATAPARSER_H
