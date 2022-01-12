/*
 * all the data structures for my program
 */


#ifndef DATASTRUCT_H
#define DATASTRUCT_H
#include <QString>
#include <QDateTime>
struct Time
{
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
    Time(int _y = 0, int _mo = 0, int _d = 0, int _h = 0, int _mi = 0, int _s = 0)
        :year(_y),month(_mo),day(_d),hour(_h),minute(_mi),second(_s){}
    Time(const Time &o)
        :year(o.year),month(o.month),day(o.day),hour(o.hour),minute(o.minute),second(o.second){}
    bool operator<(Time const &oth)
    {
        if(year < oth.year)
            return true;
        else if (year > oth.year)
            return false;
        if(month < oth.month)
            return true;
        else if(month > oth.month)
            return false;
        if(day < oth.day)
            return true;
        else if(day > oth.day)
            return false;
        if(hour < oth.hour)
            return true;
        else if(hour > oth.hour)
            return false;
        if(minute < oth.minute)
            return true;
        else if(minute > oth.minute)
            return false;
        if(second < oth.second)
            return true;
        else
            return false;
    }
    bool operator<=(Time const &oth)
    {
        if(year < oth.year)
            return true;
        else if (year > oth.year)
            return false;
        if(month < oth.month)
            return true;
        else if(month > oth.month)
            return false;
        if(day < oth.day)
            return true;
        else if(day > oth.day)
            return false;
        if(hour < oth.hour)
            return true;
        else if(hour > oth.hour)
            return false;
        if(minute < oth.minute)
            return true;
        else if(minute > oth.minute)
            return false;
        if(second <= oth.second)
            return true;
        else
            return false;
    }

    bool operator>(Time const &oth)
    {
        if(year > oth.year)
            return true;
        else if (year < oth.year)
            return false;
        if(month > oth.month)
            return true;
        else if(month < oth.month)
            return false;
        if(day > oth.day)
            return true;
        else if(day < oth.day)
            return false;
        if(hour > oth.hour)
            return true;
        else if(hour < oth.hour)
            return false;
        if(minute > oth.minute)
            return true;
        else if(minute < oth.minute)
            return false;
        if(second > oth.second)
            return true;
        else
            return false;

    }
    bool operator>=(Time const &oth)
    {
        if(year > oth.year)
            return true;
        else if (year < oth.year)
            return false;
        if(month > oth.month)
            return true;
        else if(month < oth.month)
            return false;
        if(day > oth.day)
            return true;
        else if(day < oth.day)
            return false;
        if(hour > oth.hour)
            return true;
        else if(hour < oth.hour)
            return false;
        if(minute > oth.minute)
            return true;
        else if(minute < oth.minute)
            return false;
        if(second >= oth.second)
            return true;
        else
            return false;

    }

    bool operator==(Time const &oth)
    {
        return year == oth.year && month == oth.month && day == oth.day && hour == oth.hour && minute == oth.minute && second == oth.second;
    }
    operator QString() const
    {
        QString ret;
        ret.append(QString::number(year));
        ret.append('/');
        ret.append(QString::number(month));
        ret.append('/');
        ret.append(QString::number(day));
        ret.append(' ');
        ret.append(QString::number(hour));
        ret.append(':');
        ret.append(QString::number(minute));
        ret.append(':');
        ret.append(QString::number(second));
        return ret;
    }
    Time& operator=(const Time &o)
    {
        if (this == &o)
            return *this;
        year = o.year;
        month = o.month;
        minute = o.minute;
        second = o.second;
        day = o.day;
        hour = o.hour;
        return *this;
    }
    //两个日期差多少天 ，必须大的减小的
    friend int operator-(const Time &a, const Time &b)
    {
        QDate end(a.year, a.month, a.day);
        QDate sta(b.year, b.month, b.day);
        int cnt = 0;

        while(sta < end)
        {
            ++cnt;
            sta = sta.addDays(1);
        }
        return cnt;
    }
};

struct Location
{
    double latitude;
    double longitude;
    Location(double _la, double _lo)
        :latitude(_la),longitude(_lo){}
};

struct Checkin
{
    int userID;
    int locationID;
    Time time;
    Checkin(int _u, int _l, Time _t)
        :userID(_u),locationID(_l),time(_t){}
};

struct allInfo
{
    int userID;
    int locationID;
    Time time;
    double latitude;
    double longitude;
    allInfo(int _u=0,int _l=0, Time _t=Time(), double _la=0, double _lo=0)
        :userID(_u),locationID(_l),time(_t),latitude(_la),longitude(_lo){}
};

struct POIIDandTime
{
    int POIID;
    int times;
    bool operator < (const POIIDandTime &o)
    {
        return times > o.times;
    }
    bool operator <= (const POIIDandTime &o)
    {
        return times >= o.times;
    }
    bool operator > (const POIIDandTime &o)
    {
        return times < o.times;
    }
    bool operator >= (const POIIDandTime &o)
    {
        return times <= o.times;
    }
    bool operator == (const POIIDandTime &o)
    {
        return times == o.times;
    }
};

struct yearMonth
{
    int year;
    int month;
    yearMonth(int _y = 0, int _m = 0) : year(_y), month(_m){}
    bool operator < (const yearMonth &o)
    {
        if (year < o.year)
            return true;
        if (year > o.year)
            return false;
        if (month < o.month)
            return true;
        return false;
    }
    bool operator > (const yearMonth &o)
    {
        if (year > o.year)
            return true;
        if (year < o.year)
            return false;
        if (month > o.month)
            return true;
        return false;
    }
    bool operator == (const yearMonth &o)
    {
        return (year == o.year) && (month == o.month);
    }
    bool operator <= (const yearMonth &o)
    {
        if (year < o.year)
            return true;
        if (year > o.year)
            return false;
        if (month <= o.month)
            return true;
        return false;
    }
    bool operator >= (const yearMonth &o)
    {
        if (year > o.year)
            return true;
        if (year < o.year)
            return false;
        if (month >= o.month)
            return true;
        return false;
    }
    operator QString() const
    {
        QString ret;
        ret.append(QString::number(year));
        ret.append('/');
        ret.append(QString::number(month));
        return ret;
    }
    friend int operator-(const yearMonth &a, const yearMonth &b)
    {
        return (a.year-b.year)*12+a.month-b.month;
    }
    friend yearMonth operator+(const yearMonth &a, const int &b)
    {
        int month = a.month+b;
        int year = a.year;
        if (month > 12)
        {
            month -= 12;
            year++;
        }
        yearMonth ret(year,month);
        return ret;
    }

};



#endif // DATASTRUCT_H
