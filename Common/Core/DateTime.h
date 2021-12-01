#pragma once

#include <stdint.h>
#include <string>
#include <time.h>

namespace Realisim
{
namespace Core
{
    /*-----------------------------------------------------------------------------------------------------------------
        Cette classe modelise une date un temps locale (avec timezone). Donc une date de calendrier (année, mois, jour)
        et une heure (hh:mm:ss zzz) avec un timeZone. Les millisecondes sont prises en compte.

        Les heures sont en format 24h donc entre [0, 23].

        Les plages de chaque champs:
            Year -> [1900, 9999]
            Month -> [1, 12]
            Day -> [1, 31]
            Hour -> [0, 23]
            Minute -> [0, 59] 
            Second -> [0, 60*] (60 to accomodate leap seconds... usually [0, 59] )
            Millisecond -> [0, 999] 
            OffsetInHoursFromUtc -> [-infinite, infinite]

        Le constructeurs avec parametres permet de créer une Date et temps local en spécifiant le timezone.

        Conversion vers UTC:
            voir methodes toUtc().
            
        Conversion vers Timezone x:
            voir méthode toTimeZone().

        Examples:
            // 1er janvier 2018 00:00:00, time zone 0. -> temps utc: 1er janvier 2018 00:00:00
            DateTime newYear(2018, 1, 1, 0, 0, 0); 

            // 1er janvier 2018 00:00:00, time zone -5. -> temps utc: 1er janvier 2018 05:00:00
            DateTime newYearInMontreal = newYear.toTimeZone(-5);
            DateTime newYearInMontrealUtc = newYearInMontreal.toUtc();

            DateTime myBirthday(1980, 7, 3);
            // what week day was my birthday when I was 20?
            myBirthday.addYears(20);
            DateTime::Weekday wd = myBirthday.getWeekDay();


        Details internes:
            
            Le tout tourne autour des fonctions std définies dans <ctime>
            voir http://www.cplusplus.com/reference/ctime/

            membres:
                struct tm mLocalTimeInfo; La date et le temps en local
                time_t mUtcTime; La representation UTC de la date et temps local
                int64_t mOffsetFromUtcInSeconds; La timeZone en secondes
                int64_t mLocalTimeOffsetFromUtcInSeconds; La timezone en secondes de la machine locale. Cette valueur
                    permet de mettre le temps local retourné par mktime en temps utc.
                bool mHasDaylightSavingTimeFlag; Indique si la date a le flag DST
                bool mApplyDaylightSavingTime; Indique si on applique le DST à la timezone. Ça affecte uniquement la
                    valeur retournée par getOffsetFromUtcInHours();

        Notes:
            This class is not thread safe because of call to localtime and gmtime.

            replacing with localtime_r and gmtime_r would make it thread safe...

            #ifdef WIN32
            #define localtime_r(_Time, _Tm) localtime_s(_Tm, _Time)
            #endif
    */
    class DateTime
    {
    public:
        DateTime();
        DateTime(int iYear, int iMonth, int iDay);
        DateTime(int iYear, int iMonth, int iDay, int iHour, int iMinute, int iSecond, double iOffsetInHoursFromUtc = 0.0); //local time with offset from utc
        DateTime(const DateTime&) = default;
        DateTime& operator=(const DateTime&) = default;
        ~DateTime();

        enum Weekday{wSunday = 0, wMonday, wTuesday, wWednesday, wThursday, wFriday, wSaturday};

        // operator >, <, ==, !=, <=, >=
        bool operator<(const DateTime&) const;
        bool operator>(const DateTime&) const;
        bool operator==(const DateTime&) const;
        bool operator!=(const DateTime&) const;
        bool operator<=(const DateTime&) const;
        bool operator>=(const DateTime&) const;

        void addDays(int iV); // en double ? pour les fonctions add?
        void addHours(double iV);
        void addMinutes(double iV);
        void addMonths(int iV);
        void addSeconds(int iV);
        void addSeconds(int64_t iV);
        void addSeconds(double iV);
        void addMilliseconds(int64_t iV);
        void addYears(int iV);
        void applyDaylightSavingTime(bool iV);

        static DateTime currentDateTime();
        static DateTime fromMillisecondsSinceEpoch(int64_t iV);
        void fromUtcTime_t(const time_t& iUtcTime);
        double getOffsetFromUtcInHours() const;
        int getDay() const;
        int getHour() const;
        int getMillisecond() const;
        int getMinute() const;
        int getMonth() const;
        int getSecond() const;
        Weekday getWeekday() const;
        int getYear() const;
        bool hasDaylightSavingTimeFlag() const;
        bool isDaylightSavingTimeApplied() const;
        static int64_t millisecondsSinceEpoch();
        void now();

        void setDate(int iYear, int iMonth, int iDay);
        void setDay(int iV);
        void setHour(int iV);
        void setMillisecond(int iV);
        void setMinute(int iV);
        void setMonth(int iV);
        void setSecond(int iV);
        void setTime(int iHour, int iMinute, int iSecond);
        void setOffsetFromUtcInHours(double iV);
        void setYear(int iV);

        int64_t toMillisecondsSinceEpoch() const;
        std::string toString() const;
        time_t toUtcTime_t() const;
        DateTime toTimeZone( double iTimeZoneInHours ) const;
        DateTime toUtc() const;

    protected:
        time_t localTimeToUtcTime(time_t iLocalTime);
        time_t utcTimeToLocalTime(time_t iUtcTime);

        void computeLocalTimeOffsetFromUtc();
        void makeLocalTimeInfoFromUtcTime();
        void makeUtcTimeFromLocalInfo();

        struct tm mLocalTimeInfo;
        time_t mUtcTime; // number of seconds since EPOCH
        int16_t mMilliseconds;
        int64_t mOffsetFromUtcInSeconds;
        int64_t mLocalTimeOffsetFromUtcInSeconds;
        bool mHasDaylightSavingTimeFlag;
        bool mApplyDaylightSavingTime;
    };
}
}