
#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS 1
#endif

#include "Core/Unused.h"
#include "Core/DateTime.h"
#include "Math/CommonMath.h"
#include <chrono>

using namespace Realisim;
using namespace Core;
using namespace std;

namespace
{
    const double kSecondsToHours = 1.0/3600.0;
}

//---------------------------------------------------------------------------------------------------------------------
// 
DateTime::DateTime() :
    mLocalTimeInfo(),
    mUtcTime(0),
    mMilliseconds(0),
    mOffsetFromUtcInSeconds(0),
    mLocalTimeOffsetFromUtcInSeconds(0),
    mHasDaylightSavingTimeFlag(false),
    mApplyDaylightSavingTime(false)
{
    computeLocalTimeOffsetFromUtc();

    struct tm *pTimeInfo = gmtime(&mUtcTime);
    mLocalTimeInfo = *pTimeInfo;

    mUtcTime = mktime(&mLocalTimeInfo);
}

//---------------------------------------------------------------------------------------------------------------------
DateTime::DateTime(int iYear, int iMonth, int iDay) :
    mLocalTimeInfo(),
    mUtcTime(0),
    mMilliseconds(0),
    mOffsetFromUtcInSeconds(0),
    mLocalTimeOffsetFromUtcInSeconds(0),
    mHasDaylightSavingTimeFlag(false),
    mApplyDaylightSavingTime(false)
{
    computeLocalTimeOffsetFromUtc();

    mLocalTimeInfo.tm_year = clamp(iYear - 1900, 0, 9999);
    mLocalTimeInfo.tm_mon = clamp(iMonth - 1, 0, 11); // voir doc 
    mLocalTimeInfo.tm_mday = clamp(iDay, 1, 31);

    makeUtcTimeFromLocalInfo();
}

//---------------------------------------------------------------------------------------------------------------------
// http://www.cplusplus.com/reference/ctime/tm/
//
// iYear -> [1900, 9999]
// iMonth -> [1, 12]
// iDay -> [1, 31]
// iHour -> [0, 23]
// iMinute -> [0, 59] 
// iSecond -> [0, 60*] (60 to accomodate leap seconds... usually [0, 59] )
// iTimeZone -> [-11.9999, 12.999]
//
DateTime::DateTime(int iYear, int iMonth, int iDay, int iHour, int iMinute, int iSecond, double iOffsetInHoursFromUtc /*=0*/) :
    mLocalTimeInfo(),
    mUtcTime(0),
    mMilliseconds(0),
    mOffsetFromUtcInSeconds(0),
    mLocalTimeOffsetFromUtcInSeconds(0),
    mHasDaylightSavingTimeFlag(false),
    mApplyDaylightSavingTime(false)
{
    computeLocalTimeOffsetFromUtc();
    
    mOffsetFromUtcInSeconds = (int64_t)(iOffsetInHoursFromUtc * 3600);

    mLocalTimeInfo.tm_year = clamp(iYear - 1900, 0, 9999);
    mLocalTimeInfo.tm_mon = clamp(iMonth - 1, 0, 11); // voir doc 
    mLocalTimeInfo.tm_mday = clamp(iDay, 1, 31);

    mLocalTimeInfo.tm_hour = clamp(iHour, 0, 23);
    mLocalTimeInfo.tm_min = clamp(iMinute, 0, 59);
    mLocalTimeInfo.tm_sec = clamp(iSecond, 0, 60);

    makeUtcTimeFromLocalInfo();
}


//---------------------------------------------------------------------------------------------------------------------
DateTime::~DateTime()
{}

//---------------------------------------------------------------------------------------------------------------------
void DateTime::addDays(int iV)
{
    addHours(iV * 24);
}

//---------------------------------------------------------------------------------------------------------------------
void DateTime::addHours(double iV)
{
    addMinutes(iV * 60);
}

//---------------------------------------------------------------------------------------------------------------------
void DateTime::addMilliseconds(int64_t iV)
{
    int64_t totalMs = iV + mMilliseconds;
    const int64_t seconds = (int64_t)(totalMs * 0.001);
    mMilliseconds = int16_t(totalMs - seconds*1000);

    mUtcTime += seconds;
    makeLocalTimeInfoFromUtcTime();
}

//---------------------------------------------------------------------------------------------------------------------
void DateTime::addMinutes(double iV)
{
    addSeconds( (int64_t)(iV *60) );
}

