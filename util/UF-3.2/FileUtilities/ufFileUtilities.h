#if !defined(FILEUTILITIESFILEUTILITIES_H)
#define FILEUTILITIESFILEUTILITIES_H


#include <stdexcept>
#include "boost/filesystem/operations.hpp" // includes boost/filesystem/path.hpp
#include <vector>

#include "ufWin32Header.h"

namespace UF {
//! Utilities for manipulating the file system.
namespace FileSystemUtilities {

  //! A singleton class holding some useful file utilities.
  class UF_FILE_UTILITIES_EXPORT FileUtilities {

  public:
    //! Unique point of access for the singleton.
    static FileUtilities* Instance()
    {
      if ( !pInstance_ )
      {
        // Check for a dead reference.
        if ( destroyed_ )
          OnDeadReference();
        else // First call initialise.
          Create();
      }
      return pInstance_;
    }

  private:

    //! Create a new singleton and store a pointer to it in pInstance_
    static void Create()
    {
      // Task: Initialise pInstance_
      static FileUtilities theInstance;
      pInstance_ = &theInstance;
    }
    //! Gets called if a dead reference is detected.
    static void OnDeadReference()
    {
      throw std::runtime_error("Dead Reference Error in FileUtilities.");
    }
    FileUtilities( const FileUtilities & ); // Disable the copy constructor.
    FileUtilities& operator = ( const FileUtilities & ); // Disable self-assignment.
    //! Private constructor, disables creation of the object by clients.
    FileUtilities(void) {};
    // Make the destructor private in order to prevent clients
    // holding a pointer from accidentally deleting it.
    virtual ~FileUtilities(void)
    {
      pInstance_ = 0;
      destroyed_ = true;
    };

    //! Pointer to the instance.
    static FileUtilities * pInstance_;
    //! True if destroyed.
    static bool destroyed_;

  public:
    /**
     * List all files in a path and the sub-directories in that path.
     * @param inDirectory - The path where the data files are stored.
     * @param files - The list of files in the path and, optionally, the subdirectories.
     * @param fileCount - The total number of files.
     * @param directoryCount - The total number of directories.
     * @param errorCount - The total number of errors.
     * @param recurse - If true, recurse into the subdirectories.
     */
    void ListAllFiles ( boost::filesystem::path & inDirectory, std::vector< boost::filesystem::path > &files,
                        unsigned long & fileCount, unsigned long & directoryCount,
                        unsigned long & errorCount, bool recurse = true  );

    /**
     * Given a path and a file name, find the full path name for that file.
     * If necessary, recurse into subdirectories in order to find it.
     * @param directoryPath - The directory from where we will start looking for the file.
     * @param fileName - The name of the file we are searching for.
     * @param pathFound - The full path and file name.
     * @param recurse - If true, recurse into the subdirectories to find the file.
     * @return true if the file was found.
     */
    bool FindFile( const boost::filesystem::path & directoryPath,
                   const std::string & fileName,
                   boost::filesystem::path & pathFound, bool recurse = true );
    };
} // Namespace FileSystemUtilities.
} // Namespace UF.

#endif
