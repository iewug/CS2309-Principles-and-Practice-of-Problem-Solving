#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    isParsedOver = false;

    ui->doneLabel->hide();

    chart = new QChart();
    chartView = new QChartView();
    DAUchart = new QChart();
    DAUchartView = new QChartView();

    ui->label_58->hide();
    ui->label_61->hide();
    ui->label_68->hide();
    ui->label_69->hide();

}

MainWindow::~MainWindow()
{
    delete chart;
    delete chartView;
    delete DAUchart;
    delete DAUchartView;
    delete ui;
}

/*
 * Once the openFileButton is clicked, the loading and all the data structures will be maked
 */
void MainWindow::on_openFileButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,"choose a file", "C:/Users/","*.csv");
    if (filename.isEmpty())
        return;
    ui->progressBar->setRange(0, 1502536);

    myThread *parser = new myThread(filename);
    qRegisterMetaType<allInfo>("allInfo");
    connect(parser,&myThread::parsedResult,this,&MainWindow::handleParsedResult);
    connect(parser,&myThread::finished,this,&MainWindow::handleDoneSignal);
    connect(parser,&myThread::lineProcessed,this,&MainWindow::handleProgressBar);
    parser->start();
}

/*
 * update the value of my progress bar
 */
void MainWindow::handleProgressBar(int n)
{
    ui->progressBar->setValue(n);
}

/*
 * build data structures
 */
void MainWindow::handleParsedResult(allInfo info)
{
    int POI_ID = info.locationID;
    int user_ID = info.userID;
    double latitude = info.latitude;
    double longitude = info.longitude;
    Time time = info.time;

    /*
     * build locationData
     */
    if (locationData.size() == POI_ID)
    {
        Location loc(latitude,longitude);
        locationData.push_back(loc);
    }

    /*
     * build userID
     */
    if (userIndex.size() == user_ID)
        userIndex.push_back(checkinData.size());


    /*
     * build POIIndex
     */
    if (POIIndex.size() == POI_ID)
    {
        QVector<int> newPOIID;
        POIIndex.push_back(newPOIID);
        /*
         * build KDtree
         */
        KDItem item(latitude,longitude,POI_ID);
        GPSIndex.insert(item);
    }
    POIIndex[POI_ID].push_back(checkinData.size());

    /*
     * build checkinData
     */
    Checkin chking(user_ID, POI_ID, time);
    checkinData.push_back(chking);
}

/*
 * when the child thread is over, notify the main program
 */
void MainWindow::handleDoneSignal()
{
    isParsedOver = true;
    Checkin chking(-1,-1,-1);
    checkinData.push_back(chking);
    ui->progressBar->hide();
    ui->doneLabel->show();
}

/*
 * private method to print checking-in data
 */
QString MainWindow::printCheckin(const Checkin &c)
{
    QString ret = "User: ";
    ret.append(QString::number(c.userID));
    ret.append("; POI: ");
    ret.append(QString::number(c.locationID));
    ret.append("; Time: ");
    ret.append(QString::number(c.time.year));
    ret.append('/');
    ret.append(QString::number(c.time.month));
    ret.append('/');
    ret.append(QString::number(c.time.day));
    ret.append(", ");
    ret.append(QString::number(c.time.hour));
    ret.append(':');
    ret.append(QString::number(c.time.minute));
    ret.append(':');
    ret.append(QString::number(c.time.second));
    ret.append("; (lat,long): (");
    ret.append(QString::number(locationData[c.locationID].latitude));
    ret.append(',');
    ret.append(QString::number(locationData[c.locationID].longitude));
    ret.append(')');
    return ret;

}

/*
 * filter!
 */