//---------------------------------------------------------------------------------------------------------------------
void DateTime::addMonths(int iV)
{
    const int total = getMonth() + iV;

    int yearsToAdd = total / 12;
    int finalMonth = total % 12;

    setMonth(finalMonth);
    setYear( getYear() + yearsToAdd );
}

//---------------------------------------------------------------------------------------------------------------------
void DateTime::addSeconds(int iV)
{
    addSeconds((int64_t)iV);
}

//---------------------------------------------------------------------------------------------------------------------
void DateTime::addSeconds(int64_t iV)
{
    mUtcTime += iV;
    makeLocalTimeInfoFromUtcTime();
}

//---------------------------------------------------------------------------------------------------------------------
void DateTime::addSeconds(double iV)
{
    addMilliseconds((int64_t)(iV * 1000.0));
}

//---------------------------------------------------------------------------------------------------------------------
void DateTime::addYears(int iV)
{
    setYear( getYear() + iV );
}

//---------------------------------------------------------------------------------------------------------------------
void DateTime::applyDaylightSavingTime(bool iV)
{
    mApplyDaylightSavingTime = iV;
}

//---------------------------------------------------------------------------------------------------------------------
void DateTime::computeLocalTimeOffsetFromUtc()
{
    time_t now;
    time(&now);

    struct tm *utcInfo = gmtime(&now);
    time_t utcTime = mktime(utcInfo);

    struct tm *localInfo = localtime(&now);
    time_t localTime = mktime(localInfo);

    mLocalTimeOffsetFromUtcInSeconds = (int64_t)difftime(localTime, utcTime);
}

//---------------------------------------------------------------------------------------------------------------------
DateTime DateTime::currentDateTime()
{
    DateTime r;
    r.now();
    return r;
}

//---------------------------------------------------------------------------------------------------------------------
DateTime DateTime::fromMillisecondsSinceEpoch(int64_t iV)
{
    DateTime t;

    t.mOffsetFromUtcInSeconds = t.mLocalTimeOffsetFromUtcInSeconds;
    t.mUtcTime = 0;

    const int64_t seconds = (int64_t)(iV * 0.001);
    t.mMilliseconds = int16_t(iV - seconds*1000);

    t.mUtcTime += seconds;
    t.makeLocalTimeInfoFromUtcTime();
    return t;
}

//---------------------------------------------------------------------------------------------------------------------
void DateTime::fromUtcTime_t(const time_t& iUtcTime)
{
    mUtcTime = iUtcTime;

    struct tm *localInfo = gmtime(&mUtcTime);
    mLocalTimeInfo = tm(*localInfo);

    mOffsetFromUtcInSeconds = 0;
}

//---------------------------------------------------------------------------------------------------------------------
double DateTime::getOffsetFromUtcInHours() const
{
    int64_t offsetFromUtcInSeconds = mOffsetFromUtcInSeconds;

    // add 1 hours to the offset if daylight saving is applied
    if (hasDaylightSavingTimeFlag() && isDaylightSavingTimeApplied())
    {
        offsetFromUtcInSeconds += 3600;
    }

    return offsetFromUtcInSeconds * kSecondsToHours;
}

//---------------------------------------------------------------------------------------------------------------------
int DateTime::getDay() const
{
    return mLocalTimeInfo.tm_mday;
}

//---------------------------------------------------------------------------------------------------------------------
int DateTime::getMillisecond() const
{
    return mMilliseconds;
}

//---------------------------------------------------------------------------------------------------------------------
int DateTime::getHour() const
{
    return mLocalTimeInfo.tm_hour;
}

//---------------------------------------------------------------------------------------------------------------------
int DateTime::getMinute() const
{
    return mLocalTimeInfo.tm_min;
}

//---------------------------------------------------------------------------------------------------------------------
int DateTime::getMonth() const
{
    return mLocalTimeInfo.tm_mon + 1; // [0, 11] dans struct tm, notre interface offre [1,12]
}

//---------------------------------------------------------------------------------------------------------------------
int DateTime::getSecond() const
{
    return mLocalTimeInfo.tm_sec;
}

//---------------------------------------------------------------------------------------------------------------------
DateTime::Weekday DateTime::getWeekday() const
{
    return (Weekday)mLocalTimeInfo.tm_wday;
}

//---------------------------------------------------------------------------------------------------------------------
int DateTime::getYear() const
{
    return mLocalTimeInfo.tm_year + 1900; // see doc for explanation on 1900...
}

