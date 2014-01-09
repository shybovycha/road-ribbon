#include <QCoreApplication>
#include <QString>

#include "gpsdataparser.h"

int main(int argc, char *argv[])
{
    if (argc < 3) {
        printf("Not enough arguments\n");
        printf("Usage: %s [GPS data file] [converted JS file]\n", argv[0]);

        return 1;
    }

    GPSDataParser parser(argv[1]);

    parser.toJS(argv[2]);

    return 0;
}
