#include "ufFileUtilities.h"

#include <boost/filesystem.hpp>
#include <boost/version.hpp>
#include <iostream>
#include <iomanip>

// Changes to boost::filesystem
// Rename basic_path::leaf() -> filename, branch_path -> parent_path, replace_leaf -> replace_filename. 
// Add basic_path member functions stem, extension, replace_extension. 

namespace fs = boost::filesystem;

using namespace UF::FileSystemUtilities;

FileUtilities* FileUtilities::pInstance_ = 0;
bool FileUtilities::destroyed_ = false;

void FileUtilities::ListAllFiles ( boost::filesystem::path & inDirectory, std::vector< boost::filesystem::path > &files,
                           unsigned long & fileCount, unsigned long & directoryCount,
                           unsigned long & errorCount, bool recurse )
{
//  files.clear();
//  fileCount = 0;
//  directoryCount = 0;
//  errorCount = 0;

  if ( !fs::exists( inDirectory ) )
  {
    return;
  }
  if ( recurse )
  {
    ++directoryCount;
    if ( fs::is_directory( inDirectory ) )
    {
      fs::recursive_directory_iterator end_iter;
      for ( fs::recursive_directory_iterator dir_itr( inDirectory );
            dir_itr != end_iter;
            ++dir_itr )
      {
        try
        {
          if ( fs::is_directory( *dir_itr ) )
          {
            ++directoryCount;
            // std::cout  << dir_itr.level() << "  "  << *dir_itr  << std::endl;
          }
         else
          {
            files.push_back( *dir_itr );
            ++fileCount;
          }
        }
        catch ( const std::exception & ex )
        {
          ++errorCount;
#if BOOST_VERSION >= 103600
          std::cout << dir_itr->filename() << " " << ex.what() << std::endl;
#else
          std::cout << dir_itr->leaf() << " " << ex.what() << std::endl;
#endif
        }
      }
    }
  }
  else
  {
    if ( fs::is_directory( inDirectory ) )
    {
      ++directoryCount;
      fs::directory_iterator end_iter;
      for ( fs::directory_iterator dir_itr( inDirectory );
            dir_itr != end_iter;
            ++dir_itr )
      {
        try
        {
          if ( fs::is_directory( *dir_itr ) )
          {
            ++directoryCount;
            // std::cout  << dir_itr.level() << "  "  << *dir_itr  << std::endl;
          }
         else
          {
            files.push_back( *dir_itr );
            ++fileCount;
          }
        }
        catch ( const std::exception & ex )
        {
          ++errorCount;
#if BOOST_VERSION >= 103600
          std::cout << dir_itr->filename() << " " << ex.what() << std::endl;
#else
          std::cout << dir_itr->leaf() << " " << ex.what() << std::endl;
#endif
        }
      }
    }  
  }


}

bool FileUtilities::FindFile( const boost::filesystem::path & directoryPath,
                const std::string & fileName,
                boost::filesystem::path & pathFound, bool recurse )
{
  if ( !exists( directoryPath ) ) return false;

  if ( recurse )
  {
    boost::filesystem::recursive_directory_iterator end_itr; // default construction yields past-the-end

    for ( boost::filesystem::recursive_directory_iterator itr( directoryPath );
          itr != end_itr;
          ++itr )
    {
#if BOOST_VERSION >= 103600
      if ( itr->filename() == fileName )
      {
        pathFound = *itr;
        return true;
      }
#else
      if ( itr->leaf() == fileName )
      {
        pathFound = *itr;
        return true;
      }
#endif
    }
  }
  else
  {
    boost::filesystem::directory_iterator end_itr; // default construction yields past-the-end

    for ( boost::filesystem::directory_iterator itr( directoryPath );
          itr != end_itr;
          ++itr )
    {
#if BOOST_VERSION >= 103600
      if ( itr->filename() == fileName )
      {
        pathFound = *itr;
        return true;
      }
#else
      if ( itr->leaf() == fileName )
      {
        pathFound = *itr;
        return true;
      }
#endif
    }
  }

  return false;
}