void MainWindow::on_okButton_clicked()
{
    if (!isParsedOver)
    {
        QMessageBox::warning(this,"Warning","Please wait until all the file is loaded");
        return;
    }

    ui->textBrowser->clear();

    bool userCheck, POIcheck, GPScheck, timeCheck;
    userCheck = ui->userID_checkbox->isChecked();
    POIcheck = ui->POIID_checkbox->isChecked();
    GPScheck = ui->GPS_checkbox->isChecked();
    timeCheck = ui->time_checkbox->isChecked();

    /*
     * build starting time and ending time
     */
    int syear,smonth,sday,shour,sminute,ssecond;
    int eyear,emonth, eday, ehour,eminute,esecond;
    QDateTime beginDateTime = ui->beginTime->dateTime();
    QString beginT = beginDateTime.toString("yyyy/M/d hh:mm:ss");
    QStringList split = beginT.split(' ');
    QStringList daysplit = split.at(0).split('/');
    QStringList secondsplit = split.at(1).split(':');
    syear = daysplit.at(0).toInt();
    smonth = daysplit.at(1).toInt();
    sday = daysplit.at(2).toInt();
    shour = secondsplit.at(0).toInt();
    sminute = secondsplit.at(1).toInt();
    ssecond = secondsplit.at(2).toInt();
    Time stime(syear,smonth,sday,shour,sminute,ssecond);
    split.clear();
    daysplit.clear();
    secondsplit.clear();
    QDateTime endDateTime = ui->endTime->dateTime();
    QString endT = endDateTime.toString("yyyy/M/d hh:mm:ss");
    split = endT.split(' ');
    daysplit = split.at(0).split('/');
    secondsplit = split.at(1).split(':');
    eyear = daysplit.at(0).toInt();
    emonth = daysplit.at(1).toInt();
    eday = daysplit.at(2).toInt();
    ehour = secondsplit.at(0).toInt();
    eminute = secondsplit.at(1).toInt();
    esecond = secondsplit.at(2).toInt();
    Time etime(eyear,emonth,eday,ehour,eminute,esecond);

    if (etime < stime)
    {
        QMessageBox::warning(this,"Warning","Please check your input time");
        return;
    }

    int userID = ui->userID_inputbox->value();
    int POIID = ui->POIID_inputbox->value();
    double lowLatitude = ui->lowLa->value();
    double highLatitude = ui->highLa->value();
    double lowLongitude = ui->lowLo->value();
    double highLongitude = ui->highLo->value();

    if (lowLatitude > highLatitude || lowLongitude > highLongitude)
    {
        QMessageBox::warning(this,"Warning","Please check your input GPS");
        return;
    }

    bool istextBrowserEmpty = true;

    /*
     * key code for filter
     */
    if (userCheck)
    {
        for (int i = userIndex[userID];;i++)
        {
            if (checkinData[i].userID!=userID)
                break;
            if (POIcheck)
            {
                if (checkinData[i].locationID != POIID)
                    continue;
            }
            if (GPScheck)
            {
                double mylat = locationData[checkinData[i].locationID].latitude;
                double mylo = locationData[checkinData[i].locationID].longitude;
                if (mylat > highLatitude || mylat < lowLatitude || mylo > highLongitude || mylo < lowLongitude)
                    continue;
            }
            if (timeCheck)
            {
                if (stime > checkinData[i].time || etime < checkinData[i].time)
                    continue;
            }
            istextBrowserEmpty = false;
            ui->textBrowser->append(printCheckin(checkinData[i]));
        }
        if (istextBrowserEmpty)
            ui->textBrowser->setText("no data that meet the requirements");
    }
    else if(POIcheck)
    {
        for (int i = 0; i < POIIndex[POIID].size(); i++)
        {
            int chk = POIIndex[POIID][i];
            if (GPScheck)
            {
                double mylat = locationData[checkinData[chk].locationID].latitude;
                double mylo = locationData[checkinData[chk].locationID].longitude;
                if (mylat > highLatitude || mylat < lowLatitude || mylo > highLongitude || mylo < lowLongitude)
                    continue;
            }
            if (timeCheck)
            {
                if (stime > checkinData[chk].time || etime < checkinData[chk].time)
                    continue;
            }
            istextBrowserEmpty = false;
            ui->textBrowser->append(printCheckin(checkinData[chk]));
        }
        if (istextBrowserEmpty)
            ui->textBrowser->setText("no data that meet the requirements");
    }
    else if (timeCheck || GPScheck)
    {
        QVector<int> chk;
        for (int i = 0; i < checkinData.size()-1; i++)
        {
            if (timeCheck)
            {
                if (checkinData[i].time > etime || checkinData[i].time < stime)
                    continue;
            }
            if (GPScheck)
            {
                double mylat = locationData[checkinData[i].locationID].latitude;
                double mylo = locationData[checkinData[i].locationID].longitude;
                if (mylat > highLatitude || mylat < lowLatitude || mylo > highLongitude || mylo < lowLongitude)
                    continue;
            }
            chk.push_back(i);
        }
        QString output;
        if (chk.size() == 0)
            output = "no data that meet the requirements";
        else output = "The data that meet the requirements are located in line: ";
        for (int i = 0; i < chk.size();i++)
        {
            output.append(QString::number(chk[i]));
            output.append(' ');
        }
        ui->textBrowser->setText(output);

        /* Instead of showing the lines only, the following code can
         * display the checkin detail. But I find showing so much data annoying
        for (int i = 0; i < POIinRange.size();i++)
        {
            int POIID = POIinRange[i];
            for (int j = 0; j < POIIndex[POIID].size(); j++)
            {
                ui->textBrowser->append(printCheckin(checkinData[POIIndex[POIID][j]]));
            }
        }
        */
    }
    else
    {
        QMessageBox::warning(this,"Warning","Please select at least one checkbox");
    }
}

/*
 * User's favorite POIs
 */
