#ifndef _Logger_h
#define _Logger_h

#include <stdexcept>
#include <string>
#include <fstream>

#include "ufWin32Header.h"

namespace UF {

namespace Log {
//! A convenience class useful for logging data.
/*!
    By default the file will be opened for output and in append mode.
    However you can override this.

    Because it inherits from fstream it can be used as either a sink (normal mode)
    or as a source.

    Remember to either "<< std::flush" or "<< std::endl" at frequent intervals
    to ensure that the data is flushed to the sink.
*/
class UF_LOGGER_EXPORT ufLogger: public std::ofstream
{
  public:
    ufLogger()
    {
    };

    virtual ~ufLogger(void)
    {
    };

  private:
    ufLogger( const ufLogger & ); // Disable the copy constructor.
    ufLogger& operator = ( const ufLogger & ); // Disable self-assignment.

public:
    // Here we put in our specific methods and variables for this class.

    //! Opens a file by default as a sink and in append mode.
    /*!
        If the file is already open, it is closed first.
        If the file exists (and you should test for this) then the
        file can be opened as as source.

        @param fileName - the name of the file.
        @param mode - the mode to open the file in.
    */
    virtual void open( std::string const & fileName, std::ios_base::openmode const & mode = std::ios::out|std::ios::app )
    {
      if ( !this->is_open() )
      {
        this->std::ofstream::open(fileName.c_str(),mode);
      }
      else
      {
        this->close();
        this->std::ofstream::open(fileName.c_str(),mode);
      }
    }


private:
};

} // Namespace Log.
} // Namespace UF.

#endif
