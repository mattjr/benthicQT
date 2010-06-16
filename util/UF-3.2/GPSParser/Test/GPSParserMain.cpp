
//#pragma once
#include "ufBOD.h"
#include "ufBWC.h"
#include "ufGGA.h"
#include "ufGLL.h"
#include "ufGSA.h"
#include "ufGSV.h"
#include "ufHCHDG.h"
#include "ufRMC.h"
#include "ufRMB.h"
#include "ufRTE.h"
#include "ufVTG.h"
#include "ufWPL.h"
#include "ufXTE.h"
// Garmin proprietary messages.
#include "ufGRME.h"
#include "ufGRMM.h"
#include "ufGRMZ.h"


#include <iostream>
#include "ufLogger.h"
#include "ufSystemTime.h"

using namespace UF;
using namespace UF::Log;
using namespace UF::Time;

  template < typename T > void TestGPSMsg ( T & gps, std::string const & msg, ufLogger *pLog )
  {

    *pLog << "Parsing               : " << msg;
    std::istringstream s(msg);
    s >> gps;
    NMEA::TAG_NMEA_STATUS status = gps.GetParseStatus();
    if ( status != NMEA::NMEA_OK &&
      status != NMEA::NMEA_NAVIGATION_RECEIVER_WARNING &&
      status != NMEA::NMEA_SIMULATING )
    {
      *pLog << "Parse failed - " << NMEA::ErrStatus(status);
      *pLog << std::endl;
      if ( status == NMEA::NMEA_INVALID_CHECKSUM )
      {
        // Force a parse.
        gps.Parse(true);
      }
    }
    else
      if ( status != NMEA::NMEA_OK )
        *pLog << "Parse OK but - " << NMEA::ErrStatus(status) << std::endl;
    if ( status == NMEA::NMEA_OK ||
      status == NMEA::NMEA_NAVIGATION_RECEIVER_WARNING ||
      status == NMEA::NMEA_SIMULATING )
    {
      *pLog << "Reconstructed sentence: " << gps << std::endl;
    }

  }