void MainWindow::on_tab2_okbutton_clicked()
{
    if (!isParsedOver)
    {
        QMessageBox::warning(this,"Warning","Please wait until all the file is loaded");
        return;
    }

    /*
     * parser
     */
    QString stringUser = ui->tab2_userID->text();
    int sz= stringUser.size();
    for (int i = 0; i < sz; i++)
    {
        QChar foo = stringUser.at(i);
        if (foo <= '9' && foo >= '0')
            continue;
        if ((foo != ',' && foo != '-') || (i == 0 || i == sz-1))
        {
            QMessageBox::warning(this,"Warning","Please check your input");
            return;
        }
        QChar hou = stringUser.at(i+1);
        if (hou < '0' || hou > '9')
        {
            QMessageBox::warning(this,"Warning","Please check your input");
            return;
        }
    }
    QVector<int> UserIDs;
    QStringList split = stringUser.split(',');
    for (int i = 0; i < split.size(); i++)
    {
        QString bar = split.at(i);
        bool isInt = true;
        for (int j = 0; j < bar.size(); j++)
        {
            if (bar.at(j) == '-')
            {
                isInt = false;
                break;
            }
        }
        if (isInt)
        {
            UserIDs.push_back(bar.toInt());
        }
        else
        {
            QStringList barbar = bar.split('-');
            int small = barbar.at(0).toInt();
            int big = barbar.at(1).toInt();
            for (int j = small; j <= big; j++)
            {
                UserIDs.push_back(j);
            }
        }
    }

    int startYear = ui->tab2_syear->value();
    int endYear = ui->tab2_eyear->value();
    int startMonth = ui->tab2_smonth->value();
    int endMonth = ui->tab2_emonth->value();
    yearMonth staaart(startYear,startMonth);
    yearMonth eeeend(endYear,endMonth);
    if (eeeend < staaart)
    {
        QMessageBox::warning(this,"Warning","ending time is smaller than starting time");
        return;
    }
    int numberOfMonth = eeeend-staaart+1;
    int maxSize = userIndex.size();

    /*
     * an array of map is used to store different month's visited POIIDs and their times
     */
    QMap<int,int> *map = new QMap<int,int>[numberOfMonth];
    for (int i = 0; i < UserIDs.size(); i++)
    {
        int userID = UserIDs[i];
        if (userID >= maxSize)
        {
            QMessageBox::warning(this,"Warning","UserID too Big!");
            delete[] map;
            return;
        }
        for (int j = userIndex[userID];checkinData[j].userID==userID;j++)
        {
            yearMonth myTime(checkinData[j].time.year,checkinData[j].time.month);
            if (myTime < staaart || myTime > eeeend)
                continue;
            int targetPOI = checkinData[j].locationID;
            int delta = myTime-staaart;
            QMap<int,int>::iterator it = map[delta].find(targetPOI);
            if (it == map[delta].end())
            {
                map[delta].insert(targetPOI, 1);
            }
            else
            {
                int cnt = it.value();
                it.value() = ++cnt;
            }
        }
    }

    /*
     * draw the table
     */
    /* 创建数据模型 */
    QStandardItemModel* model = new QStandardItemModel();
    /* 自适应所有列，让它布满空间 */
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    /* 设置表格标题行(输入数据为QStringList类型) */
    model->setHorizontalHeaderLabels({"y/m", "TOP1", "TIMES", "TOP2", "TIMES", "TOP3", "TIMES", "TOP4", "TIMES", "TOP5", "TIMES",});
    for (int i = 0; i < numberOfMonth; i++)
    {
        QVector<POIIDandTime> bar;
        for (QMap<int,int>::iterator it = map[i].begin(); it != map[i].end(); it++)
        {
            POIIDandTime foo;
            foo.POIID = it.key();
            foo.times = it.value();
            bar.push_back(foo);
        }
        std::sort(bar.begin(),bar.end());
        QString output;
        output.append(QString(staaart+i));
        model->setItem(i, 0, new QStandardItem(output));

        for (int j = 0; j < 5 && j < bar.size(); j++)
        {
            model->setItem(i,2*j+1,new QStandardItem(QString::number(bar[j].POIID)));
            model->setItem(i,2*j+2,new QStandardItem(QString::number(bar[j].times)));
        }
        //ui->tab2_textBrowser->append(output);
    }
    ui->tableView->setModel(model);/* 设置表格视图数据 */
    delete[] map;
}

/*
 * checking-in number over time
 */
