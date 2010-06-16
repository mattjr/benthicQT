//
// C++ Interface: SaveData
//
// Description:
//
//
// Author: Andrew J. P. Maclean <a.maclean@cas.edu.au>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DTM_UFSAVEDATA_H
#define DTM_UFSAVEDATA_H

#include "ufWin32Header.h"
#include "vtkType.h"

#include <iosfwd>
#include <string>

class vtkImageData;

namespace UF {
namespace DTM {

//! Save image data to a file.
/**
Save image data to a file.

@author Andrew J. P. Maclean
*/
class UF_DTM_EXPORT SaveData{
public:
    SaveData();

    ~SaveData();

public:
  /**
   * Write out the image data to a file in binary format.
   *UF_DTM_EXPORTS
   * The header contains a block of three integer values corresponding to Dimensions[3],
   * six double precision values corresponding to Origin[3] and Spacing[3],
   * followed by three integer values corresponding to Increment[3],
   *  a single integer corresponding to ScalarType.
   *
   * The scalar type is defined as follows:
   * \code
   * #define VTK_VOID            0
   * #define VTK_BIT             1
   * #define VTK_CHAR            2
   * #define VTK_SIGNED_CHAR    15
   * #define VTK_UNSIGNED_CHAR   3
   * #define VTK_SHORT           4
   * #define VTK_UNSIGNED_SHORT  5
   * #define VTK_INT             6
   * #define VTK_UNSIGNED_INT    7
   * #define VTK_LONG            8
   * #define VTK_UNSIGNED_LONG   9
   * #define VTK_FLOAT          10
   * #define VTK_DOUBLE         11
   * #define VTK_ID_TYPE        12
   * \endcode
   *
   * This header is followed by all the scalars.
   *
   * @param ImageData The data to be written to the file.
   * @param FileName  The name of the file.
   */
  void WriteImageData ( vtkImageData * ImageData, std::string const & FileName );

  /**
   * Write out the image data to a file in binary or text format. The file will
   * consist of particles with the value (x,y,z). The scalar from the image
   * file becomes the z-value. If the file is written as a binary file the type
   * (float double etc.) is determined from the type of the scalars.
   *
   * NOTE: Only the image data corresponding to (i,j,0) is written.
   *
   * @param ImageData The data to be written to the file.
   * @param FileName  The name of the file.
   * @param ScalarRange  The (inclusive) range of scalars to write out.
   * @param WriteBinary  If true, a binary file is written, otherwise an ASCII file is written.
   */
  void WriteImageDataAsParticles ( vtkImageData * ImageData, std::string const & FileName, double ScalarRange[2], bool WriteBinary = true );

private:
  /**
   * Write the header information to the file.
   * The header contains a block of three integer values corresponding to Dimensions[3],
   * six double precision values corresponding to Origin[3] and Spacing[3],
   * followed by three integer values corresponding to Increment[3],
   *  a single integer corresponding to ScalarType and then all the scalars.
   *
   * The scalar type is defined as follows:
   * \code
   * #define VTK_VOID            0
   * #define VTK_BIT             1
   * #define VTK_CHAR            2
   * #define VTK_SIGNED_CHAR    15
   * #define VTK_UNSIGNED_CHAR   3
   * #define VTK_SHORT           4
   * #define VTK_UNSIGNED_SHORT  5
   * #define VTK_INT             6
   * #define VTK_UNSIGNED_INT    7
   * #define VTK_LONG            8
   * #define VTK_UNSIGNED_LONG   9
   * #define VTK_FLOAT          10
   * #define VTK_DOUBLE         11
   * #define VTK_ID_TYPE        12
   * \endcode
   * @param Dimensions[] Dimensions of the data.
   * @param Origin[] Origin of the lower-ledt corner of the data.
   * @param Spacing[] The spacing of the data.
   * @param Increment[] Increments.
   * @param ScalarType This is defined as follows (look in vtkSystemIncludes.h):
   */
  void WriteBinaryHeader ( int Dimensions[3], double Origin[3],
    double Spacing[3], vtkIdType Increment[3], int ScalarType, std::ofstream &ofs );

};

} // Namespace DTM.
} // Namespace UF.

#endif