int GPSParserMain (int argc, char* argv[])
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
  pLog->open("TestGPSParser.log",std::ios::out);
  std::string time;
  SystemTime *pClock = SystemTime::Instance();
  pClock->ISOLocalTime(time);
  *pLog << sep << "\nStarted:  " << time << std::endl;

  bool fail = false;

  *pLog << "Testing the GPS Parser" << std::endl;

  NMEA::BOD bod;
  NMEA::BWC bwc;
  NMEA::GLL gll;
  NMEA::GGA gga;
  NMEA::GSA gsa;
  NMEA::GSV gsv;
  NMEA::RMC rmc;
  NMEA::RMB rmb;
  NMEA::RTE rte;
  NMEA::VTG vtg;
  NMEA::WPL wpl;
  NMEA::XTE xte;

  NMEA::HCHDG hchdg;

  Garmin::GRME grme;
  Garmin::GRMM grmm;
  Garmin::GRMZ grmz;


  std::string bod_s = "$GPBOD,,T,,M,,*47\r\n";
  std::string bod_s1 = "$GPBOD,42.3,T,29.9,M,BURNS FARM,AJPM*36\r\n";
  std::string bwc_s = "$GPBWC,081837,,,,,,T,,M,,N,*13\r\n";
  std::string bwc_s1 = "$GPBWC,234934,3342.6514,S,15034.2922,E,61.2,T,48.8,M,0.027,N,BURNS FARM,S*06\r\n";
  std::string gll_s = "$GPGLL,3722.36223,N,12159.82741,W,170003.00,A*17\r\n";
  std::string gll_s1 = "$GPGLL,3342.6785,S,15034.2457,E,234924,V,S*58\r\n";
  std::string gga_s1 = "$GPGGA,123456.12,1234.123456,N,12345.123456,W,2,12,12.1,12345.12,M,-12.1,M,123,1234*64\r\n";
  //std::string gga_s1 = "$GPGGA,123456.12,1234.123456,N,12345.123456,W,2,12,12.1,12345.12,M,-12.1,M,,*64\r\n";
  std::string gga_s2 = "$GPGGA,161229.487,3723.2475,N,12158.3416,W,1,07,1.0,9.0,M,,,,0000*18\r\n";
  std::string gga_s3 = "$GPGGA,183805.50,3722.36223,N,12159.82741,W,2,7,2.8,16.12,M,-31.24,M,005,0001*6F\r\n";
  std::string gga_s4 = "$GPGGA,183805.50,3722.36223,N,12159.82741,W,2,7,2.8,16.12,M,-31.24,M,005,0001*6E\r\n";
  std::string gsa_s = "$GPGSA,A,3,31,29,23,26,,21,17,09,08,,03,,01.7,01.0,01.4*0B\r\n";
  std::string gsv_s = "$GPGSV,2,2,12,08,43,294,47,07,19,062,42,05,49,314,49,02,03,120,29*7E\r\n";
  std::string hchdg_s = "$HCHDG,24.4,,,12.4,E*2C\r\n";
  std::string rmb_s = "$GPRMB,A,4.08,L,EGLL,EGLM,5130.02,N,00046.34,W,004.6,213.9,122.9,A*3D\r\n";
  std::string rmb_s1 = "$GPRMB,V,0.01,R,AJPM,BURNS F,3342.6514,S,15034.2922,E,0.027,61.2,10.6,A,S*6C\r\n";
  std::string rmc_s = "$GPRMC,213357.20,A,3722.410857,N,12159.773686,W,000.3,102.4,290498,15.4,W*6F\r\n";
  std::string rmc_s1 = "$GPRMC,234940,V,3342.6525,S,15034.2806,E,10.8,48.2,041003,12.4,E,S*30\r\n";
  std::string rte_s = "$GPRTE,2,1,c,0,PBRCPK,PBRTO,PTELGR,PPLAND,PYAMBU,PPFAIR,PWARRN,PMORTL,PLISMR*73\r\n";
  std::string vtg_s = "$GPVTG,179.00,T,193.00,M,000.11,N,000.20,K*48\r\n";
  std::string wpl_s = "$GPWPL,3342.6829,S,15034.2577,E,AJPM*47\r\n";
  std::string xte_s = "$GPXTE,A,A,4.07,L,N*6D\r\n";
  std::string xte_s1 = "$GPXTE,V,V,0.01,R,N,S*0E\r\n";

  std::string grme_s = "$PGRME,15.0,M,22.5,M,27.0,M*1A\r\n";
  std::string grmm_s = "$PGRMM,WGS 84*06\r\n";
  std::string grmz_s = "$PGRMZ,1076,f,3*2B\r\n";

  *pLog << "Testing each sentence. " << std::endl;
  *pLog << "The original and reconstructed sentences should contain the same information" << std::endl;
  *pLog << " but not necessarily the same checksum because the field widths on the" << std::endl;
  *pLog << " output may be different." << std::endl;
  TestGPSMsg(bod,bod_s,pLog);
  TestGPSMsg(bod,bod_s1,pLog);
  TestGPSMsg(bwc,bwc_s,pLog);
  TestGPSMsg(bwc,bwc_s1,pLog);
  TestGPSMsg(gll,gll_s,pLog);
  TestGPSMsg(gll,gll_s1,pLog);
  TestGPSMsg(gga,gga_s1,pLog);
  TestGPSMsg(gga,gga_s2,pLog);
  TestGPSMsg(gga,gga_s3,pLog);
  *pLog << "We expect an invalid checksum message." << std::endl;
  TestGPSMsg(gga,gga_s4,pLog);
  TestGPSMsg(gsa,gsa_s,pLog);
  TestGPSMsg(gsv,gsv_s,pLog);
  TestGPSMsg(hchdg,hchdg_s,pLog);
  TestGPSMsg(rmb,rmb_s,pLog);
  TestGPSMsg(rmb,rmb_s1,pLog);
  TestGPSMsg(rmc,rmc_s,pLog);
  TestGPSMsg(rmc,rmc_s1,pLog);
  TestGPSMsg(rte,rte_s,pLog);
  *pLog << "Size of the waypoint list in the message: " << (unsigned int)rte.WayPointListSz() << std::endl;
  TestGPSMsg(vtg,vtg_s,pLog);
  TestGPSMsg(wpl,wpl_s,pLog);
  TestGPSMsg(xte,xte_s,pLog);
  TestGPSMsg(xte,xte_s1,pLog);

  TestGPSMsg(grme,grme_s,pLog);
  TestGPSMsg(grmm,grmm_s,pLog);
  TestGPSMsg(grmz,grmz_s,pLog);

  *pLog << std::endl << "Testing assignment/operators." << std::endl;
  *pLog << "Testing assignment: ";
  NMEA::VTG vtg1 = vtg;
  if ( vtg1 == vtg )
    *pLog << " = OK" << std::endl;
  else
    *pLog << " = Failed" << std::endl;

  *pLog << "Testing ==, !=: ";
  if ( vtg == vtg1 )
    *pLog << " == Ok ";
  else
    *pLog << " == failed ";
  if ( vtg != vtg1 )
    *pLog << ", != failed, ";
  else
    *pLog << ", != Ok, ";
  *pLog << std::endl;

  *pLog << "Testing += (you should see \"cat 123456\" on a new line at the end of the VTG sentence:" << std::endl;
  vtg += 'c';
  std::string s = "at ";
  vtg += s;
  vtg += 123456;
  *pLog << vtg.GetSentence() << std::endl;

  *pLog << "Testing copy constructor: ";
  NMEA::GGA gga1(gga);
  if ( gga1 == gga )
    *pLog << "OK." << std::endl;
  else
    *pLog << "failed." << std::endl;

  *pLog << "Testing GetMnemonic() " << std::endl;
  if ( "GPGGA" != NMEA::GetMnemonic(gga_s1) )
    *pLog << "GetMnemonic() failed. expected GPGGA, but got: "
             << NMEA::GetMnemonic(gga_s1);
  else
    *pLog << "Ok" << std::endl;
  *pLog << std::endl;

  *pLog << "Testing FillNMEA() " << std::endl;
  std::string msg;
  *pLog << ErrStatus(FillNMEA(gga_s1,gga,msg)) << std::endl;
  if (msg.empty())
    *pLog << "Ok";
  else
  *pLog << "Fail - " << msg << *pLog;
  *pLog << std::endl;


  pClock->ISOLocalTime(time);
  *pLog << ((fail)?"Failed.":"Passed") << "\n"
      << "Finished: " << time << "\n" << sep << std::endl;

  delete pLog;

  if ( fail )
    return 1;

  return 0;
}

