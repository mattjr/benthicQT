#include <iostream>
#include <stdexcept>
#include <string>
#include <fstream>
#include <iomanip>
#include <algorithm>

#include "ufFileUtilities.h"

#include "boost/filesystem/fstream.hpp"
#include "boost/filesystem/operations.hpp"

#include "ufLogger.h"
#include "ufSystemTime.h"

using namespace UF;
using namespace UF::FileSystemUtilities;
using namespace UF::Log;

namespace fs = boost::filesystem;

int FileUtilitiesMain ( int argc, char* argv[] )
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
  pLog->open("TestFileUtilities.log",std::ios::out);
  std::string time;
  Time::SystemTime *pClock = Time::SystemTime::Instance();
  pClock->ISOLocalTime(time);
  *pLog << sep << "\nStarted:  " << time << std::endl;

  bool fail = false;
  *pLog << "Testing the file utilities Library" << std::endl;
  try {
    // Get the program path and name.
    //fs::path fullProgPath(argv[0], fs::native);
    fs::path progPath = fs::initial_path();
    fs::directory_iterator it = fs::directory_iterator(progPath);
    // Get the first file in the current directory.
    while(fs::is_directory(*it))
    {
      it++;
    }
#if BOOST_VERSION >= 103600
    fs::path progName = it->filename();
#else
    fs::path progName = it->leaf();
#endif
    fs::path fullProgPath = *it;
    *pLog << "Path: " << progPath.string() << std::endl;
    *pLog << "Name: " << progName.string() << std::endl;

    fs::path foundFile;
    if ( FileUtilities::Instance()->FindFile( progPath, progName.string(), foundFile) )
    {
      *pLog << "Found: " << fullProgPath.string() << std::endl;
      fail = false;
    }
    else
    {
      *pLog << "Failed to find: " << fullProgPath.string() << std::endl;
      fail = true;
    }

    unsigned long fileCount = 0;
    unsigned long directoryCount = 0;
    unsigned long errorCount = 0;
    std::vector< fs::path > fileList;

    FileUtilities::Instance()->ListAllFiles(progPath, fileList, fileCount, directoryCount, errorCount, true );
    *pLog << "Found " << fileCount << " files in " << directoryCount
          << " directories with " << errorCount << " errors." << std::endl;

    for ( std::vector< fs::path >::iterator p = fileList.begin(); p != fileList.end(); ++p)
     *pLog << p->string() << std::endl;

    if ( !fileList.empty() )
    {
      if (std::find(fileList.begin(),fileList.end(),fullProgPath) != fileList.end() )
      {
        *pLog << "Ok, found: " << fullProgPath.string() << std::endl;
        fail = false;
      }
      else
      {
         *pLog << "Error: " << fullProgPath.string() << " is not in fileList. " << std::endl;
        fail = true;
     }
    }
    else
    {
      *pLog << "Error: We should have found: " << fullProgPath.string() << std::endl;
      fail = true;
    }
  }
  catch ( const std::exception & ex )
  {
    fail = true;
    *pLog << ex.what() << std::endl;
  }

  pClock->ISOLocalTime(time);
  *pLog << ((fail)?"Failed.":"Passed") << "\n"
      << "Finished: " << time << "\n" << sep << std::endl;

  delete pLog;

  if ( fail )
    return 1;

  return 0;
}
