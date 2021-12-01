
#include "gtest/gtest.h"
#include "Core/DateTime.h"
#include "Core/Unused.h"
#include <iostream>
#include <thread>

using namespace Realisim;
using namespace Core;
using namespace std;


namespace
{
    // define a macro to test equality in order to ease readability
    // and maintain the line reporting mechanism of gtest
    //
#define EXPECT_EQ_DATETIME_WITHOUT_TIMEZONE(a, b) \
{ \
    EXPECT_EQ(a.getHour(), b.getHour());\
    EXPECT_EQ(a.getMinute(), b.getMinute());\
    EXPECT_EQ(a.getSecond(), b.getSecond());\
    EXPECT_EQ(a.getMillisecond(), b.getMillisecond());\
    \
    EXPECT_EQ(a.getYear(), b.getYear());\
    EXPECT_EQ(a.getMonth(), b.getMonth());\
    EXPECT_EQ(a.getDay(), b.getDay());\
}

    void print(const DateTime& iDt)
    {
        UNUSED(iDt);
//#define DOPRINT 1
#ifdef DOPRINT
        cout << iDt.toString() << 
            " offset from utc: " << iDt.getOffsetFromUtcInHours() << 
            " | DST flag: " << (iDt.hasDaylightSavingTimeFlag() ? "On" : "Off") << 
            " | utc time: " << iDt.toUtc().toString() << endl;
#endif // !NDEBUG
    }

    // grab current time in ms since epoch, sleep for iSleepInMs
    // and ensure the result is in tolerance ( +- 15 milli, since this is a common resolution on windows)
    //
    void testMillisecondsDelta(int iSleepInMs)
    {
        // create a delta from now in ms
        int64_t nowInMilliseconds = DateTime::millisecondsSinceEpoch();
        this_thread::sleep_for( chrono::duration<int, std::milli>(iSleepInMs) ); // x milli
        int64_t laterInMilliseconds = DateTime::millisecondsSinceEpoch();

        // validate delta
        int64_t delta = laterInMilliseconds - nowInMilliseconds;
        const bool deltaIsInRange = delta >= (iSleepInMs - 15) && delta <= (iSleepInMs + 15);
        EXPECT_TRUE(deltaIsInRange);

        // create dateTime from milli and validate conversion back to milli
        DateTime dtNow = DateTime::fromMillisecondsSinceEpoch(nowInMilliseconds);
        EXPECT_TRUE( dtNow.toMillisecondsSinceEpoch() == nowInMilliseconds );

        // ensure later in DateTime is equal to Now + delta
        DateTime dtLater = DateTime::fromMillisecondsSinceEpoch(laterInMilliseconds);
        dtNow.addMilliseconds(delta);
        EXPECT_TRUE( dtNow == dtLater );

        print(dtNow);
    }
}

// Test the operator first, since we will use the operators to validate the whole things.
//
TEST(DateTime, operators)
{
    DateTime reference(2018, 1, 1, 13, 0, 0, -5); //EST

    // operator ==
    {
        DateTime d(reference);
        EXPECT_TRUE( d == reference );
        EXPECT_FALSE( d != reference );
    }

    // operator !=
    {
        DateTime d;
        EXPECT_TRUE( d != reference );
        EXPECT_FALSE( d == reference );
    }

    // operator >,<
    {
        DateTime d;
        EXPECT_TRUE( d < reference );
        EXPECT_FALSE( d > reference );
    }

    // operator <=, >=
    {
        DateTime greater(2019, 1, 1, 12, 13, 14, 4.22);
        DateTime equal(reference);
        DateTime smaller(2017, 1, 1, 12, 13, 14, 4.22);

        EXPECT_TRUE( greater >= reference );
        EXPECT_FALSE( greater <= reference );

        EXPECT_TRUE( equal >= reference );
        EXPECT_TRUE( equal <= reference );

        EXPECT_TRUE( smaller <= reference );
        EXPECT_FALSE( smaller >= reference );
    }

}

