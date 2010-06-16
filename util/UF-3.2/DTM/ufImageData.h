//
// C++ Interface: ImageData
//
// Description:
//
//
// Author: Andrew J. P. Maclean <a.maclean@cas.edu.au>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DTM_UFIMAGEDATA_H
#define DTM_UFIMAGEDATA_H

#include "ufWin32Header.h"

#include <vector>

namespace UF{
//! Contains classes whose functions exploit the regular geometry and topology of data.
namespace DTM {

//! Routines to assist in the calculation of a regular grid of points.
/**
This is a virtual base class containing all the routines to exploit the regular geometry and topology of a
data structure consisting of a regular grid of points.

@author Andrew J. P. Maclean
*/
class UF_DTM_EXPORT ImageData{
public:
  ImageData();

  virtual ~ImageData();

public:
  // Copy/Assignment.

  /**
   * Copy constructor.
   * @param id
   * @return
   */
  ImageData(ImageData const &  id);

  /**
   * Assignment
   * @param id
   * @return
   */
  ImageData operator = (ImageData const & id);

  /**
   * Equality
   * @param id
   * @return
   */
  bool operator == (ImageData const & id) const;

  /**
   * Inequality
   * @param id
   * @return
   */
  bool operator != (ImageData const & id) const { return !(*this == id); }

public:
  /**
   * Set the dimensions.
   * @param Dimensions[]
   */
  void SetDimensions ( int const Dimensions[3] );

  /**
   * Get the dimensions.
   * @param Dimensions[]
   */
  void GetDimensions ( int Dimensions[3] );

  /**
   * Set the origin.
   * @param Origin[]
   */
  void SetOrigin ( double const Origin[3] );

  /**
   * Get the origin.
   * @param Origin[]
   */
  void GetOrigin ( double Origin[3] );

  /**
   * Set the spacing.
   * @param Spacing[]
   */
  void SetSpacing ( double const Spacing[3] );

  /**
   * Get the spacing.
   * @param Spacing[]
   */
  void GetSpacing ( double Spacing[3] );

  /**
   * Set the index increment.
   * Where:
   * - IndexIncrement[0] = dimension[0],
   * - IndexIncrement[1] = dimension[0]*dimension[1],
   * - IndexIncrement[2] = dimension[0]*dimension[1]*dimension[2],
   * @param IndexIncrement[]
   */
  void SetIndexIncrement ( int const IndexIncrement[3] );

  /**
   * Get the index increment.
   * Where:
   * - IndexIncrement[0] = dimension[0],
   * - IndexIncrement[1] = dimension[0]*dimension[1],
   * - IndexIncrement[2] = dimension[0]*dimension[1]*dimension[2],
   * @param IndexIncrement[]
   */
  void GetIndexIncrement ( int IndexIncrement[3] );

  // Toploogy
  /**
   * Given the indices of a point, calculate the point id.
   * You should check that the id lies in the dataset.
   * @param i - i coordinate of the point.
   * @param j - j coordinate of the point.
   * @param k - k coordinate of the point.
   * @param id The id of the point.
   */
  void PointId(int const & i, int const & j, int const & k, int & id ) const;

  /**
   * Given the indices of a point, calculate the point id.
   * You should check that the id lies in the dataset.
   * @param ijk[] - coordinate of the point.
   * @param id - id of the point.
   */
  void PointId(int const ijk[3], int & id ) const;

  /**
  * Get the structured coordinates corresponding to a point Id.
  * @param id - id of the point.
  * @param i - i coordinate of the point.
  * @param j - j coordinate of the point.
  * @param k - k coordinate of the point.
  */
  void PointCoord(int const & id, int & i, int & j, int & k) const;

  /**
  * Get the structured coordinates corresponding to a point Id.
  * @param id - id of the point.
  * @param ijk[] - coordinates of the point.
  */
  void PointCoord(int const & id, int ijk[3]) const;

