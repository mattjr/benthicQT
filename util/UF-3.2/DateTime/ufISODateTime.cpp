#include "ufISODateTime.h"

using namespace UF::Time;

ISODateTime::ISODateTime(void) :
  secondsInDay(86400)
{
}

ISODateTime::~ISODateTime(void)
{
}

ISODateTime::ISODateTime(std::string const &date_time) :
  secondsInDay(86400)
{
  this->SetISODateTime(date_time);
}

std::string ISODateTime::GetISODateTime (int const & precision,
                              char const & dateSep,
                              char const & timeSep,
                              char const & dateTimeSep,
                              char const & UTCMarker) const
{
  return this->ISODateTimeStr(this->year,this->month,this->day,
    this->hour,this->minute,this->second,this->offset,
    precision, dateSep, timeSep, dateTimeSep, UTCMarker);
}

bool ISODateTime::SetISODateTime (std::string const & DateTime)
{
  bool Ok = this->UF::Time::DateTime::ISODateTime(DateTime,this->year,this->month,this->day,this->hour,this->minute,this->second,this->offset); 
  return Ok;
}

int ISODateTime::GetDOW()
{
  return (int)this->DoW(this->year,this->month,this->day);
}

int ISODateTime::GetDOY()
{
  return (int)this->DoY(this->year,this->month,this->day);
}

void ISODateTime::ToUTC()
{
  double jdn;
  double fod;
  jdn = this->GetJDN();
  fod = this->GetFoD();
  fod -= double(this->offset) / secondsInDay;
  this->NormalizeJDFOD(jdn,fod);
  this->SetJDN(jdn);
  this->SetFoD(fod);
  this->offset = 0;
}

void ISODateTime::ToLocalTime(int const & Offset)
{
  double jdn;
  double fod;
  this->ToUTC();
  jdn = this->GetJDN();
  fod = this->GetFoD();
  fod += double(Offset) / secondsInDay;
  this->NormalizeJDFOD(jdn,fod);
  this->SetJDN(jdn);
  this->SetFoD(fod);
  this->offset = Offset;
}

double ISODateTime::GetJDN()
{
  return this->JulianDayNumber(this->year,this->month,this->day);
}

double ISODateTime::GetFoD()
{
  return this->ToFoD(this->hour,this->minute,this->second);
}

void ISODateTime::SetJDN(double const & jdn)
{
  this->CalendarDate(jdn,this->year,this->month,this->day);
}

void ISODateTime::SetFoD(double const & fod,int const & secondsPrecision)
{
  double jdn = this->GetJDN();
  double lfod  = fod;
  this->NormalizeJDFOD(jdn,lfod);
  this->CalendarDate(jdn,this->year,this->month,this->day);
  this->FromFoD(lfod,this->hour,this->minute,this->second,secondsPrecision);
}

bool ISODateTime::IsValidOffset()
{
  double offsetMax = 12 * 3600; // Maximum timezone in hours.
  return this->offset >= -offsetMax && this->offset <= offsetMax;
}

bool ISODateTime::IsValidDateTime()
{
  bool valid = this->IsValidYMD(this->year,this->month,this->day);
  double fod = this->ToFoD(this->hour,this->minute,this->second);
  valid &= fod >= 0 && fod < 1;
  valid &= this->IsValidOffset();
  return valid;
}