//---------------------------------------------------------------------------------------------------------------------
bool DateTime::hasDaylightSavingTimeFlag() const
{
    return mHasDaylightSavingTimeFlag;
}

//---------------------------------------------------------------------------------------------------------------------
bool DateTime::isDaylightSavingTimeApplied() const
{
    return mApplyDaylightSavingTime;
}

//---------------------------------------------------------------------------------------------------------------------
time_t DateTime::localTimeToUtcTime(time_t iLocalTime)
{
    time_t r = iLocalTime + mLocalTimeOffsetFromUtcInSeconds - mOffsetFromUtcInSeconds;
    return r;
}

//---------------------------------------------------------------------------------------------------------------------
void DateTime::makeLocalTimeInfoFromUtcTime()
{
    //rebuild localTime
    time_t localTime = utcTimeToLocalTime(mUtcTime);

    struct tm *localTimeInfo = localtime(&localTime);
    mLocalTimeInfo = tm(*localTimeInfo);

    // localtime was modified by mktime.
    // use it's week day and DST flag
    //
    mLocalTimeInfo.tm_wday = localTimeInfo->tm_wday;
    mHasDaylightSavingTimeFlag = localTimeInfo->tm_isdst == 1 ? true : false;
}

//---------------------------------------------------------------------------------------------------------------------
void DateTime::makeUtcTimeFromLocalInfo()
{
    struct tm localtime = mLocalTimeInfo; // make a copy to prevent overwritting mLocalTimeInfo
    mUtcTime = localTimeToUtcTime(mktime(&localtime));

    // localtime was modified by mktime.
    // use it's week day and DST flag
    //
    mLocalTimeInfo.tm_wday = localtime.tm_wday;
    mHasDaylightSavingTimeFlag = localtime.tm_isdst == 1 ? true : false;
}


//---------------------------------------------------------------------------------------------------------------------
int64_t DateTime::millisecondsSinceEpoch()
{
    DateTime dt;
    dt.now();
    return dt.toMillisecondsSinceEpoch();
}

//---------------------------------------------------------------------------------------------------------------------
// returns the current local time with correct relation to UTC time.
// milliseconds are taken into account.
//
void DateTime::now()
{
    using namespace std::chrono;

    // get now time from system clock
    auto now = system_clock::now();

    // set resolution to milliseconds
    auto now_ms = time_point_cast<milliseconds>(now);

    // get a duration since epoch in milliseconds resolution
    auto value = now_ms.time_since_epoch();
    int64_t millisecondsSinceEpoch = value.count();

    // create a time_t in seconds
    const int64_t secondsSinceEpoch = (int64_t)(millisecondsSinceEpoch * 0.001);
    // keep the remainder in milliseconds
    mMilliseconds = int16_t(millisecondsSinceEpoch - secondsSinceEpoch*1000);

    // construct the time info structure
    struct tm *pTimeInfo = localtime(&secondsSinceEpoch);
    mLocalTimeInfo = *pTimeInfo;

    mOffsetFromUtcInSeconds = mLocalTimeOffsetFromUtcInSeconds;

    makeUtcTimeFromLocalInfo();
}

//---------------------------------------------------------------------------------------------------------------------
bool DateTime::operator<(const DateTime& iLhs) const
{
    return mUtcTime < iLhs.mUtcTime;
}

//---------------------------------------------------------------------------------------------------------------------
bool DateTime::operator>(const DateTime& iLhs) const
{
    return mUtcTime > iLhs.mUtcTime;
}

//---------------------------------------------------------------------------------------------------------------------
bool DateTime::operator==(const DateTime& iLhs) const
{
    return mUtcTime == iLhs.mUtcTime &&
        mMilliseconds == iLhs.mMilliseconds &&
        mOffsetFromUtcInSeconds == iLhs.mOffsetFromUtcInSeconds;
}

//---------------------------------------------------------------------------------------------------------------------
bool DateTime::operator!=(const DateTime& iLhs) const
{
    return !operator==(iLhs);
}

//---------------------------------------------------------------------------------------------------------------------
bool DateTime::operator<=(const DateTime& iLhs) const
{
    return mUtcTime <= iLhs.mUtcTime;
}

//---------------------------------------------------------------------------------------------------------------------
bool DateTime::operator>=(const DateTime& iLhs) const
{
    return mUtcTime >= iLhs.mUtcTime;
}

