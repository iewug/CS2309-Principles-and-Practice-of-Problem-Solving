#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <datastruct.h>
#include <QVector>
#include <kdtree.h>
#include <QString>
#include <QDebug>
#include <mythread.h>
#include <QMessageBox>
#include <QMap>
#include <QTableView>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QtCharts>
#include <QChartView>
#include <QMetaType>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    /*
     * Once the openFileButton is clicked, the loading and all the data structures will be done
     */
    void on_openFileButton_clicked();

    /*
     * build data structures
     */
    void handleParsedResult(allInfo info);

    /*
     * update the value of my progress bar
     */
    void handleProgressBar(int n);

    /*
     * when the child thread is over, notify the main program
     */
    void handleDoneSignal();

    /*
     * filter
     */
    void on_okButton_clicked();

    /*
     * User's favorite POIs
     */
    void on_tab2_okbutton_clicked();

    /*
     * checking-in number over time
     */
    void on_tab3_okbutton_clicked();

    /*
     * DAU
     */
    void on_tab4_okbutton_clicked();

    /*
     * user's difference
     */
    void on_tab5_okbutton_clicked();

    /*
     * trajectory of one user over time
     */
    void on_trajectory_pushButton_clicked();

    /*
     * heatmap
     */
    void on_heat_pushButton_clicked();

    /*
     * prediction
     */
    void on_pre_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    /*
     * data structures for my program
     */
    //checkin存所有的数据
    QVector<Checkin> checkinData;
    //userIndex[i]是userID为i的用户对应checkin的起始位置
    QVector<int> userIndex;
    //POIIndex[i][j]是第i个POI的第j(0-base)次访问对应的checkin的位置
    QVector<QVector<int>> POIIndex;
    //location[i]存locationID为i的经纬度
    QVector<Location> locationData;
    //GPSIndex.range(a,b,c,d)返回[a,b]X[c,d]范围内的点对应checkin的位置
    KDtree GPSIndex;
    //to indicate all the file has been loaded
    bool isParsedOver;


    /*
     * private method to print checking-in data
     */
    QString printCheckin(const Checkin &c);

    QChart *chart;
    QChartView *chartView;

    QChart *DAUchart;
    QChartView *DAUchartView;
};
#endif // MAINWINDOW_H
