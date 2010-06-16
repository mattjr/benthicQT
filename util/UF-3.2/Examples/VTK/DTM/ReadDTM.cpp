#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>


/**
 * Read the header from the binary data file.
 * @param ifs The input file stream.
 * @param Dimensions[] The dimensions.
 * @param Origin[] The origin.
 * @param Spacing[] The spacing.
 * @param Increment[] The increments.
 * @param ScalarType The scalar type.
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
 * @return The position of file pointer. This should be pointing to the beginning of the scalar data.
 */
size_t ReadHeader(std::ifstream & ifs,
   int Dimensions[3], double Origin[3], double Spacing[3],
   int Increment[3], int & ScalarType)
{
  ifs.seekg(0,std::ios::beg);

  for ( int i = 0; i < 3; ++i ) ifs.read(reinterpret_cast<char *>(&Dimensions[i]), sizeof(int));
  for ( int i = 0; i < 3; ++i ) ifs.read(reinterpret_cast<char *>(&Origin[i]), sizeof(double));
  for ( int i = 0; i < 3; ++i ) ifs.read(reinterpret_cast<char *>(&Spacing[i]), sizeof(double));
  for ( int i = 0; i < 3; ++i ) ifs.read(reinterpret_cast<char *>(&Increment[i]), sizeof(int));
  ifs.read(reinterpret_cast<char *>(&ScalarType), sizeof(int));

  return ifs.tellg();
}

  /**
   * Read the scalars from a binary data file.
   * @param ifs The input file stream.
   * @param pos The position of the start of the scalar arrsy in the file.
   * @param Scalars The scalars.
   */
template <typename T>
  void ReadScalars(std::ifstream &ifs, size_t pos, std::vector<T> &Scalars)
{
  ifs.seekg(pos,std::ios::beg);
  Scalars.clear();
  while( !ifs.eof() )
    {
    T tmp;
    ifs.read(reinterpret_cast<char *>(&tmp), sizeof(T));
    if ( !ifs.eof() )
      Scalars.push_back(tmp);
    }
}

/**
 * Write out the header information as text.
 * @param ofs The output file stream.
 * @param Dimensions[] The dimensions of the data.
 * @param Origin[] The origin.
 * @param Spacing[] The spacing.
 * @param Increment[] The increment.
 * @param ScalarType The scalar type.
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
 */
void WriteHeader(std::ofstream & ofs,
   int Dimensions[3], double Origin[3], double Spacing[3],
   int Increment[3], int & ScalarType)
{
//  ofs.seekg(0,std::ios::beg);
  std::ios::fmtflags flags = ofs.setf(std::ios::fmtflags());
  ofs << std::fixed << std::setprecision(2);

  ofs << "Dimensions:  ";
  for ( int i = 0; i < 3; ++i ) ofs << Dimensions[i] << " ";
  ofs << std::endl;
  ofs << "Origin:      ";
  for ( int i = 0; i < 3; ++i ) ofs << Origin[i] << " ";
  ofs << std::endl;
  ofs << "Spacings:    ";
  for ( int i = 0; i < 3; ++i ) ofs << Spacing[i] << " ";
  ofs << std::endl;
  ofs << "Increments:  ";
  for ( int i = 0; i < 3; ++i ) ofs << Increment[i] << " ";
  ofs << std::endl;
  ofs << "Scalar type: " << ScalarType << std::endl;

  ofs << std::setprecision(6) << std::endl;
  std::ios::fmtflags newflags = ofs.setf(std::ios::fmtflags());
  ofs.setf(flags,newflags);
}

  /**
   * Write out the scalars as text.
   * @param ofs The output file stream.
   * @param Scalars The scalars to be written out.
   * @param Dimensions[] The dimensions of the scalars (only needed if NumberRows or
                         NumberColumns are true.
   * @param DisplayI If true, identify the i coordinate.
   * @param DisplayJ If true, identify the j coordinate.
   * @param DisplayK If true, identify the k coordinate.
   */
template <typename T>
  void WriteScalars(std::ofstream &ofs, std::vector<T> & Scalars, int Dimensions[3],
     bool const &DisplayI = false, bool const &DisplayJ = false, bool const DisplayK = false)
{
  std::ios::fmtflags flags = ofs.setf(std::ios::fmtflags());
  ofs << std::fixed << std::setprecision(2);

  int choice = (DisplayK << 2) + (DisplayJ << 1) + DisplayI;
  if ( choice & 4  ) choice = 4;
  if ( choice & 2  ) choice = 2;
  if ( choice & 1  ) choice = 1;

  if ( choice == 0 )
    {
      for ( typename std::vector<T>::iterator p = Scalars.begin(); p != Scalars.end(); ++p )
        ofs << *p << "\n";
      return;
    }
  if ( Scalars.size() != (size_t)(Dimensions[0]*Dimensions[1]*Dimensions[2]) )
    {
    std::cerr << "Dimensions do not match." << std::endl;
    return;
    }

  typename std::vector<T>::iterator p = Scalars.begin();
  for ( int i = 0; i < Dimensions[0]; ++i )
    {
    if ( choice & 1 )
      ofs << "(" << i << ",...,...) ";
    for ( int j = 0; j < Dimensions[1]; ++j )
      {
      if( choice & 2 )
      {
        ofs << "(" << i << "," << j << ",...) ";
      }
      for ( int k = 0; k < Dimensions[2]; ++k )
        {
        if ( choice & 4 )
          ofs << "(" << i << "," << j << "," << k << ") ";
        ofs << *p++ << " ";
        if ( choice & 4 )
          ofs << std::endl;
        }
      if( choice & 2 )
        ofs << std::endl;
      }
    if ( choice & 1 )
    ofs << std::endl;
    }


  ofs << std::setprecision(6) << std::endl;
  std::ios::fmtflags newflags = ofs.setf(std::ios::fmtflags());
  ofs.setf(flags,newflags);
}

/**
 * Read in data produced by DTMConverter and produce file containing
 * the scalars and header information.
*/
int main ( int argc, char* argv[] )
{
  if ( argc != 3 )
    {
    std::cout << "Need input file and output file." << std::endl;
    return 1;
    }

  std::string ifn = argv[1];
  std::string ofn = argv[2];

  std::cerr << "Input file: " << ifn << std::endl;
  std::cerr << "Output file: " << ofn << std::endl;

  if ( ifn == ofn )
    {
    std::cerr << "File names cannot be the same." << std::endl;
    return 1;
    }

  std::ifstream ifs(ifn.c_str(),std::ios::binary);
  if ( !ifs )
    {
    std::cout << "Opening of:" << ifn << " failed." << std::endl;
    return 1;
    }
  std::ofstream ofs(ofn.c_str(),std::ios::binary);
  if ( !ofs )
    {
    std::cout << "Opening of:" << ofn << " failed." << std::endl;
    return 1;
    }

  int dimensions[3];
  double origin[3];
  double spacing[3];
  int increment[3];
  int scalarType;

  size_t pos = ReadHeader(ifs,dimensions,origin,spacing,increment,scalarType);
  std::vector< double > scalars;
  ReadScalars<double>(ifs,pos,scalars);

  WriteHeader(ofs,dimensions,origin,spacing,increment,scalarType);
  switch ( scalarType )
    {
    case 11:
      WriteScalars<double>(ofs,scalars,dimensions,false,false,false);
      break;
    default:
      std::cerr << "Unknown type, cannot write out scalars." << std::endl;
    }

  ifs.close();
  ofs.close();

  return 0;

}

