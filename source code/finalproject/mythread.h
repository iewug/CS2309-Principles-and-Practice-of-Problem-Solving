/*
 * multi-thread is used prevent my program from being stuck
 */
#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <datastruct.h>
#include <QFile>
#include <QString>
#include <QFileDialog>
#include <QTextStream>

class myThread : public QThread
{
    Q_OBJECT
public:
    explicit myThread(const QString &filename, QObject *parent = nullptr);
    void run() override;

signals:
    void lineProcessed(int n);
    void parsedResult(allInfo info);

private:
    QString filename;
};

#endif // MYTHREAD_H
