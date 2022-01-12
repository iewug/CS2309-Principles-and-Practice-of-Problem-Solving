#include "mythread.h"

myThread::myThread(const QString &_filename, QObject *parent) : QThread(parent)
{
    filename = _filename;
}

void myThread::run()
{
    int cnt = 0;
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        while (!stream.atEnd())
        {
            cnt++;

            /*
             * parser
             */
            QString line = stream.readLine();
            QStringList split = line.split(',');
            int user_ID = split.at(0).toInt();
            int POI_ID = split.at(1).toInt();
            double latitude = split.at(3).toDouble();
            double longitude = split.at(4).toDouble();
            QStringList timeSplit = split.at(2).split('T');
            QStringList dateSplit = timeSplit.at(0).split('-');
            int year = dateSplit.at(0).toInt();
            int month = dateSplit.at(1).toInt();
            int day = dateSplit.at(2).toInt();
            QStringList secondSplit = timeSplit.at(1).split(':');
            int hour = secondSplit.at(0).toInt();
            int minute = secondSplit.at(1).toInt();
            QString secondwithz = secondSplit.at(2);
            secondwithz.remove('Z');
            int second = secondwithz.toInt();
            emit lineProcessed(cnt);
            emit parsedResult(allInfo(user_ID,POI_ID,Time(year,month,day,hour,minute,second),latitude,longitude));
        }
    }
    file.close();
}
