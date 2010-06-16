#include <iostream>

#include "ufStringFunctions.h"

#include "ufSystemTime.h"
#include "ufLogger.h"

using namespace UF;
using namespace UF::Log;
using namespace UF::StrFmt;

int StringFns (int argc, char* argv[])
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


  pLog->open("TestStringFns.log",std::ios::out);
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
