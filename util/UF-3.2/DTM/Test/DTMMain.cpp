#include <iostream>
#include <stdexcept>
#include <string>
#include <fstream>
#include <iomanip>

#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>

#include <vtkParticleReader.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkAppendPolyData.h>
#include <vtkImageData.h>

#include "ufObservers.h"
#include "ufVisualiser.h"
#include "ufProbeInput.h"
#include "ufProbeSource.h"
#include "ufProbe.h"
#include "ufLookupTables.h"
#include "ufDataCharacteristics.h"
#include "ufSaveData.h"
#include "ufPlaneTo3d.h"

#include "ufDTMImage.h"
//#include "ufDTMVolume.h"
#include "ufDTMVolumeView.h"

#include <vtkPolyData.h>
#include <vtkXMLPolyDataWriter.h>

#include "ufLogger.h"
#include "ufSystemTime.h"

using namespace UF::DTM;
using namespace UF::Time;
using namespace UF::Log;

// See the examples directory for a proper test.

int DTMMain (int argc, char* argv[])
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
  pLog->open("TestDTMLib.log",std::ios::out);
  std::string time;
  SystemTime *pClock = SystemTime::Instance();
  pClock->ISOLocalTime(time);
  *pLog << sep << "\nStarted:  " << time << std::endl;

  bool fail = false;

  *pLog << "Testing the DTM Library" << std::endl;

  pClock->ISOLocalTime(time);
  *pLog << ((fail)?"Failed.":"Passed") << "\n"
      << "Finished: " << time << "\n" << sep << std::endl;

  delete pLog;

  if ( fail )
    return 1;

  return 0;
}
