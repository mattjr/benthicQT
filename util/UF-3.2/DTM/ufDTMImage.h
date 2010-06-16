//
// C++ Interface: DTMImage
//
// Description:
//
//
// Author: Andrew J. P. Maclean <a.maclean@cas.edu.au>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DTM_UFDTMDTMIMAGE_H
#define DTM_UFDTMDTMIMAGE_H

#include "ufImageData.h"

#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>

namespace UF {
namespace DTM {

  typedef std::vector < double > ufDTMScalarTypeBase;
  class ufDTMScalarType;

//! Treats a Digital Terrain Map as an image.
/**
Handles digital terrain maps as an image. Before reading or writing
to files make sure the streams are open. The class has functions for
writing out the image as either a text file or as a binary file. Similarly
there are functions for reading the data as either text or binary.
The text IO functions are useful for debugging. Binary is often more compact.

Ultimately the class should be templated.

@author Andrew J. P. Maclean
*/
class UF_DTM_EXPORT DTMImage : public ImageData
{
public:
    DTMImage();

    virtual ~DTMImage();
public:
  // Copy/Assignment.

  /**
   * Copy constructor.
   * @param id
   * @return
   */
  DTMImage(DTMImage const &  id);

  /**
   * Assignment
   * @param id
   * @return
   */
  DTMImage operator = (DTMImage const & id);

  /**
   * Equality
   * @param id
   * @return
   */
  bool operator == (DTMImage const & id) const;

  /**
   * Inequality
   * @param id
   * @return
   */
  bool operator != (DTMImage const & id) const { return !(*this == id); }

public:
   /**
    * Return a scalar value corresponding to a point (x,y,z).
    * The value returned is the average over the voxel containing the point.
    * An exception is thrown if the point is out of bounds.
    * @param x
    * @param y
    * @param z
    * @return The scalar value.
    */
   double GetScalar ( double const & x, double const & y, double const & z );

   /**
    * Return a scalar value corresponding to a point (x,y,z).
    * The value returned is the average over the voxel containing the point.
    * An exception is thrown if the point is out of bounds.
    * @param xyz[]
    * @return The scalar value.
    */
   double GetScalar ( double const xyz[3] );

   /**
    * Sets the scalar value for the structured point corresponding to the point (x,y,z).
    * @param x
    * @param y
    * @param z
    * @param ScalarValue
    * @return true if set
    */
   void SetScalar ( double const & x, double const & y, double const & z, double const & ScalarValue );

   /**
    * Sets the scalar value for the structured point corresponding to the point (x,y,z).
    * @param xyz[]
    * @param ScalarValue
    * @return true if set.
    */
   void SetScalar ( double const xyz[3], double const & ScalarValue );

    /**
    * Get a pointer to the scalars.
    * @return The pointer to the scalars.
    */
   template < typename T >
     std::vector<T> *GetScalars() { return reinterpret_cast<ufDTMScalarTypeBase *>(this->scalars); }

public:
  // Reading/Writing

  /**
   * Reads image data from a text file.
   * @param ifs
   */
  void ReadTextImageData(std::ifstream & ifs);

  /**
  * Read the header from the binary data file.
  * @param ifs The input file stream.
  */
 void ReadBinaryHeader(std::ifstream & ifs);

  /**
   * Read the scalars from a binary data file.
   * If the member variable startOfScalars is zero then
   * ReadHeader() will be called first.
   */
  template <typename T>
    void ReadBinaryScalars(std::ifstream &ifs)
    {
      if ( this->startOfScalars == 0 )
         ReadBinaryHeader(ifs);
      ifs.seekg(this->startOfScalars,std::ios::beg);
      ufDTMScalarTypeBase *pScalars = this->GetScalars<T>();
      pScalars->clear();
      while( !ifs.eof() )
        {
        T tmp;
        ifs.read(reinterpret_cast<char *>(&tmp), sizeof(T));
        if ( !ifs.eof() )
          pScalars->push_back(tmp);
        }
    }

  /**
   * Write out the header information as text.
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
   * @param ofs The output file stream.
   * @param precision The precision for the display.
   */
  void WriteTextHeader(std::ofstream & ofs, int const & precision );

  /**
   * Write out the scalars as text.
   * @param ofs The output file stream.
   * @param precision The precision for the display.
   * @param DisplayI If true, identify the i coordinate.
   * @param DisplayJ If true, identify the j coordinate.
   * @param DisplayK If true, identify the k coordinate.
   */
  void WriteTextScalars(std::ofstream &ofs, int const & precision = 2,
                        bool const &DisplayI = false, bool const &DisplayJ = false, bool const DisplayK = false);


  /**
   * Write out the image data to a file in binary format.
   *
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
   * @param ofs The output file stream.
   */
  void WriteBinaryImageData ( std::ofstream &ofs );

  /**
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
   * @return the type of the scalars.
   */
  int GetScalarType () { return this->scalarType; }

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
   * @param ofs The output file stream.
   */
  void WriteBinaryHeader ( std::ofstream &ofs );

protected:
  //! Points to the start of the scalars in a binary data file.
  std::size_t startOfScalars;

  //! The type of scalar.
  /*!
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
  */
  int scalarType;
  //! The scalars.
  //std::vector<double> scalars;
  ufDTMScalarType *scalars;

};


} // Namespace DTM.
} // Namespace UF.

#endif