void MainWindow::on_tab3_okbutton_clicked()
{
    if (!isParsedOver)
    {
        QMessageBox::warning(this,"Warning","Please wait until all the file is loaded");
        return;
    }
    bool byPOIID = ui->tab3_POIcheckbox->isChecked();
    bool byRange = ui->tab3_rangecheckbox->isChecked();
    bool byHour = ui->tab3_hourcheckbox->isChecked();
    bool byMonth = ui->tab3_monthcheckbox->isChecked();
    if (byPOIID && byRange)
    {
        QMessageBox::warning(this,"Warning","Please select only one between POIID and GPS Range");
        return;
    }
    if (!byPOIID && !byRange)
    {
        QMessageBox::warning(this,"Warning","Please select at least one between POIID and GPS Range");
        return;
    }
    if (byHour && byMonth)
    {
        QMessageBox::warning(this,"Warning","Please select only one between by hour and by month");
        return;
    }
    if (!byHour && !byMonth)
    {
        QMessageBox::warning(this,"Warning","Please select at least one between by hour and by month");
        return;
    }

    QVector<int> POIIDs;
    if (byPOIID)
    {
        QString stringPOI = ui->tab3_POIinput->text();

        QStringList split = stringPOI.split(',');
        for (int i = 0; i < split.size(); i++)
        {
            QString bar = split.at(i);
            bool isInt = true;
            for (int j = 0; j < bar.size(); j++)
            {
                if (bar.at(j) == '-')
                {
                    isInt = false;
                    break;
                }
            }
            if (isInt)
            {
                POIIDs.push_back(bar.toInt());
            }
            else
            {
                QStringList barbar = bar.split('-');
                int small = barbar.at(0).toInt();
                int big = barbar.at(1).toInt();
                for (int j = small; j <= big; j++)
                {
                    POIIDs.push_back(j);
                }
            }
        }
        int sz= stringPOI.size();
        for (int i = 0; i < sz; i++)
        {
            QChar foo = stringPOI.at(i);
            if (foo <= '9' && foo >= '0')
                continue;
            if ((foo != ',' && foo != '-') || (i == 0 || i == sz-1))
            {
                QMessageBox::warning(this,"Warning","Please check your input");
                return;
            }
            QChar hou = stringPOI.at(i+1);
            if (hou < '0' || hou > '9')
            {
                QMessageBox::warning(this,"Warning","Please check your input");
                return;
            }
        }
    }
    if (byRange)
    {
        int sla = ui->tab3_sla->value();
        int ela = ui->tab3_ela->value();
        int slo = ui->tab3_slo->value();
        int elo = ui->tab3_elo->value();
        if (sla >= ela || slo >= elo)
        {
            QMessageBox::warning(this,"Warning","Please check your input GPS");
            return;
        }
        POIIDs = GPSIndex.rangeSearch(sla,ela,slo,elo);
    }

    QVector<int> times;
    if (byMonth)
    {
        int startYear = ui->tab3_syear->value();
        int endYear = ui->tab3_eyear->value();
        int startMonth = ui->tab3_smonth->value();
        int endMonth = ui->tab3_emonth->value();
        yearMonth staaart(startYear,startMonth);
        yearMonth eeeend(endYear,endMonth);
        if (eeeend < staaart)
        {
            QMessageBox::warning(this,"Warning","ending time is smaller than starting time");
            return;
        }

        //key code
        times.resize(eeeend-staaart+1);
        for (int i = 0; i < POIIDs.size(); i++)
        {
            int targetPOI = POIIDs[i];
            int sz = POIIndex[targetPOI].size();
            for (int j = 0; j < sz; j++)
            {
                int chk = POIIndex[targetPOI][j];
                int mymonth = checkinData[chk].time.month;
                int myyear = checkinData[chk].time.year;
                yearMonth mytime(myyear,mymonth);
                if (mytime < staaart || mytime > eeeend)
                    continue;
                times[mytime-staaart] = times[mytime-staaart]+1;
            }
        }

        /*
         * draw the line chart
         */
        chart->removeAllSeries();
        if (chart->axisX())
        {
            chart->removeAxis(chart->axisX());
            chart->removeAxis(chart->axisY());
        }
        QLineSeries* series = new QLineSeries();
        QCategoryAxis *xAxis=new QCategoryAxis();
        QValueAxis *yAxis = new QValueAxis();
        for (int i = 0; i < times.size(); i++)
        {
            series->append(i, times[i]);
            QString output = QString(staaart+i);
            xAxis->append(output,i);
        }
        xAxis->setTitleText("yy/m");
        xAxis->setLabelsPosition(QCategoryAxis::AxisLabelsPositionOnValue);
        yAxis->setTitleText("times");
        chart->addSeries(series);
        chart->addAxis(xAxis,Qt::AlignBottom);
        series->attachAxis(xAxis);
        chart->addAxis(yAxis,Qt::AlignLeft);
        series->attachAxis(yAxis);
        chart->legend()->hide();
        chartView->setChart(chart);
        ui->linechart->addWidget(chartView);
    }
    if (byHour)
    {
        QDate date = ui->tab3_dayInput->date();
        times.resize(30);
        for (int i = 0; i < POIIDs.size(); i++)
        {
            int targetPOI = POIIDs[i];
            int sz = POIIndex[targetPOI].size();
            for (int j = 0; j < sz; j++)
            {
                int chk = POIIndex[targetPOI][j];
                int mymonth = checkinData[chk].time.month;
                int myyear = checkinData[chk].time.year;
                int myday = checkinData[chk].time.day;
                int myhour = checkinData[chk].time.hour;
                QDate mydate(myyear,mymonth,myday);
                if (mydate != date)
                    continue;
                times[myhour] = times[myhour]+1;
            }
        }
        /*
         * draw the line chart
         */
        chart->removeAllSeries();
        if (chart->axisX())
        {
            chart->removeAxis(chart->axisX());
            chart->removeAxis(chart->axisY());
        }
        QLineSeries* series = new QLineSeries();
        QValueAxis *xAxis=new QValueAxis();
        QValueAxis *yAxis = new QValueAxis();
        for (int i = 0; i < 24; i++)
        {
            series->append(i, times[i]);
        }
        xAxis->setRange(0,24);
        xAxis->setTickCount(13);
        xAxis->setTitleText("hour");
        yAxis->setTitleText("times");
        chart->addSeries(series);
        chart->addAxis(xAxis,Qt::AlignBottom);
        series->attachAxis(xAxis);
        chart->addAxis(yAxis,Qt::AlignLeft);
        series->attachAxis(yAxis);
        chart->legend()->hide();
        chartView->setChart(chart);
        ui->linechart->addWidget(chartView);
    }
}

