//
// C++ Implementation: DTMImage
//
// Description:
//
//
// Author: Andrew J. P. Maclean <a.maclean@cas.edu.au>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "ufDTMImage.h"
#include <exception>
#include <sstream>
#include <locale>
#include <cstring>

class UF::DTM::ufDTMScalarType: public UF::DTM::ufDTMScalarTypeBase
{};


namespace {
/**
Exception class for bounds checking.
@author Andrew J. P. Maclean
*/
class IndexOutOfBounds : public std::exception
{
public:
  IndexOutOfBounds(const char * fnName, int const & i = 0, int const & j = 0, int const & k = 0):
    fnName(fnName)
    ,i(i)
    ,j(j)
    ,k(k)
  {}

  virtual const char * what() const throw()
  {
    std::ostringstream os;
    os << "An index is out of bounds in " << this->fnName << " (i,j,k): (" << this->i << "," << this->j << "," << this->k << ")";
    static char s[255];
    std::memset(s,0,255);
    if ( os.str().size() < 255 )
    {
      for ( int i = 0; i < (int)os.str().size(); ++i )
        s[i] = os.str()[i];
    }
    else
      for ( int i = 0; i < 255; ++i )
        s[i] = os.str()[i];
    return s;
  }

private:
  const char *fnName;
  int i;
  int j;
  int k;
};

/**
Exception class for parametric range checking.
@author Andrew J. P. Maclean
*/
class ParametricRangeOutOfBounds : public std::exception
{
public:
  ParametricRangeOutOfBounds(const char * fnName, double const & r = 0, double const & s = 0, double const & t = 0):
    fnName(fnName)
    , r(r)
    ,s(s)
    ,t(t)
  {}

  virtual const char * what() const throw()
  {
    std::ostringstream os;
    os << "A parametric range is out of bounds in " << this->fnName << " (i,j,k): (" << this->r << "," << this->s << "," << this->t << ")";
    static char s[255];
    std::memset(s,0,255);
    if ( os.str().size() < 255 )
    {
      for ( int i = 0; i < (int)os.str().size(); ++i )
        s[i] = os.str()[i];
    }
    else
      for ( int i = 0; i < 255; ++i )
        s[i] = os.str()[i];
    return s;
  }

private:
  const char * fnName;
  double r;
  double s;
  double t;
};

/**
  * Print the scalars.
  * @param Data The array of scalars.
  * @param ofs The output file stream.
  */
template < typename T >
  void WriteBinaryScalars ( std::vector<T> const & Data, std::ofstream &ofs )
{
  typename std::vector<T>::const_iterator p;
  for ( p = Data.begin(); p != Data.end(); ++p )
    {
    ofs.write((char *) &p , sizeof(T));
    }
  ofs.flush();
}

/**
 * Skip characters until a numeric one is found.
 * @param ifs The input file stream.
 */
void SkipUntilNum (std::ifstream &ifs)
{
  std::locale loc;
  char c = char(ifs.peek());
  while (!(std::isdigit(c,loc)|| c == '+' || c == '-' || c == '.'))
    {
    ifs.get(c);
    c = char(ifs.peek());
    }
}

}


using namespace UF::DTM;

DTMImage::DTMImage()
 : ImageData()
 , startOfScalars(0)
 , scalarType(0)
{
  this->scalars = new ufDTMScalarType;
}

DTMImage::DTMImage( DTMImage const & id )
{
  this->scalars = new ufDTMScalarType;

  for (int i = 0; i < 3; ++i)
    {
    this->dimensions[i] = id.dimensions[i];
    this->origin[i] = id.origin[i];
    this->spacing[i] = id.spacing[i];
    this->pointProduct[i] = id.pointProduct[i];
    this->cellProduct[i] = id.cellProduct[i];
    this->indexIncrement[i] = id.indexIncrement[i];
    }
  this->startOfScalars = id.startOfScalars;
  this->scalarType = id.scalarType;
  *this->scalars = *id.scalars;
}

DTMImage::~DTMImage()
{
  delete this->scalars;
}

DTMImage DTMImage::operator =( DTMImage const & id )
{
  if ( this != &id )
    {
    for (int i = 0; i < 3; ++i)
      {
      this->dimensions[i] = id.dimensions[i];
      this->origin[i] = id.origin[i];
      this->spacing[i] = id.spacing[i];
      this->pointProduct[i] = id.pointProduct[i];
      this->cellProduct[i] = id.cellProduct[i];
      this->indexIncrement[i] = id.indexIncrement[i];
      }
    this->startOfScalars = id.startOfScalars;
    this->scalarType = id.scalarType;
    *this->scalars = *id.scalars;
    }
  return *this;
}

