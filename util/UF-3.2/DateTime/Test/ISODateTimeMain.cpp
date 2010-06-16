#include "ufISODateTime.h"
#include "ufSystemTime.h"
#include "ufTimeConversions.h"
#include "ufLogger.h"
#include <iostream>
#include <iomanip>
#include <sstream>

#ifdef _MSC_VER
#include <sys/timeb.h>
#include <windows.h>
#else
#include <sys/time.h>
#endif

#ifdef __QNXNTO__
#include <time.h>
#include <unistd.h>
#else
#include <ctime>
#endif

#include <set>
#include <string>

#if defined(_MSC_VER) || defined(__BORLANDC__)
 typedef unsigned __int64 uint64;
 typedef signed __int64 int64;
#else
 typedef unsigned long long uint64;
 typedef signed long long int64;
#endif


// Create a list of times, this will test weak ordering.
std::set < UF::Time::ISODateTime, UF::Time::ltISODateTime > dbTimes;

using namespace UF;
using namespace UF::Log;

//! Get the current time.
/*!
 * @param LT - local time
 * @param UTC - Coordinated Universal Time
 *
 */
void CurrentTime(UF::Time::ISODateTime &LT, UF::Time::ISODateTime &UTC)
{
  UF::Time::SystemTime *pSysTime = UF::Time::SystemTime::Instance();
  std::string time;
  pSysTime->ISOLocalTime(time);
  LT.SetISODateTime(time);
  UTC = LT;
  UTC.ToUTC();
}

int ISODateTimeMain (int argc, char* argv[])
{
  // Get rid of the argc, argv unreferenced formal parameter warnings.
  int numargs;
  std::string programName;
  numargs = argc;
  if ( argc != 0 )
  {
    programName = argv[0];
  }

  std::string sep = "------------------------------------------------------------------------";
  // Set up the log file.
  ufLogger *pLog = new ufLogger;
  pLog->open("TestISODateTime.log",std::ios::out);
  std::string time;
  Time::SystemTime *pClock = Time::SystemTime::Instance();
  pClock->ISOLocalTime(time);
  *pLog << sep << "\nStarted:  " << time << std::endl;

  bool fail = false;
  *pLog << "Testing the ISO Date-Time Library" << std::endl;

  UF::Time::ISODateTime LT;
  UF::Time::ISODateTime UTC;
  CurrentTime(LT,UTC);

  // Create a list of times, this will test weak ordering.
  std::set < UF::Time::ISODateTime, UF::Time::ltISODateTime > Times;
  time = "1582-10-4 00:00:00";
  UF::Time::ISODateTime dt(time);
  Times.insert(time);
  time = "2000-01-01 12:00:00";
  dt.SetISODateTime(time);
  Times.insert(time);
  time = "1582-10-15 00:00:00";
  dt.SetISODateTime(time);
  Times.insert(time);

  int ltTest = 0; // 001
  int gtTest = 0; // 110
  int eqTest = 0; // 010
  int leTest = 0; // 110
  int geTest = 0; // 1
  int neTest = 0;

  int shift = 0;

  std::set < UF::Time::ISODateTime, UF::Time::ltISODateTime > ::iterator p;
  
  for ( p = Times.begin(); p != Times.end(); ++p)
  {
    ltTest |= (*p < dt) << shift;     
    gtTest |= (*p > dt) << shift;     
    eqTest |= (*p == dt) << shift;     
    leTest |= (*p <= dt) << shift;     
    geTest |= (*p >= dt) << shift;     
    neTest |= (*p != dt) << shift; 
    shift++;
  }
  if (ltTest != 1 )
  {
    fail |= false;
    *pLog << "Less than tests failed." << std::endl;
  }

  if (gtTest != 4 )
  {
    fail |= false;
    *pLog << "Greater than tests failed.\n" << std::endl;
  }

  if (eqTest != 2 )
  {
    fail |= false;
    *pLog << "Equality tests failed." << std::endl;
  }

  if (leTest != 3 )
  {
    fail |= false;
    *pLog << "Less than or equal to tests failed." << std::endl;
  }

  if (geTest != 6 )
  {
    fail |= false;
    *pLog << "Greater than or equal to tests failed." << std::endl;
  }

  if (neTest != 5 )
  {
    fail |= false;
    *pLog << "Not equal to tests failed." << std::endl;
  }

  if ( fail )
  {
    *pLog << "Comparison results were:" << std::endl;
      for ( p = Times.begin(); p != Times.end(); ++p)
      {
        *pLog << p->GetISODateTime() << " <  " <<  dt.GetISODateTime() << std::boolalpha << " " << (*p < dt) << std::endl;     
        *pLog << p->GetISODateTime() << " >  " <<  dt.GetISODateTime() << std::boolalpha << " " << (*p > dt) << std::endl;     
        *pLog << p->GetISODateTime() << " == " <<  dt.GetISODateTime() << std::boolalpha << " " << (*p == dt) << std::endl;     
        *pLog << p->GetISODateTime() << " <= " <<  dt.GetISODateTime() << std::boolalpha << " " << (*p <= dt) << std::endl;     
        *pLog << p->GetISODateTime() << " >= " <<  dt.GetISODateTime() << std::boolalpha << " " << (*p >= dt) << std::endl;     
        *pLog << p->GetISODateTime() << " != " <<  dt.GetISODateTime() << std::boolalpha << " " << (*p != dt) << std::endl;     
        *pLog << std::endl;
      }
  }

  time = "2000-01-01 23:59:59.999+09:30";
  dt.SetISODateTime(time);
  std::string time1 = dt.GetISODateTime();
  if ( time != time1 )
  {
    fail |= false;
    *pLog << "Time conversion failed." << std::endl;
    *pLog << "Expected: " << time << std::endl;
    *pLog << "Got:      " << time1 << std::endl;
  }

  dt.SetISODateTime("2000-01-01 12:00:00+00:00");
  dt.SetOffset(0);
  double epoch = dt.GetEpoch();
  epoch++;
  epoch--;
  if ( (int64)epoch != (int64)211813488000.0)
  {
    fail |= false;
    *pLog << "Getting the epoch failed." << std::endl;
    *pLog << "Expected: " << 211813488000LL << std::endl;
    *pLog << "Got:      " << std::fixed << (int64)epoch << std::endl;
  }
  epoch--;
  dt.SetEpoch(epoch);
  time = dt.GetISODateTime(0,'-',':',' ',0);
  if ( time != "2000-01-01 11:59:59" )
  {
    fail |= false;
    *pLog << "Getting the epoch failed." << std::endl;
    *pLog << "Expected: " << "2000-01-01 11:59:59" << std::endl;
    *pLog << "Got:      " << time << std::endl;
  }

  pClock->ISOLocalTime(time);
  *pLog << ((fail)?"Failed.":"Passed") << "\n"
      << "Finished: " << time << "\n" << sep << std::endl;


  delete pLog;

  if ( fail )
    return 1;

  return 0;
}
