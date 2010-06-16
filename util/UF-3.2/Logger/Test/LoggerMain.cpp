#include "ufLogger.h"
#include "ufSystemTime.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <iterator>
#include <cmath>

using namespace UF;
using namespace UF::Log;

int LoggerMain (int argc, char* argv[])
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


  pLog->open("TestLogger.log",std::ios::out);
  std::string time;
  Time::SystemTime *pClock = Time::SystemTime::Instance();
  pClock->ISOLocalTime(time);
  *pLog << sep << "\nStarted:  " << time << std::endl;

  bool fail = false;
  *pLog << "Testing the Logger Library" << std::endl;

  pLog->open("TestAppend.log",std::ios::out);
  *pLog << sep << std::endl;
  for ( int i = 2; i < 10; ++i )
  {
    double x = std::sqrt((double)i);
    bool perfecetSq = (i>1)?int(x)*int(x) == i:false;
    *pLog << "i: " << i << " sqrt("<< i << "): "
      << std::fixed << std::setprecision(4) << x
      << " Perfect square: " << std::boolalpha << perfecetSq
      << "\n";
  }
  *pLog << sep << std::endl;

  pLog->open("Test.log",std::ios::out);
  *pLog << sep << "\n";
  *pLog << "This file is not in append mode.\n";
  *pLog << sep << std::endl;

  // Reopen the first file.
  pLog->open("TestAppend.log");
  *pLog << "Added another line.\n";
  *pLog << sep << std::endl;
  *pLog << "Here we have appended another line." << std::endl;

  // Reopen the second file.
  // Note: It must be in append mode if we want to add more lines.
  pLog->open("Test.log");
  *pLog << "Added another line.\n";
  *pLog << sep << std::endl;

  // Reopen the second file again and add some binary data.
  // Note: It must be opened in binary and append mode.
  pLog->open("Test.log",std::ios::out|std::ios::app|std::ios::binary);
  *pLog << "Added binary data.\n";
  int count = 0;
  for ( int i = 0; i < 5; ++i )
  {
    double k = i;
    count += sizeof(k);
    pLog->write((char*)&k,sizeof(k));
    *pLog << std::flush;
  }
  *pLog << std::endl << sep << std::endl;
  //std::cout << "Wrote " << count << " bytes" << std::endl;

  // Let's open the first file as a source and write out its contents.
/*
  pLog->open("TestAppend.log", std::ios::in|std::ios::binary);
  while (!pLog->eof())
  {
    std::string s;
    std::getline(*pLog,s);
    std::cout << s << std::endl;
  }
*/

  pLog->open("TestLogger.log",std::ios::out|std::ios::app);

  pClock->ISOLocalTime(time);
  *pLog << ((fail)?"Failed.":"Passed") << "\n"
      << "Finished: " << time << "\n" << sep << std::endl;

  delete pLog;

  if ( fail )
    return 1;

  return 0;
}