bool DTMImage::operator ==( DTMImage const & id ) const
{
  if ( this != &id )
    return false;

  bool res = false;
  for (int i = 0; i < 3; ++i)
    {
    res |= this->dimensions[i] == id.dimensions[i];
    res |= this->origin[i] == id.origin[i];
    res |= this->spacing[i] == id.spacing[i];
    res |= this->pointProduct[i] == id.pointProduct[i];
    res |= this->cellProduct[i] == id.cellProduct[i];
    res |= this->indexIncrement[i] == id.indexIncrement[i];
    }
  res |= this->startOfScalars == id.startOfScalars;
  res |= this->scalarType == id.scalarType;
  res |= *this->scalars == *id.scalars;

  return res;
}

void DTMImage::ReadBinaryHeader( std::ifstream & ifs )
{
  ifs.seekg(0,std::ios::beg);

  int Dimensions[3];
  for ( int i = 0; i < 3; ++i ) ifs.read(reinterpret_cast<char *>(&Dimensions[i]), sizeof(int));
  this->SetDimensions(Dimensions);
  for ( int i = 0; i < 3; ++i ) ifs.read(reinterpret_cast<char *>(&this->ImageData::origin[i]), sizeof(double));
  for ( int i = 0; i < 3; ++i ) ifs.read(reinterpret_cast<char *>(&this->ImageData::spacing[i]), sizeof(double));
  for ( int i = 0; i < 3; ++i ) ifs.read(reinterpret_cast<char *>(&this->ImageData::indexIncrement[i]), sizeof(int));
  ifs.read(reinterpret_cast<char *>(&this->scalarType), sizeof(int));

  this->startOfScalars = ifs.tellg();
}

void DTMImage::WriteTextHeader( std::ofstream & ofs, int const & precision )
{
  std::ios::fmtflags flags = ofs.setf(std::ios::fmtflags());
  ofs << std::fixed << std::setprecision(precision);

  ofs << "Dimensions:  ";
  for ( int i = 0; i < 3; ++i ) ofs << this->dimensions[i] << " ";
  ofs << std::endl;
  ofs << "Origin:      ";
  for ( int i = 0; i < 3; ++i ) ofs << this->origin[i] << " ";
  ofs << std::endl;
  ofs << "Spacings:    ";
  for ( int i = 0; i < 3; ++i ) ofs << this->spacing[i] << " ";
  ofs << std::endl;
  ofs << "Increments:  ";
  for ( int i = 0; i < 3; ++i ) ofs << this->indexIncrement[i] << " ";
  ofs << std::endl;
  ofs << "Scalar type: " << this->scalarType << std::endl;

  ofs << std::setprecision(6) << std::endl;
  std::ios::fmtflags newflags = ofs.setf(std::ios::fmtflags());
  ofs.setf(flags,newflags);
}

void DTMImage::WriteBinaryImageData( std::ofstream &ofs  )
{

  WriteBinaryHeader(ofs);

  switch(this->scalarType)
    {
    case 11:
      WriteBinaryScalars<double>(*this->GetScalars<double>(),ofs);
    }

  ofs.close();

}

void DTMImage::WriteBinaryHeader( std::ofstream &ofs  )
{
    for ( int i = 0; i < 3; ++i ) ofs.write((char *) &this->dimensions[i], sizeof(int));
    for ( int i = 0; i < 3; ++i ) ofs.write((char *) &this->origin[i], sizeof(double));
    for ( int i = 0; i < 3; ++i ) ofs.write((char *) &this->spacing[i], sizeof(double));
    for ( int i = 0; i < 3; ++i ) ofs.write((char *) &this->indexIncrement[i], sizeof(int));
    ofs.write((char *) &this->scalarType, sizeof(int));
    ofs.flush();
}

