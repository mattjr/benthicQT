#include "ufTimeConversions.h"

using namespace UF::Time;

TimeConversions* TimeConversions::pInstance_ = 0;
bool TimeConversions::destroyed_ = false;

TimeConversions::TimeConversions(void) :
  DayRange(41983-1) // Use one day less than the maximun range
  , Jd2000(2451544.5)
  , SecsInDay(86400.0)
  , InvalidTimeFlag(0xffffffff)
  , UncorrectedTimeFlag(0xe0000000)
{};

double TimeConversions::GPSDateToJDN ( double GPSDate ) const
{
  int year;
  int month;
  int day;
  int d;

  if ( GPSDate < 0 )
    d = (int)-GPSDate;
  else
    d = (int)GPSDate;
  day = int(d / 10000);
  d = d - day * 10000;
  month = int(d / 100);
  year = int(d - month * 100);

  year = year + 2000;
  if ( this->IsValidYMD(year, month, day ) )
    return this->JulianDayNumber(year,month,day);
  else
    return -1;
}

unsigned int TimeConversions::DateTimeToUI ( double const &  jdn, double const &  fod ) const
{
  double JDN = jdn;
  double FoD = fod;
  this->NormalizeJDFOD(JDN,FoD);
  double date = JDN - this->Jd2000;
  if ( date < 0 || date > this->DayRange )
    return this->InvalidTimeFlag;
  else
  {
    int hour;
    int minute;
    double second;
    this->ToHMS(FoD * 24.0, hour, minute, second);

    unsigned int period = (unsigned int)(( date + FoD ) * this->SecsInDay);
    return period;
  }
}

unsigned int TimeConversions::DateTimeToUI ( int const & year, int const & month, int const & day,
  int const &  hour, int const &  minute, double const & second ) const
{
  double jdn = this->JulianDayNumber(year,month,day);
  double fod = this->HMSToSeconds(hour,minute,second)/this->SecsInDay;
  if ( !this->IsValidYMD(year,month,day) || fod < 0 || fod > 1 )
  {
    return this->InvalidTimeFlag;
  }
  return this->DateTimeToUI(jdn,fod);
}