  /**
   * Given the indices of a point, calculate the cell id. Note that there are
   * one fewer cells along each topological axis than points.
   * You should check that the id lies in the dataset.
   * @param i - i coordinate of the point.
   * @param j - j coordinate of the point.
   * @param k - k coordinate of the point.
   * @param id The id of the cell.
   */
  void CellId(int const & i, int const & j, int const & k, int & id) const;

  /**
  * Given the indices of a point, calculate the cell id. Note that there are
  * one fewer cells along each topological axis axis than points.
  * You should check that the id lies in the dataset.
  * @param ijk[] - coordinates of the point.
  * @param id - id of the point.
  */
  void CellId(int const ijk[3], int & id) const;

  /**
  * Get the structured coordinates corresponding to a cell Id.
  * @param id - id of the point.
   * @param i - i coordinate of the point.
   * @param j - j coordinate of the point.
   * @param k - k coordinate of the point.
  */
  void CellCoord(int const & id, int & i, int & j, int & k) const;

  /**
  * Get the structured coordinates corresponding to a cell Id.
  * @param id - id of the point.
  * @param ijk[] - coordinate of the point.
  */
  void CellCoord(int const & id, int ijk[3]) const;

  /**
   * Given the structured coordinates of the lower left point. Return the
   * point Ids of the eight points forming the cell.
   * @param i - i coordinate of the point.
   * @param j - j coordinate of the point.
   * @param k - k coordinate of the point.
   * @param Id[] - id of the point.
   */
  void GetVoxelIds (int const & i, int const & j, int const & k, int Id[8]);

  /**
   * Given the structured coordinates of the lower left point. Return the
   * point Ids of the eight points forming the cell.
   * @param ijk[] - coordinate of the lower left point.
   * @param Id[] - ids of th eeight points that form the cell.
   */
  void GetVoxelIds (int const ijk[3], int Id[8]);

  /**
   * Return the weights corresponding to the vertices of a voxel.
   * @param r - r'th wieght.
   * @param s - s'th weight
   * @param t - t'th weight.
   * @param w[] The interpolating weights.
   */
  void WeightVoxel(double const & r, double const & s, double const & t, double w[8]);

  // Geometry

  /**
   * Given a point p(x,y,z) we find the structured coordinates (i,j,k) of the point
   * and the the parametric coordinates (r,s,t) of the point.
   * These represent the position of the point p inside the cell corresponding to p..
   * You should check that i,j,k lie in the bounds of the dataset and that 0 <= r,s,t < 1.
   * @param x - x coordinate of the point.
   * @param y - y coordinate of the point.
   * @param z - z coordinate of the point.
   * @param i - i coordinate of the point.
   * @param j - j coordinate of the point.
   * @param k - k coordinate of the point.
   * @param r - parametric r coordinate of the point.
   * @param s - parametric s coordinate of the point.
   * @param t - parametric t coordinate of the point.
   */
  void TransformToParametric( double const &x, double const &y, double const &z,
                              int & i, int & j, int & k,
                              double &r, double &s, double &t);
  /**
   * Given a point p(x,y,z) we find the structured coordinates (i,j,k) of the point
   * and the the parametric coordinates (r,s,t) of the point.
   * These represent the position of the point p inside the cell corresponding to p..
   * You should check that i,j,k lie in the bounds of the dataset and that 0 <= r,s,t < 1.
   * @param xyz[] The point.
   * @param ijk[] The structured coordinates.
   * @param rst[] The parametric coordinates.
   */
  void TransformToParametric( double const xyz[3], double ijk[3], double rst[3] );

  // Searching

  /**
   * Given a point p(x,y,z) we find the structured coordinates of the point closest to it (i,j,k).
   * You should check that i,j,k lie in the bounds of the dataset.
   * @param x - x coordinate of the point.
   * @param y - y coordinate of the point.
   * @param z - z coordinate of the point.
   * @param i - i coordinate of the point.
   * @param j - j coordinate of the point.
   * @param k - k coordinate of the point.
   */
  void FindClosestStructuredPoint(double const & x, double const & y, double const & z,
                                  int & i, int & j, int & k);