//---------------------------------------------------------------------------------------------------------------------
void DateTime::setDate(int iYear, int iMonth, int iDay)
{
    mLocalTimeInfo.tm_year = clamp(iYear - 1900, 0, 9999);
    mLocalTimeInfo.tm_mon = clamp(iMonth - 1, 0, 11); // voir doc 
    mLocalTimeInfo.tm_mday = clamp(iDay, 1, 31);

    makeUtcTimeFromLocalInfo();
}

//---------------------------------------------------------------------------------------------------------------------
void DateTime::setDay(int iV)
{
    mLocalTimeInfo.tm_mday = clamp(iV, 1, 31);
    makeUtcTimeFromLocalInfo();
}

//---------------------------------------------------------------------------------------------------------------------
void DateTime::setHour(int iV)
{
    mLocalTimeInfo.tm_hour = clamp(iV, 0, 23);
    makeUtcTimeFromLocalInfo();
}

//---------------------------------------------------------------------------------------------------------------------
void DateTime::setMillisecond(int iV)
{
    mMilliseconds = (int16_t)clamp(iV, 0, 999);
}

//---------------------------------------------------------------------------------------------------------------------
void DateTime::setMinute(int iV)
{
    mLocalTimeInfo.tm_min = clamp(iV, 0, 59);
    makeUtcTimeFromLocalInfo();
}

//---------------------------------------------------------------------------------------------------------------------
void DateTime::setMonth(int iV)
{
    mLocalTimeInfo.tm_mon = clamp(iV - 1, 0, 11); // voir doc 
    makeUtcTimeFromLocalInfo();
}

//---------------------------------------------------------------------------------------------------------------------
void DateTime::setOffsetFromUtcInHours(double iV)
{
    mOffsetFromUtcInSeconds = (int64_t)(iV * 3600);
    makeUtcTimeFromLocalInfo();
}

//---------------------------------------------------------------------------------------------------------------------
void DateTime::setSecond(int iV)
{
    mLocalTimeInfo.tm_sec = clamp(iV, 0, 60);
    makeUtcTimeFromLocalInfo();
}

//---------------------------------------------------------------------------------------------------------------------
void DateTime::setTime(int iHour, int iMinute, int iSecond)
{
    mLocalTimeInfo.tm_hour = clamp(iHour, 0, 23);
    mLocalTimeInfo.tm_min = clamp(iMinute, 0, 59);
    mLocalTimeInfo.tm_sec = clamp(iSecond, 0, 60);

    makeUtcTimeFromLocalInfo();
}

//---------------------------------------------------------------------------------------------------------------------
void DateTime::setYear(int iV)
{
    mLocalTimeInfo.tm_year = clamp(iV - 1900, 0, 9999);
    makeUtcTimeFromLocalInfo();
}

//---------------------------------------------------------------------------------------------------------------------
int64_t DateTime::toMillisecondsSinceEpoch() const
{
    return mUtcTime * 1000 + mMilliseconds;
}

//---------------------------------------------------------------------------------------------------------------------
// retourne la date en string avec le format Www Mmm dd hh:mm:ss yyyy
//
// pour un custom format... voir http://www.cplusplus.com/reference/ctime/strftime/
//
string DateTime::toString() const
{
    // asctime returns a char * terminating with \n\0.
    // we will remove the \n
    string t(asctime(&mLocalTimeInfo));
    return string(t.c_str(), t.size() - 1);
}

//---------------------------------------------------------------------------------------------------------------------
time_t DateTime::toUtcTime_t() const
{
    return mUtcTime;
}

//---------------------------------------------------------------------------------------------------------------------
DateTime DateTime::toTimeZone( double iTimeZoneInHours ) const
{
    time_t utcTime = toUtcTime_t();
    utcTime += (int64_t)(iTimeZoneInHours * 3600); //add seconds...

    DateTime r;
    r.fromUtcTime_t(utcTime);
    r.mMilliseconds = mMilliseconds;
    r.makeLocalTimeInfoFromUtcTime();
    r.setOffsetFromUtcInHours(iTimeZoneInHours);
    return r;
}

//---------------------------------------------------------------------------------------------------------------------
DateTime DateTime::toUtc() const
{
    DateTime r;
    r.fromUtcTime_t( mUtcTime );
    r.mMilliseconds = mMilliseconds;
    return r;
}

//---------------------------------------------------------------------------------------------------------------------
time_t DateTime::utcTimeToLocalTime(time_t iUtcTime)
{
    return  iUtcTime - mLocalTimeOffsetFromUtcInSeconds + mOffsetFromUtcInSeconds;
}