/*
 * DAU
 */
void MainWindow::on_tab4_okbutton_clicked()
{
    if (!isParsedOver)
    {
        QMessageBox::warning(this,"Warning","Please wait until all the file is loaded");
        return;
    }
    int POI1 = ui->tab4_POI1->value();
    int POI2 = ui->tab4_POI2->value();
    int year = ui->tab4_year->value();
    int month = ui->tab4_month->value();
    QVector<int> times1;
    QVector<int> times2;
    times1.resize(33);
    times2.resize(33);
    for (int i = 0; i < POIIndex[POI1].size(); i++)
    {
        int chk = POIIndex[POI1][i];
        Time chktime = checkinData[chk].time;
        if (chktime.year == year && chktime.month == month)
        {
            times1[chktime.day] = times1[chktime.day]+1;
        }
    }
    for (int i = 0; i < POIIndex[POI2].size(); i++)
    {
        int chk = POIIndex[POI2][i];
        Time chktime = checkinData[chk].time;
        if (chktime.year == year && chktime.month == month)
        {
            times2[chktime.day] = times2[chktime.day]+1;
        }
    }

    /*
     * draw the line chart
     */
    DAUchart->removeAllSeries();
    if (DAUchart->axisX())
    {
        DAUchart->removeAxis(DAUchart->axisX());
        DAUchart->removeAxis(DAUchart->axisY());
    }
    QLineSeries* series1 = new QLineSeries();
    QLineSeries* series2 = new QLineSeries();
    series1->setName("POI1");
    series2->setName("POI2");
    QValueAxis *xAxis1 = new QValueAxis();
    QValueAxis *yAxis1 = new QValueAxis();
    xAxis1->setRange(1,31);
    xAxis1->setTickCount(16);
    int max1 = -1;
    for (int i = 1; i <= 31; i++)
    {
        max1 = max1 > times1[i] ? max1 : times1[i];
    }
    int max2 = -1;
    for (int i = 1; i <= 31; i++)
    {
        max2 = max2 > times2[i] ? max2 : times2[i];
    }
    int max = max1 > max2 ? max1 : max2;
    yAxis1->setRange(0,max);
    for (int i = 1; i <= 31; i++)
    {
        series1->append(i, times1[i]);
        series2->append(i,times2[i]);
    }
    DAUchart->addSeries(series1);
    DAUchart->addSeries(series2);
    xAxis1->setTitleText("date");
    yAxis1->setTitleText("Active Users");

    if (max1 > max2)
    {
        DAUchart->setAxisX(xAxis1,series1);
        DAUchart->setAxisY(yAxis1,series1);
        DAUchart->setAxisX(xAxis1, series2);
        DAUchart->setAxisY(yAxis1, series2);
    }
    else
    {
        DAUchart->setAxisX(xAxis1, series2);
        DAUchart->setAxisY(yAxis1, series2);
        DAUchart->setAxisX(xAxis1,series1);
        DAUchart->setAxisY(yAxis1,series1);
    }
    DAUchartView->setChart(DAUchart);
    ui->dau_linechart->addWidget(DAUchartView);
}

/*
 * user's difference
 */
