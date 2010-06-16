#include <iostream>

#include "ufStringFunctions.h"

#include "ufSystemTime.h"
#include "ufLogger.h"

using namespace UF;
using namespace UF::Log;
using namespace UF::StrFmt;

int main(void)
{
  std::string sep = "------------------------------------------------------------------------";
  // Set up the log file.
  ufLogger *pLog = new ufLogger;


  pLog->open("EgStringFns.log",std::ios::out);
  std::string time;
  Time::SystemTime *pClock = Time::SystemTime::Instance();
  pClock->ISOLocalTime(time);
  *pLog << sep << "\nStarted:  " << time << std::endl;

  bool fail = false;

  std::string test = " \n\rSome data. \r\n";
  std::string ref = "Some data.";

  ufStringFns* pSFN = ufStringFns::Instance();
  std::string result;
  result = pSFN->Trim(test);
  if ( ref != result )
  {
    *pLog << "Test: " << test << "\nReference: " << ref << "\nResult: " << result << std::endl;
    fail = true;
  }
  test = "cbaxabcbcsbbbccaa";
  ref = "xabcbcs";
  result = pSFN->Trim(test,"abc");
  if ( ref != result )
  {
    *pLog << "Test: " << test << "\nReference: " << ref << "\nResult: " << result << std::endl;
    fail |= true;
  }

  pClock->ISOLocalTime(time);
  *pLog << ((fail)?"Failed.":"Passed") << "\n"
      << "Finished: " << time << "\n" << sep << std::endl;

  delete pLog;

  if ( fail )
    return 1;

  return 0;
}
