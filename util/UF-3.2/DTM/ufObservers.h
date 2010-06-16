//
// C++ Interface: Observers
//
// Description: Some helper classes used to monitor the progress of VTK objects.
//
//
// Author: Andrew J. P. Maclean <a.maclean@cas.edu.au>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DTM_UFOBSERVERSOBSERVERS_H
#define DTM_UFOBSERVERSOBSERVERS_H

#include "ufWin32Header.h"

#include <sstream>

#include <vtkCommand.h>
#include <vtkObject.h>

namespace UF {
// Observers used to monitor the progress of VTK objects.
namespace DTM {

//! Callback for the start of a VTK command.
/**
@class vtkStartCommand
This class provides a callback for the start of a VTK object's execution.
@author Andrew J. P. Maclean
*/
class UF_DTM_EXPORT vtkStartCommand : public vtkCommand
{
public:
  static vtkStartCommand *New() { return new vtkStartCommand; }

  /**
   * This is the method that actually does the work of the callback.
   * @param caller The object invoking the event.
   * @param eventId The the id of the event.
   * @param callData The  data that can be passed into the execute method.
   * @return
   */
  virtual void Execute(vtkObject * caller, unsigned long /*eventId*/, void * /*callData*/)
  { vtkstd::cout << "Starting execution of " << caller->GetClassName() << std::endl; }
};

//! Callback for the end of a VTK command.
/**
@class vtkEndCommand
This class provides a callback for the end of a VTK object's execution.
@author Andrew J. P. Maclean
*/
class UF_DTM_EXPORT vtkEndCommand : public vtkCommand
{
public:
  static vtkEndCommand *New() { return new vtkEndCommand; }

  /**
   * This is the method that actually does the work of the callback.
   * @param caller The object invoking the event.
   * @param eventId The the id of the event.
   * @param callData The  data that can be passed into the execute method.
   * @return
   */
 virtual void Execute(vtkObject * caller, unsigned long /*eventId*/, void * /*callData*/)
 {
    vtkstd::ostringstream os;
    os << "Execution ended for " << caller->GetClassName();
    cout << os.str() << vtkstd::endl;
  }
};

//! Callback for the progress information when a VTK command is executing.
/**
@class vtkProgressCommand
This class provides a callback for the progress of a VTK object's execution.
@author Andrew J. P. Maclean
*/
class UF_DTM_EXPORT vtkProgressCommand : public vtkCommand
{
public:
  static vtkProgressCommand *New() { return new vtkProgressCommand; }

  /**
   * This is the method that actually does the work of the callback.
   * @param caller The object invoking the event.
   * @param eventId The the id of the event.
   * @param callData The  data that can be passed into the execute method.
   * @return
   */
  virtual void Execute(vtkObject * caller, unsigned long /*eventId */, void * callData)
  { double progress = *(static_cast<double*>(callData))*100.0;
         vtkstd::cout << "Progress at " << (int)progress << "% for " << caller->GetClassName() << "." << vtkstd::endl; }
};

/*
Use these something like this:

  vtkSmartPointer<vtkStartCommand> sobserver = vtkSmartPointer<vtkStartCommand>::New();
  vtkSmartPointer<vtkEndCommand> eobserver = vtkSmartPointer<vtkEndCommand>::New();
  vtkSmartPointer<vtkProgressCommand> pobserver = vtkSmartPointer<vtkProgressCommand>::New();

  vtkSmartPointer<vtkParticleReader> pr = vtkSmartPointer<vtkParticleReader>::New();
    pr->SetFileName(ifn.c_str());
    pr->AddObserver( vtkCommand::StartEvent, sobserver );
    pr->AddObserver( vtkCommand::EndEvent, eobserver );
    pr->AddObserver( vtkCommand::ProgressEvent, pobserver );
*/


} // Namespace DTM.
} // Namespace UF.

#endif
