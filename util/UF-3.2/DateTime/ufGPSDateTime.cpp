#include "ufGPSDateTime.h"

using namespace UF::Time;

GPSDateTime::GPSDateTime(void)
 : weekNumber(-1)
 , secondsInWeek(-1)
 , secondsInDay(86400)
 , GPS0(JulianDayNumber(1980,1,6))
 , GPS0fod(19.0/secondsInDay)
{
}

GPSDateTime::~GPSDateTime(void)
{
}

std::string GPSDateTime::GetISODateTime (int const & UTCOffset) const
{
  double jdn;
  double fod;
  this->GetGPSTimeAsUTC(jdn, fod);
  fod += double(UTCOffset)/this->secondsInDay;
  this->NormalizeJDFOD(jdn,fod);
  return this->ISODateTimeStr(jdn,fod,UTCOffset);
}

bool GPSDateTime::SetISODateTime (std::string const & DateTime)
{
  double jdn;
  double fod;
  int utcOffset;
  bool Ok = this->ISODateTime(DateTime,jdn,fod,utcOffset);
  this->SetGPSTime(jdn,fod,utcOffset);
  return Ok;
}

void GPSDateTime::SetGPSTime (double const & jdn, double const & fod, int const & UTCOffset)
{
  double jdn1 = jdn;
  double fod1 = fod - double(UTCOffset)/this->secondsInDay;
  // We now have UTC.
  this->NormalizeJDFOD(jdn1,fod1);
  double tai_utc = this->LeapSecond(jdn1,fod1);
  fod1 += tai_utc/this->secondsInDay;
  this->NormalizeJDFOD(jdn1,fod1);
  // We now have TAI.
  // Subtract the eopch.
  fod1 -= this->GPS0fod;
  jdn1 -= this->GPS0;
  // Normalise.
  while (fod1 < 0)
  {
    fod1 += 1;
    jdn1 -= 1;
  }
  while (fod1 > 1)
  {
    fod1 -= 1;
    jdn1 += 1;
  }
  if ( jdn1 < 0 || fod1 < 0 )
  {
    // GPS Time is undefined.
   this->weekNumber = -1;
   this->secondsInWeek = -1;
  }
  else
  {
    this->weekNumber = int(jdn1 / 7);
    this->secondsInWeek = (fod1 + (jdn1 - this->weekNumber * 7) ) * this->secondsInDay;
  }
}

void GPSDateTime::GetGPSTimeAsUTC (double & jdn, double & fod) const
{
  jdn = this->weekNumber * 7 + int(this->secondsInWeek/this->secondsInDay);
  fod = (this->secondsInWeek - int(this->secondsInWeek/this->secondsInDay)*this->secondsInDay)/this->secondsInDay;
  // Add in the eopch.
  fod += this->GPS0fod;
  jdn += this->GPS0;
  this->NormalizeJDFOD(jdn,fod);
  double tai_utc = this->LeapSecond(jdn,fod);
  fod -= tai_utc/this->secondsInDay;
  this->NormalizeJDFOD(jdn,fod);
}

double GPSDateTime::GetTAIUTCDiff() const
{
  double jdn;
  double fod;
  this->GetGPSTimeAsUTC(jdn,fod);
  return this->LeapSecond(jdn,fod);
}