TEST(DateTime, constructors)
{
    DateTime dt;
    print(dt);

    DateTime reference(1980, 7, 3, 12, 25, 35);
    print(reference);

    DateTime dt2(1980, 7, 3, 12, 25, 35, -2);
    print(dt2);
    EXPECT_EQ_DATETIME_WITHOUT_TIMEZONE(reference, dt2);
    EXPECT_EQ(-2.0, dt2.getOffsetFromUtcInHours());
    

    DateTime dt3(1980, 7, 3, 12, 25, 35, +2);
    print(dt3);
    EXPECT_EQ_DATETIME_WITHOUT_TIMEZONE(reference, dt3);
    EXPECT_EQ(2.0, dt3.getOffsetFromUtcInHours());

    DateTime dt4(dt3);
    EXPECT_EQ_DATETIME_WITHOUT_TIMEZONE(dt3, dt4);
    EXPECT_EQ(dt3.getOffsetFromUtcInHours(), dt4.getOffsetFromUtcInHours());

    // create default DateTime and customize
    DateTime dt5;
    dt5.setYear(-100);
    dt5.setMonth(2);
    dt5.setDay(25);
    dt5.setHour(13);
    dt5.setMinute(25);
    dt5.setSecond(35);
    print(dt5);

    EXPECT_EQ_DATETIME_WITHOUT_TIMEZONE(dt5, DateTime(-100, 2, 25, 13, 25, 35));
}

TEST(DateTime, ConversionsFromLocalToUtc)
{
    DateTime local = DateTime::currentDateTime();
    DateTime local2; local2.now();
    EXPECT_EQ_DATETIME_WITHOUT_TIMEZONE(local, local2);
    print(local);

    DateTime reference(2018, 3, 1, 12, 0, 0);
    print(reference);
    EXPECT_EQ_DATETIME_WITHOUT_TIMEZONE(reference, reference.toUtc());

    // test all utc offset from -20 to +20
    for(int i = -20; i <= 20; ++i)
    {        
        DateTime b(reference);
        b.setOffsetFromUtcInHours(i);
        print(b);

        // make an utc time
        // take the reference and add -1*offset
        //
        DateTime utc(reference);
        utc.addHours(-i);
        print(utc);
        
        EXPECT_EQ_DATETIME_WITHOUT_TIMEZONE(reference, b);
        EXPECT_EQ(b.getOffsetFromUtcInHours(), i);

        EXPECT_EQ_DATETIME_WITHOUT_TIMEZONE(b.toUtc(), utc);
        EXPECT_EQ(utc.getOffsetFromUtcInHours(), 0);
    }
}

TEST(DateTime, timezoneConversion)
{
    // january first 2018 00:00:00 UTC
    DateTime reference(2018, 1, 1, 0, 0, 0);

    {
        DateTime a = reference.toTimeZone(-5);
        print(a);

        DateTime b(2017, 12, 31, 19, 0, 0);
        print(b);

        EXPECT_EQ_DATETIME_WITHOUT_TIMEZONE(a, b);
        EXPECT_EQ_DATETIME_WITHOUT_TIMEZONE(a.toUtc(), reference);
        EXPECT_EQ(a.getOffsetFromUtcInHours(), -5);
    }

    {
        DateTime a = reference.toTimeZone(8);
        print(a);

        DateTime b(2018, 1, 1, 8, 0, 0);
        print(b);

        EXPECT_EQ_DATETIME_WITHOUT_TIMEZONE(a, b);
        EXPECT_EQ_DATETIME_WITHOUT_TIMEZONE(a.toUtc(), reference);
        EXPECT_EQ(a.getOffsetFromUtcInHours(), 8);
    }
    
    // test with a time in a timezone
    DateTime referenceUtc(2018, 1, 1, 5, 0, 0);
    DateTime reference2(2018, 1, 1, 0, 0, 0, -5);
    print(reference2);
    {
        DateTime a = reference2.toTimeZone(-5);
        print(a);

        DateTime b(2018, 1, 1, 0, 0, 0, -5);
        print(b);

        EXPECT_EQ_DATETIME_WITHOUT_TIMEZONE(a, b);
        EXPECT_EQ_DATETIME_WITHOUT_TIMEZONE(a.toUtc(), referenceUtc.toUtc());
        EXPECT_EQ(a.getOffsetFromUtcInHours(), -5);
    }

    {
        DateTime a = reference2.toTimeZone(-8);
        print(a);

        DateTime b(2017, 12, 31, 21, 0, -8);
        print(b);

        EXPECT_EQ_DATETIME_WITHOUT_TIMEZONE(a, b);
        EXPECT_EQ_DATETIME_WITHOUT_TIMEZONE(a.toUtc(), referenceUtc.toUtc());
        EXPECT_EQ(a.getOffsetFromUtcInHours(), -8);
    }

    {
        DateTime a = reference2.toTimeZone(8);
        print(a);

        DateTime b(2018, 1, 1, 13, 0, 0);
        print(b);

        EXPECT_EQ_DATETIME_WITHOUT_TIMEZONE(a, b);
        EXPECT_EQ_DATETIME_WITHOUT_TIMEZONE(a.toUtc(), referenceUtc.toUtc());
        EXPECT_EQ(a.getOffsetFromUtcInHours(), 8);
    }
}