void MainWindow::on_tab5_okbutton_clicked()
{
    if (!isParsedOver)
    {
        QMessageBox::warning(this,"Warning","Please wait until all the file is loaded");
        return;
    }

    ui->quickWidget->setSource(QUrl(QStringLiteral("qrc:/map.qml")));
    QObject *pRoot = (QObject*)ui->quickWidget->rootObject();
    QObject *object = pRoot->findChild<QObject*>("myMap");
    int year = ui->tab5_year->value();
    int month = ui->tab5_month->value();
    int user1 = ui->tab5_user1->value();
    int user2 = ui->tab5_user2->value();
    int minLng, maxLng, minLat, maxLat;
    minLng= minLat = 1000;
    maxLng = maxLat = -1000;
    bool hasData = false;
    for (int i = userIndex[user1]; checkinData[i].userID == user1;i++)
    {
        if (checkinData[i].time.year != year || checkinData[i].time.month != month)
            continue;
        hasData = true;
        int locID = checkinData[i].locationID;
        double y = locationData[locID].latitude;
        double x = locationData[locID].longitude;
        minLng = minLng < x ? minLng : x;
        maxLng = maxLng > x ? maxLng : x;
        minLat = minLat < y ? minLat : y;
        maxLat = maxLat > y ? maxLat : y;
        QMetaObject::invokeMethod(object, "addMarkerRed", Q_ARG(double, y),Q_ARG(double, x));
    }
    for (int i =userIndex[user2]; checkinData[i].userID == user2; i++)
    {
        if (checkinData[i].time.year != year || checkinData[i].time.month != month)
            continue;
        hasData = true;
        int locID = checkinData[i].locationID;
        double y = locationData[locID].latitude;
        double x = locationData[locID].longitude;
        minLng = minLng < x ? minLng : x;
        maxLng = maxLng > x ? maxLng : x;
        minLat = minLat < y ? minLat : y;
        maxLat = maxLat > y ? maxLat : y;
        QMetaObject::invokeMethod(object, "addMarkerBlue", Q_ARG(double, y),Q_ARG(double, x));

    }
    if (!hasData)
    {
        QMessageBox::warning(this,"Warning","There is no data available");
        return;
    }
    double centerLat = (maxLat+minLat)/2;
    double centerLng = (maxLng+minLng)/2;
    QMetaObject::invokeMethod(object, "setCenter", Q_ARG(double, centerLat),Q_ARG(double, centerLng));
}

/*
 * trajectory of one user over time
 */
void MainWindow::on_trajectory_pushButton_clicked()
{
    if (!isParsedOver)
    {
        QMessageBox::warning(this,"Warning","Please wait until all the file is loaded");
        return;
    }
    int syear,smonth,sday,shour,sminute,ssecond;
    int eyear,emonth, eday, ehour,eminute,esecond;
    QDateTime beginDateTime = ui->beginTime_tra->dateTime();
    QString beginT = beginDateTime.toString("yyyy/M/d hh:mm:ss");
    QStringList split = beginT.split(' ');
    QStringList daysplit = split.at(0).split('/');
    QStringList secondsplit = split.at(1).split(':');
    syear = daysplit.at(0).toInt();
    smonth = daysplit.at(1).toInt();
    sday = daysplit.at(2).toInt();
    shour = secondsplit.at(0).toInt();
    sminute = secondsplit.at(1).toInt();
    ssecond = secondsplit.at(2).toInt();
    Time stime(syear,smonth,sday,shour,sminute,ssecond);
    split.clear();
    daysplit.clear();
    secondsplit.clear();
    QDateTime endDateTime = ui->endTime_tra->dateTime();
    QString endT = endDateTime.toString("yyyy/M/d hh:mm:ss");
    split = endT.split(' ');
    daysplit = split.at(0).split('/');
    secondsplit = split.at(1).split(':');
    eyear = daysplit.at(0).toInt();
    emonth = daysplit.at(1).toInt();
    eday = daysplit.at(2).toInt();
    ehour = secondsplit.at(0).toInt();
    eminute = secondsplit.at(1).toInt();
    esecond = secondsplit.at(2).toInt();
    Time etime(eyear,emonth,eday,ehour,eminute,esecond);
    if (etime < stime)
    {
        QMessageBox::warning(this,"Warning","Please check your input time");
        return;
    }

    ui->quickWidget_2->setSource(QUrl(QStringLiteral("qrc:/map.qml")));
    QObject *pRoot = (QObject*)ui->quickWidget_2->rootObject();
    QObject *object = pRoot->findChild<QObject*>("myMap");
    int ID = ui->tra_input->value();
    int minLng, maxLng, minLat, maxLat;
    minLng= minLat = 1000;
    maxLng = maxLat = -1000;
    bool hasData = false;
    for (int i = userIndex[ID]; checkinData[i].userID == ID; i++)
    {
        Time myTime = checkinData[i].time;
        if (myTime < stime || myTime > etime)
            continue;
        hasData = true;
        int locID = checkinData[i].locationID;
        double y = locationData[locID].latitude;
        double x = locationData[locID].longitude;
        minLng = minLng < x ? minLng : x;
        maxLng = maxLng > x ? maxLng : x;
        minLat = minLat < y ? minLat : y;
        maxLat = maxLat > y ? maxLat : y;
        QMetaObject::invokeMethod(object, "addMarkerRed", Q_ARG(double, y),Q_ARG(double, x));
    }
    if (!hasData)
    {
        QMessageBox::warning(this,"Warning","no data that meet the requirement");
        return;
    }
    double centerLat = (maxLat+minLat)/2;
    double centerLng = (maxLng+minLng)/2;
    QMetaObject::invokeMethod(object, "setCenter", Q_ARG(double, centerLat),Q_ARG(double, centerLng));
    for (int i = userIndex[ID]; checkinData[i+1].userID == ID; i++)
    {
        Time myTime1 = checkinData[i].time;
        if (myTime1 < stime) continue;
        if (myTime1 > etime) break;
        Time myTime2 = checkinData[i+1].time;
        if (myTime2 > etime) break;
        int locID1 = checkinData[i].locationID;
        double x1 = locationData[locID1].latitude;
        double y1 = locationData[locID1].longitude;
        int locID2 = checkinData[i+1].locationID;
        double x2 = locationData[locID2].latitude;
        double y2 = locationData[locID2].longitude;
        QMetaObject::invokeMethod(object, "addLine", Q_ARG(double, x1),Q_ARG(double, y1),Q_ARG(double, x2),Q_ARG(double, y2));
    }

}