  /**
   * Given a point p(x,y,z) we find the structured coordinates of the point closest to it (i,j,k).
   * You should check that i,j,k lie in the bounds of the dataset.
   * @param xyz[] - the point's coordinates.
   * @param ijk[] - the structured coordinates.
   */
  void FindClosestStructuredPoint(double const xyz[3], int ijk[3]);

  // Interpolation

  // Checking

  /**
   * Given the coordinates of a structured point check if it lies in the bounds of the dataset.
   * @param i - i coordinate of the point.
   * @param j - j coordinate of the point.
   * @param k - k coordinate of the point.
   * @return true if the point lies within the bounds.
   */
  bool CheckPointBounds(int const & i,int const & j,int const & k) const;

  /**
   * Given the coordinates of a structured point check if it lies in the bounds of the dataset.
   * @param ijk[] - the structured coordinates of the point.
   * @return true if the point lies within the bounds.
   */
  bool CheckPointBounds(int const ijk[3]) const;

  /**
  * Check the parametric parameters for a point.
  * @param r - r parameter of the point.
  * @param s - s parameter of the point.
  * @param t - t parameter of the point.
  * @return true if all the parameters lie in the range 0 ... 1
  */
  bool CheckParametricRange( double const & r, double const & s, double const & t ) const;

  /**
  * Check the parametric parameters for a point.
  * @param rst[] - parametric parameters of the point.
  * @return true if all the parameters lie in the range 0 ... 1
  */
  bool CheckParametricRange( double const rst[] ) const;

  /**
   * Check that the point id lies within the bounds of the dataset.
   * @param id - id of the point.
   * @return true if it lies in the bounds of the dataset.
   */
  bool CheckPointId(int const id) const
  {
    int sz = 1;
    for ( int i = 0; i < 3; ++i )
       sz *= this->dimensions[i];

    return id >= 0 && id < sz;
  }

  /**
   * Check that the cell id lies within the bounds of the dataset.
   * @param id - id of the point.
   * @return true if it lies in the bounds of the dataset.
   */
  bool CheckCellId(int const id) const
  {
    int sz = 1;
    for ( int i = 0; i < 3; ++i )
       sz *= this->dimensions[i]-1;

    return id >= 0 && id < sz;
  }

  /**
  * Get a pointer to the scalars implement this in the derived class. 
  * This cannot be virtual.
  * @return The pointer to the scalars.
  */
  template < typename T >
    std::vector<T> *GetScalars ();// = 0;



private:
  /**
   * Given the structured coordinates corresponding to the lower-left
   * point of a cell, find the remaining points in the cell. Do ghosting
   * if needed.
   * @param i - i coordinate of the point.
   * @param j - j coordinate of the point.
   * @param k - k coordinate of the point.
   * @param Id[] - id of the point.
   */
  void FindPoints( int const & i, int const & j, int const & k, int Id[] );

protected:
  //! Dimensions of the data.
  int dimensions[3];
  //! THe origin.
  double origin[3];
  //! THe spacing.
  double spacing[3];
  //! The products of the dimensions for points.
  /*!
      This is:
      - pointProduct[0] = dimensions[0]
      - pointProduct[1] = dimensions[0]*dimensions[1]
      - pointProduct[2] = dimensions[0]*dimensions[1]*dimensions[2]
   */
  int pointProduct[3];
  //! The products of the dimensions for cells.
  /*!
      This is:
      - cellProduct[0] = (dimensions[0]-1)
      - cellProduct[1] = (dimensions[0]-1)*(dimensions[1]-1)
      - cellProduct[2] = (dimensions[0]-1)*(dimensions[1]-1)*(dimensions[2]-1)
      One or more of these can be zero.
   */
  int cellProduct[3];
  //! Increments.
  int indexIncrement[3];

};


} // Namespace DTM.
} // Namespace UF.

#endif