TEST(DateTime, functions)
{
    {
        DateTime dt(2018, 1, 1);
        EXPECT_EQ( dt.getWeekday(), DateTime::wMonday );
    }

    {
        DateTime dt = DateTime::currentDateTime();
        dt.setTime(10, 0, 0);
        dt.setOffsetFromUtcInHours(0.0);

        print( dt.toUtc() );
        EXPECT_EQ(dt.getHour(), 10);
        EXPECT_EQ(dt.getMinute(), 0);
        EXPECT_EQ(dt.getSecond(), 0);
    }

    // millisecondsSinceEpoch and fromMillisecondsSinceEpoch
    {
        DateTime t = DateTime::currentDateTime();
        int64_t nowInMilliseconds = DateTime::millisecondsSinceEpoch();
        DateTime dtNow = DateTime::fromMillisecondsSinceEpoch(nowInMilliseconds);

        EXPECT_EQ(t.getDay(), dtNow.getDay());
        EXPECT_EQ(t.getMonth(), dtNow.getMonth());
        EXPECT_EQ(t.getYear(), dtNow.getYear());
        EXPECT_EQ(t.getHour(), dtNow.getHour());
        EXPECT_EQ(t.getMinute(), dtNow.getMinute());
        EXPECT_EQ(t.getSecond(), dtNow.getSecond());
        // do not check milliseconds here...

        testMillisecondsDelta(1);
        testMillisecondsDelta(2);
        testMillisecondsDelta(3);
        testMillisecondsDelta(10);
        testMillisecondsDelta(15);
        testMillisecondsDelta(20);
        testMillisecondsDelta(25);
        testMillisecondsDelta(100);
        testMillisecondsDelta(200);
        testMillisecondsDelta(1000);
    }
}