/*
 * heatmap
 */
void MainWindow::on_heat_pushButton_clicked()
{
    if (!isParsedOver)
    {
        QMessageBox::warning(this,"Warning","Please wait until all the file is loaded");
        return;
    }
    bool byPOIID = ui->heat_POIcheckbox->isChecked();
    bool byRange = ui->heat_rangecheckbox->isChecked();
    if (byPOIID && byRange)
    {
        QMessageBox::warning(this,"Warning","Please select only one checkbox");
        return;
    }
    if (!byPOIID && !byRange)
    {
        QMessageBox::warning(this,"Warning","Please select at least one checkbox");
        return;
    }

    int startYear = ui->heat_staYear->value();
    int endYear = ui->heat_EndYear->value();
    int startMonth = ui->heat_staMon->value();
    int endMonth = ui->heat_EndMon->value();
    yearMonth staaart(startYear,startMonth);
    yearMonth eeeend(endYear,endMonth);
    if (eeeend < staaart)
    {
        QMessageBox::warning(this,"Warning","ending time is smaller than starting time");
        return;
    }
    QVector<int> POIIDs;

    if (byPOIID)
    {
        QString stringPOI = ui->heat_poiInput->text();

        QStringList split = stringPOI.split(',');
        for (int i = 0; i < split.size(); i++)
        {
            QString bar = split.at(i);
            bool isInt = true;
            for (int j = 0; j < bar.size(); j++)
            {
                if (bar.at(j) == '-')
                {
                    isInt = false;
                    break;
                }
            }
            if (isInt)
            {
                POIIDs.push_back(bar.toInt());
            }
            else
            {
                QStringList barbar = bar.split('-');
                int small = barbar.at(0).toInt();
                int big = barbar.at(1).toInt();
                for (int j = small; j <= big; j++)
                {
                    POIIDs.push_back(j);
                }
            }
        }
        int sz= stringPOI.size();
        for (int i = 0; i < sz; i++)
        {
            QChar foo = stringPOI.at(i);
            if (foo <= '9' && foo >= '0')
                continue;
            if ((foo != ',' && foo != '-') || (i == 0 || i == sz-1))
            {
                QMessageBox::warning(this,"Warning","Please check your input");
                return;
            }
            QChar hou = stringPOI.at(i+1);
            if (hou < '0' || hou > '9')
            {
                QMessageBox::warning(this,"Warning","Please check your input");
                return;
            }
        }
    }
    if (byRange)
    {
        int sla = ui->heat_sla->value();
        int ela = ui->heat_ela->value();
        int slo = ui->heat_slo->value();
        int elo = ui->heat_elo->value();
        if (sla >= ela || slo >= elo)
        {
            QMessageBox::warning(this,"Warning","Please check your input GPS");
            return;
        }
        POIIDs = GPSIndex.rangeSearch(sla,ela,slo,elo);
    }

    ui->quickWidget_3->setSource(QUrl(QStringLiteral("qrc:/map.qml")));
    QObject *pRoot = (QObject*)ui->quickWidget_3->rootObject();
    QObject *object = pRoot->findChild<QObject*>("myMap");
    QMap<int,int> times;
    int maxcnt = 0;
    for (int i = 0; i < POIIDs.size(); i++)
    {
        int targetPOI = POIIDs[i];
        int sz = POIIndex[targetPOI].size();
        int cnt = 0;
        for (int j = 0; j < sz; j++)
        {
            int chk = POIIndex[targetPOI][j];
            int mymonth = checkinData[chk].time.month;
            int myyear = checkinData[chk].time.year;
            yearMonth mytime(myyear,mymonth);
            if (mytime < staaart || mytime > eeeend)
                continue;
            cnt++;
        }
        maxcnt = maxcnt > cnt ? maxcnt : cnt;
        times.insert(targetPOI,cnt);
    }
    int minLng, maxLng, minLat, maxLat;
    minLng= minLat = 1000;
    maxLng = maxLat = -1000;
    for(auto i = times.begin(); i != times.end(); i++)
    {
        int targetPOI = i.key();
        double y = locationData[targetPOI].latitude;
        double x = locationData[targetPOI].longitude;
        minLng = minLng < x ? minLng : x;
        maxLng = maxLng > x ? maxLng : x;
        minLat = minLat < y ? minLat : y;
        maxLat = maxLat > y ? maxLat : y;
        double opacity = (double)i.value()/(maxcnt*2);
        QMetaObject::invokeMethod(object, "changeHeat",
                                  Q_ARG(double, y+0.02),Q_ARG(double, x-0.02),Q_ARG(double, y-0.02),Q_ARG(double, x+0.02),Q_ARG(double, opacity));
        if (byPOIID)
        {
            QMetaObject::invokeMethod(object, "addMarkerRed",Q_ARG(double, y),Q_ARG(double, x));
        }
    }

    double centerLat = (maxLat+minLat)/2;
    double centerLng = (maxLng+minLng)/2;
    QMetaObject::invokeMethod(object, "setCenter",Q_ARG(double, centerLat),Q_ARG(double, centerLng));
    if (byRange)
    {
        int sla = ui->heat_sla->value();
        int ela = ui->heat_ela->value();
        int slo = ui->heat_slo->value();
        int elo = ui->heat_elo->value();
        QMetaObject::invokeMethod(object, "addLine",
                                  Q_ARG(double, sla),Q_ARG(double, slo),Q_ARG(double, sla),Q_ARG(double, elo));
        QMetaObject::invokeMethod(object, "addLine",
                                  Q_ARG(double, sla),Q_ARG(double, slo),Q_ARG(double, ela),Q_ARG(double, slo));
        QMetaObject::invokeMethod(object, "addLine",
                                  Q_ARG(double, ela),Q_ARG(double, slo),Q_ARG(double, ela),Q_ARG(double, elo));
        QMetaObject::invokeMethod(object, "addLine",
                                  Q_ARG(double, sla),Q_ARG(double, elo),Q_ARG(double, ela),Q_ARG(double, elo));
    }
    ui->label_68->setText(QString::number(maxcnt));
    ui->label_69->setText(QString::number(2*maxcnt));
    ui->label_58->show();
    ui->label_61->show();
    ui->label_68->show();
    ui->label_69->show();
}

