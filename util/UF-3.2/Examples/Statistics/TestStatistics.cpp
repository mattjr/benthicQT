
#include "ufMeanSD.h"
#include <iostream>
#include <iomanip>
#include <sstream>

#include "ufSystemTime.h"
#include "ufLogger.h"

using namespace UF;
using namespace UF::Log;

int main(int /*argc*/, char** /*argv[]*/)
{
  std::string sep = "------------------------------------------------------------------------";
  // Set up the log file.
  ufLogger *pLog = new ufLogger;
  pLog->open("EgStatistics.log",std::ios::out);
  std::string time;
  Time::SystemTime *pClock = Time::SystemTime::Instance();
  pClock->ISOLocalTime(time);
  *pLog << sep << "\nStarted:  " << time << std::endl;

  bool fail = false;
  *pLog << "Testing the Statistics Library" << std::endl;

  Statistics::MeanSD<double> Stats;

  *pLog << "\nTesting simple statistics.\nTesting doubles... " << std::endl;
  *pLog << "\nData is: 7 9 8 8 7 9\n" << std::endl;

  Stats.Initialise(7);
  Stats.Add(9);
  Stats.Add(8);
  Stats.Add(8);
  Stats.Add(7);
  Stats.Add(9);
  *pLog << "n: " << Stats.NumberOfObservations()
    << " Mean: " << Stats.Mean()
    << " SSD: " << Stats.SumSquaredDeviations()
    << " Variance: " << Stats.Variance()
    << " SD: " << Stats.StandardDeviation()
    << " Minimum: " << Stats.Minimum()
    << " Maximum: " << Stats.Maximum()
    << std::endl;

  *pLog << "\nData is: 1.0000 1.1111 1.2222\n" << std::endl;
  *pLog << "\nStandard Deviation should be 0.1111:" << std::endl;
  Stats.Reset();
  Stats.Add(1.0000);Stats.Add(1.1111);Stats.Add(1.2222);
  *pLog << "n: " << Stats.NumberOfObservations()
    << " Mean: " << Stats.Mean()
    << " SSD: " << Stats.SumSquaredDeviations()
    << " Variance: " << Stats.Variance()
    << " SD: " << Stats.StandardDeviation()
    << " Minimum: " << Stats.Minimum()
    << " Maximum: " << Stats.Maximum()
    << std::endl;
  *pLog << "\nData is: 123456789.0000 123456789.1111 123456789.2222\n" << std::endl;
  *pLog << "\nStandard Deviation should be 0.1111:" << std::endl;
  Stats.Reset();
  Stats.Add(123456789.0000);Stats.Add(123456789.1111);Stats.Add(123456789.2222);
  *pLog << "n: " << Stats.NumberOfObservations()
    << " Mean: " << std::setprecision(15) << Stats.Mean() << std::setprecision(6)
    << " SSD: " << Stats.SumSquaredDeviations()
    << " Variance: " << Stats.Variance()
    << " SD: " << Stats.StandardDeviation()
    << " Minimum: " << std::setprecision(15) << Stats.Minimum()
    << " Maximum: " << std::setprecision(15) << Stats.Maximum()
    << std::endl;

  Statistics::MeanSD<int> IStats;

  *pLog << "\nTesting ints ... " << std::endl;
  *pLog << "\nData is: 7 9 8 8 7 9\n" << std::endl;
  IStats.Initialise(7);
  IStats.Add(9);
  IStats.Add(8);
  IStats.Add(8);
  IStats.Add(7);
  IStats.Add(9);
  *pLog << "n: " << IStats.NumberOfObservations()
    << " Mean: " << IStats.Mean()
    << " SSD: " << IStats.SumSquaredDeviations()
    << " Variance: " << IStats.Variance()
    << " SD: " << IStats.StandardDeviation()
    << " Minimum: " << IStats.Minimum()
    << " Maximum: " << IStats.Maximum()
    << std::endl;

  // Result will be OK.
  *pLog << "\nTesting char with a float trait ... " << std::endl;
  *pLog << "\nData is: A C B B A C\n" << std::endl;
  Statistics::MeanSD< char, Statistics::StatisticsTraits<float> > CStats;
  CStats.Initialise('A');
  CStats.Add('C');
  CStats.Add('B');
  CStats.Add('B');
  CStats.Add('A');
  CStats.Add('C');
  *pLog << "n: " << CStats.NumberOfObservations()
    << " Mean: " << (char)CStats.Mean()
    << " SSD: " << CStats.SumSquaredDeviations()
    << " Variance: " << CStats.Variance()
    << " SD: " << CStats.StandardDeviation()
    << " Minimum: " << (char)CStats.Minimum()
    << " Maximum: " << (char)CStats.Maximum()
    << std::endl;

  *pLog << "\nTesting float with a float trait ... " << std::endl;
  *pLog << "\nTesting empty instance ... " << std::endl;
  Statistics::MeanSD<float,Statistics::StatisticsTraits<float> > FStats;
  FStats.Reset();
  try
  {
  *pLog << "n: " << FStats.NumberOfObservations()
    << " Mean: " << FStats.Mean()
    << " SSD: " << FStats.SumSquaredDeviations()
    << " Variance: " << FStats.Variance()
    << " SD: " << FStats.StandardDeviation()
    << " Minimum: " << FStats.Minimum()
    << " Maximum: " << FStats.Maximum()
    << std::endl;
  }
  catch (std::exception &e)
  {
    *pLog << "Expected a fail. The message is: \n" << e.what() << std::endl;
  }

  *pLog << "\nTesting a single observation ... " << std::endl;
  *pLog << "\nData is: -2.3\n" << std::endl;
  FStats.Add(-2.3f);
  *pLog << "n: " << FStats.NumberOfObservations()
    << " Mean: " << FStats.Mean()
    << " SSD: " << FStats.SumSquaredDeviations()
    //<< " Variance: " << FStats.Variance()
    //<< " SD: " << FStats.StandardDeviation()
    << " Minimum: " << FStats.Minimum()
    << " Maximum: " << FStats.Maximum()
    << std::endl;
  try
  {
    *pLog << "\nVariance and standard deviation of a single observation should fail ... " << std::endl;
    *pLog << "n: " << FStats.NumberOfObservations()
      << " Mean: " << FStats.Mean()
      << " SSD: " << FStats.SumSquaredDeviations()
      << " Variance: " << FStats.Variance()
      << " SD: " << FStats.StandardDeviation()
      << " Minimum: " << FStats.Minimum()
      << " Maximum: " << FStats.Maximum()
      << std::endl;
  }
  catch (std::exception &e)
  {
    *pLog << "Expected a fail. The message is: \n" << e.what() << std::endl;
  }
/*
  *pLog << "\nPress any key to continue... ";
  while ( !std::cin.get() );
*/


  pClock->ISOLocalTime(time);
  *pLog << ((fail)?"Failed.":"Passed") << "\n"
      << "Finished: " << time << "\n" << sep << std::endl;

  delete pLog;

  if ( fail )
    return 1;

  return 0;
}