void DTMImage::ReadTextImageData( std::ifstream & ifs )
{
  ifs.seekg(0,std::ios::beg);

  SkipUntilNum(ifs);
  int Dimensions[3];
  for ( int i = 0; i < 3; ++i ) ifs >> Dimensions[i];
  this->SetDimensions(Dimensions);
  SkipUntilNum(ifs);
  for ( int i = 0; i < 3; ++i ) ifs >> this->origin[i];
  SkipUntilNum(ifs);
  for ( int i = 0; i < 3; ++i ) ifs >> this->spacing[i];
  SkipUntilNum(ifs);
  for ( int i = 0; i < 3; ++i ) ifs >> this->indexIncrement[i];
  SkipUntilNum(ifs);
  ifs >> this->scalarType;
  // We are assuming that the scalar type is double.
  // When we template the class we will fix all these things.
  this->scalars->resize(this->dimensions[0]*this->dimensions[1]*this->dimensions[2]);
  this->scalars->clear();
  double tmp = 0;
  SkipUntilNum(ifs);
  while( !ifs.eof() )
    {
    ifs >> tmp >> std::skipws;
    if ( !ifs.eof() )
      this->scalars->push_back(tmp);
    }
  if ( this->scalars->size() != (std::size_t)this->pointProduct[2] )
    std::cerr << "The number of scalars read do not match the dimensions.";
}

double DTMImage::GetScalar( double const & x, double const & y, double const & z )
{
  int Id[8], i, j, k;
  double w[8], r, s, t;
  double sc = 0;
  this->TransformToParametric(x,y,z,i,j,k,r,s,t);
//   std::cout << "(x,y,z)" << x << " " << y << " " << z
//             << "(i,j,k)" << i << " " << j << " " << k
//             << "(r,s,t)" << r << " " << s << " " << t << std::endl;
  if ( !this->CheckPointBounds(i,j,k) )
    throw IndexOutOfBounds("GetScalar",i,j,k);
  if ( !this->CheckParametricRange(r,s,t) )
    throw ParametricRangeOutOfBounds("GetScalar",r,s,t);
  this->GetVoxelIds(i,j,k,Id);
  this->WeightVoxel(r,s,t,w);
  for (int i = 0; i < 8; ++i )
    {
    std::cout << " Scalar: " << this->scalars->at(Id[i]) << " w:" << w[i] << " Id:" << Id[i] << std::endl;
    sc += w[i]*this->scalars->at(Id[i]);
    }
  return sc;
}

double DTMImage::GetScalar ( double const xyz[] )
{
  return this->GetScalar(xyz[0],xyz[1],xyz[2]);
}

void DTMImage::SetScalar( double const & x, double const & y, double const & z, double const & ScalarValue )
{
  int i, j, k;
  this->FindClosestStructuredPoint(x,y,z,i,j,k);
  int id;
  this->PointId(i,j,k,id);
  if ( !this->CheckPointId(id) )
    throw IndexOutOfBounds("SetScalar",i,j,k);

  this->scalars->at(id) = ScalarValue;
}

void DTMImage::SetScalar( double const xyz[3], double const & ScalarValue )
{
  return this->SetScalar(xyz[0],xyz[1],xyz[2],ScalarValue);
}

void DTMImage::WriteTextScalars(std::ofstream &ofs, int const & precision,
                        bool const &DisplayI, bool const &DisplayJ, bool const DisplayK)
{
  if ( this->scalars->size() != (std::size_t)(this->pointProduct[2]) )
  {
    std::cerr << "Dimensions do not match." << std::endl;
    std::cerr << "Number of Scalars:" << this->scalars->size() << " expected: " << this->pointProduct[2] << std::endl;
    return;
  }

  std::ios::fmtflags flags = ofs.setf(std::ios::fmtflags());
  ofs << std::fixed << std::setprecision(precision);

  int choice = (DisplayK << 2) + (DisplayJ << 1) + DisplayI;
  if ( choice & 4  ) choice = 4;
  if ( choice & 2  ) choice = 2;
  if ( choice & 1  ) choice = 1;

  if ( choice == 0 )
  {
    for ( ufDTMScalarTypeBase::iterator p = this->scalars->begin(); p != this->scalars->end(); ++p )
      ofs << *p << "\n";
    return;
  }

  ufDTMScalarTypeBase::iterator p = this->scalars->begin();
  int i = 0;
  int j = 0;
  int k = 0;
  for ( i = 0; i < this->dimensions[0]; ++i )
  {
    if ( choice & 1 )
      ofs << "(" << i << ",...,...) ";
    for ( j = 0; j < this->dimensions[1]; ++j )
    {
      if( choice & 2 )
      {
        ofs << "(" << i << "," << j << ",...) ";
      }
      for ( k = 0; k < this->dimensions[2]; ++k )
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