/*
 * prediction
 */
void MainWindow::on_pre_pushButton_clicked()
{
    if (!isParsedOver)
    {
        QMessageBox::warning(this,"Warning","Please wait until all the file is loaded");
        return;
    }
    bool byPOI = ui->pre_poi_check->isChecked();
    bool byUser = ui->pre_user_check->isChecked();
    if (byPOI && byUser)
    {
        QMessageBox::warning(this,"Warning","Please select only one checkbox");
        return;
    }
    if (!byPOI && !byUser)
    {
        QMessageBox::warning(this,"Warning","Please select at least one checkbox");
        return;
    }
    if (byPOI)
    {
        yearMonth small(2022,1);
        yearMonth big;
        int targetPOI = ui->pre_POIinput->value();
        int sz = POIIndex[targetPOI].size();
        for (int i = 0; i < sz; i++)
        {
            Checkin check = checkinData[POIIndex[targetPOI][i]];
            yearMonth mytime(check.time.year, check.time.month);
            small = small < mytime ? small : mytime;
            big = big > mytime ? big : mytime;
        }
        int avg = sz;
        if (big - small != 0)
        {
            avg /= (big - small);
        }
        QString output = "The predicted visiting number in ";
        output.append(QString(big+1));
        output.append(" is: ");
        output.append(QString::number(avg));
        ui->pre_textbrowser->setText(output);
    }
    if (byUser)
    {
        int userID = ui->pre_userInput->value();
        Time lastTime;
        for (int i = userIndex[userID]; checkinData[i].userID == userID; i++)
        {
            Time mytime = checkinData[i].time;
            lastTime = lastTime > mytime ? lastTime : mytime;
        }
        qDebug() << (QString)lastTime;
        QMap<int,int> map;
        for (int j = userIndex[userID];checkinData[j].userID==userID;j++)
        {
            int targetPOI = checkinData[j].locationID;
            Time tmp = checkinData[j].time;
            int delta = lastTime - tmp;
            qDebug()<<delta;
            int cnt = 1;
            if (delta > 30)
                continue;
            else if (delta < 3)
            {
                cnt = 5;
            }
            else if (delta < 7)
            {
                cnt = 2;
            }
            QMap<int,int>::iterator it = map.find(targetPOI);
            if (it == map.end())
            {
                map.insert(targetPOI, 1);
            }
            else
            {
                it.value() += cnt;
            }
        }
        QVector<POIIDandTime> bar;
        for (QMap<int,int>::iterator it = map.begin(); it != map.end(); it++)
        {
            POIIDandTime foo;
            foo.POIID = it.key();
            foo.times = it.value();
            bar.push_back(foo);
        }
        std::sort(bar.begin(),bar.end());
        QString output = "POIs he/she may visit next with high probability are: ";
        for (int i = 0; i < bar.size() && i < 5; i++)
        {
            output.append(QString::number(bar[i].POIID));
            output.append(" ");
        }
        ui->pre_textbrowser->setText(output);
    }
}
