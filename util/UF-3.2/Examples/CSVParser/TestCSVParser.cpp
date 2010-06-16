#include "ufCSVParser.h"
#include "ufLogger.h"
#include "ufSystemTime.h"

#ifdef __QNXNTO__
#include <unistd.h>
#endif

#include <iostream>
#include <iomanip>

using namespace UF;
using namespace UF::CSV;
using namespace UF::Log;

void test ( ufLogger *pLog, std::string const & s, char delimiter = ',', char textdelim = '\"' )
{

  CSVParser parser;
  parser.SetDelimiter(delimiter);
  parser.SetTextDelimiter(textdelim);
  parser.SetSentence(s);
  parser.Parse();
  *pLog << "Number of fields: " << parser.NumberOfDataFields() << std::endl;
  std::vector < std::string > res = parser.GetFields();
  *pLog << "Results:" << std::endl;
  for ( std::vector < std::string >::iterator p = res.begin(); p != res.end(); ++p )
    *pLog << *p << std::endl;

}

int main ( void )
{

  std::string sep = "------------------------------------------------------------------------";
  // Set up the log file.
  ufLogger *pLog = new ufLogger;


  pLog->open("EgCSVParser.log",std::ios::out);
  std::string time;
  Time::SystemTime *pClock = Time::SystemTime::Instance();
  pClock->ISOLocalTime(time);
  *pLog << sep << "\nStarted:  " << time << std::endl;

  bool fail = false;
  *pLog << "Testing the CSVParser Library" << std::endl;


  // Feeling lazy, just try different combinations to see how it handles.
  test(pLog,"12345,5,\"\",6,n\r\n");
  test(pLog,"a+b=3+-99-",'+','-');
  CSVParser parser("123.45,-99,1.0e230");
  for ( int i = 0; i < 5; ++i )
    {
    try
      {
      double d;
      bool res = false;
      res = parser.GetField(i,d);
      *pLog << "Field " << i << ": " << d << std::endl;
      }
    catch (std::exception &e)
      {
        if ( i > 2 )
          *pLog << "This should fail. Reason:" << e.what() << std::endl;
        else
          *pLog << "Should never have failed. Reason:" << e.what() << std::endl;
      }
    }
  *pLog << "Add a few extra fields, using SetSentence()." << std::endl;
  parser.SetSentence("\"some, date\",123,12,2006-01-24,15:12:34.5");
  parser.Parse();
  *pLog << parser.MakeSentence() << std::endl;
  *pLog << "Merging the date and time fields and adding a delimiter:" << std::endl;
  parser.MergeFields(6,7);
  parser.DelimitField(6);
  *pLog << parser.MakeSentence() << std::endl;
  *pLog << "Use Init(), SetSentence(), we get:" << std::endl;
  parser.Init();
  parser.SetSentence("\"some, date\",123,12");
  parser.Parse();
  *pLog << parser.MakeSentence();
  parser.Reverse();
  *pLog << std::endl << "Reversed: " << parser.MakeSentence() << std::endl;
  *pLog << "Done." << std::endl;

  pClock->ISOLocalTime(time);
  *pLog << ((fail)?"Failed.":"Passed") << "\n"
      << "Finished: " << time << "\n" << sep << std::endl;

  delete pLog;

  if ( fail )
    return 1;

  return 0;
}