TEST(DateTime, aritmetics)
{
    DateTime reference(2018, 1, 1, 0, 0, 0, -2);
    print(reference);

    // milliseconds
    {
        DateTime a(reference);
        a.addMilliseconds(127);
        print(a);

        DateTime b(reference);
        b.setMillisecond(127);
        print(b);

        EXPECT_EQ_DATETIME_WITHOUT_TIMEZONE(a, b);
        EXPECT_EQ(a.getOffsetFromUtcInHours(), -2);
    }
    {
        DateTime a(reference);
        a.addMilliseconds(12876);
        print(a);

        DateTime b(reference);
        b.setSecond(12);
        b.setMillisecond(876);
        print(b);

        EXPECT_EQ_DATETIME_WITHOUT_TIMEZONE(a, b);
    }
    {
        DateTime a(reference);
        a.addMilliseconds(124876);
        print(a);

        DateTime b(reference);
        b.setMinute(2);
        b.setSecond(4);
        b.setMillisecond(876);
        print(b);

        EXPECT_EQ_DATETIME_WITHOUT_TIMEZONE(a, b);
    }


    // seconds as doubles
    {
        DateTime a(reference);
        a.addSeconds(22.4);
        print(a);

        DateTime b(reference);
        b.setSecond(22);
        b.setMillisecond(400);
        print(b);

        EXPECT_EQ_DATETIME_WITHOUT_TIMEZONE(a, b);
        EXPECT_EQ(a.getOffsetFromUtcInHours(), -2);
    }
    {
        DateTime a(reference);
        a.addSeconds(122.400);
        print(a);

        DateTime b(reference);
        b.setMinute(2);
        b.setSecond(2);
        b.setMillisecond(400);
        print(b);

        EXPECT_EQ_DATETIME_WITHOUT_TIMEZONE(a, b);
    }

    // seconds as int
    {
        DateTime a(reference);
        a.addSeconds(22);
        print(a);

        DateTime b(reference);
        b.setSecond(22);
        print(b);

        EXPECT_EQ_DATETIME_WITHOUT_TIMEZONE(a, b);
        EXPECT_EQ(a.getOffsetFromUtcInHours(), -2);
    }
    {
        DateTime a(reference);
        a.addSeconds(122);
        print(a);

        DateTime b(reference);
        b.setMinute(2);
        b.setSecond(2);
        print(b);

        EXPECT_EQ_DATETIME_WITHOUT_TIMEZONE(a, b);
    }

    // minutes
    {
        DateTime a(reference);
        a.addMinutes(22);
        print(a);

        DateTime b(reference);
        b.setMinute(22);
        print(b);

        EXPECT_EQ_DATETIME_WITHOUT_TIMEZONE(a, b);
    }
    {
        DateTime a(reference);
        a.addMinutes(122);
        print(a);

        DateTime b(reference);
        b.setHour(2);
        b.setMinute(2);
        print(b);

        EXPECT_EQ_DATETIME_WITHOUT_TIMEZONE(a, b);
    }

    // hours
    {
        DateTime a(reference);
        a.addHours(8);
        print(a);

        DateTime b(reference);
        b.setHour(8);
        print(b);

        EXPECT_EQ_DATETIME_WITHOUT_TIMEZONE(a, b);
    }
    {
        DateTime a(reference);
        a.addHours(37);
        print(a);

        DateTime b(reference);
        b.setDay(2);
        b.setHour(13);
        print(b);

        EXPECT_EQ_DATETIME_WITHOUT_TIMEZONE(a, b);
    }
    
    // days
    {
        DateTime a(reference);
        a.addDays(8);
        print(a);

        DateTime b(reference);
        b.setDay(9);
        print(b);

        EXPECT_EQ_DATETIME_WITHOUT_TIMEZONE(a, b);
    }
    {
        DateTime a(reference);
        a.addDays(37);
        print(a);

        // january has 31 days...
        DateTime b(reference);
        b.setMonth(2);
        b.setDay(7);
        print(b);

        EXPECT_EQ_DATETIME_WITHOUT_TIMEZONE(a, b);
    }

    // months
    {
        DateTime a(reference);
        a.addMonths(6);
        print(a);

        DateTime b(reference);
        b.setMonth(7);
        print(b);

        EXPECT_EQ_DATETIME_WITHOUT_TIMEZONE(a, b);
    }
    {
        DateTime a(reference);
        a.addMonths(37);
        print(a);

        DateTime b(reference);
        b.setYear(2021);
        b.setMonth(2);
        print(b);

        EXPECT_EQ_DATETIME_WITHOUT_TIMEZONE(a, b);
    }

    // Years
    {
        DateTime a(reference);
        a.addYears(6);
        print(a);

        DateTime b(reference);
        b.setYear(2018 + 6);
        print(b);

        EXPECT_EQ_DATETIME_WITHOUT_TIMEZONE(a, b);
    }
    {
        DateTime a(reference);
        a.addYears(467);
        print(a);

        DateTime b(reference);
        b.setYear(2018 + 467);
        print(b);

        EXPECT_EQ_DATETIME_WITHOUT_TIMEZONE(a, b);
    }
}


TEST(DateTime, specialCases)
{
    DateTime ref = DateTime::currentDateTime();
    DateTime dt(ref);

    dt.addSeconds(0);
    EXPECT_EQ_DATETIME_WITHOUT_TIMEZONE(dt, ref);

    dt.setOffsetFromUtcInHours(-8);
    dt.addSeconds(0);
    EXPECT_EQ_DATETIME_WITHOUT_TIMEZONE(dt, ref);
}