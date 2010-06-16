#if !defined(MEAN_SD_H)
#define MEAN_SD_H

#include <cmath>
#include <stdexcept>

namespace UF {
//! An interface for calculating statistics.
namespace Statistics {

  template <typename T>
    class StatisticsTraits;

  //! Calculate the Mean and Standard Deviation for a set of observations.
  /*!
  CLASS MeanSD

    Calculate the Mean, Standard Deviation and some other simple statistics
    for a set of observations. This is an incremental algorithm, thus it generates
    statistics by updating a set of internal variables based on the current
    observation. On other words there is no need to maintain a vector of observations
    because the mean, standard deviation etc. are calculated incrementally. Just
    pass the current observation to the instance of the class to update the statistical
    parameters.

    The returned values have the same type as the trait, which is double by default.
    Internally the type of the maintained variables is the same as the trait. Thus for
    the best precision the trait has been set to long double as the default.

    In general the traits will ensure that the underlying calculations
    are reasonably accurate. The user can override the default trait
    selected on the user defined type by doing the following:
    \code
      Statistics::MeanSD<char, Statistics::StatisticsTraits<float> > Stats;
    \endcode
    If traits other than float, double, long double are used:
    - Be aware that the compiler may generate possible loss of data warnings.
    - In the StandardDeviation() function the argument to the sqrt() function
      if it is of integral type is converted to long double before taking the square root.
      If this explicit conversion is omitted, Visual C++ 7.1 flags it as an error
      because it cannot resolve the template argument, however gcc-3.3.3 silently converts
      the argument to double.

    Review any warnings to ensure that they are acceptable.

    Always ensure that the trait type is either equal to or (preferably) of
    a greater precision than the type you are doing the calculation on. In
    this way the precision of the answer should be maintained.

  USAGE
    Declare it:
    \code
      Statistics::MeanSD<double> Stats;
    \endcode
    Initialise it and add observations.
    \code
      Stats.Initialise(7);
      Stats.Add(9);
      Stats.Add(8);
      Stats.Add(8);
      Stats.Add(7);
      Stats.Add(9);
    \endcode
    The results should be:
    n: 6, Mean: 8, SSD: 4, Variance: 0.8 Standard Deviation: 0.894427

  \version 1.0
  first version

  \date 23-June-2003

  \author Andrew Maclean

  \par license
  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  YOU, as the USER, take SOLE RESPONSIBILITY for, and are responsible for
  determining the FITNESS and APPLICABILITY of this software for a particular
  purpose. The author or any other person or entity are not liable in any way
  for the use or misuse of any program or software using this software.

  \todo
  In the calculation of the standard deviation, the compilers, at present,
  cannot seem to resolve the trait. Accordingly, the variance is promoted to
  long double before calculating the square root.

  \bug

  */
  template < typename T, typename AT = StatisticsTraits<double> > class MeanSD
  {
  public:
    MeanSD(void):
    initialised_(false)
    {
      n_ = AT::minusOne();
      m_ = AT::minusOne();
      SSD_ = AT::minusOne();
      d_ = AT::minusOne();
      minimum_ = AT::minusOne();
      maximum_ = AT::minusOne();
    };
    virtual ~MeanSD(void){};

    //! Initialise the data array with the first value.
    void Initialise( T x )
    {
      n_ = AT::one();
      m_ = x;
      SSD_ = AT::zero();
      d_ = AT::zero();
      initialised_ = true;
      minimum_ = x;
      maximum_ = x;
    }

    //! Add the next x value. It will automatically call Initialise() if Reset() has been executed or initialised_ is false.
    void Add( T x )
    {
      // Check
      if ( !initialised_ )
      {
        Initialise( x );
        return;
      }

      d_ = (x - m_)/(n_+AT::one());
      SSD_ += n_*d_*d_;
      m_ += d_;
      n_++;
      d_ = x - m_;
      SSD_ += d_ * d_;
      if ( x < minimum_ ) minimum_ = x;
      if ( x > maximum_ ) maximum_ = x;

    }

    //! Return the mean.
    typename AT::AccT Mean ( void )
    {
      if ( !initialised_ )
      {
        throw std::domain_error
          ("Statistics::MeanSD::Mean() - there is no data!");
      }
      else return m_;
    }

    //! Return the standard deviation.
    typename AT::AccT  StandardDeviation ( void )
    {
      if ( !initialised_ )
      {
        throw std::domain_error
          ("Statistics::MeanSD::StandardDeviation() - there is no data!");
      }
      else
        if ( n_ < 2*AT::one() )
        {
        throw std::domain_error
          ("Statistics::MeanSD::StandardDeviation() - number of observations < 2");
        }
        else
          return (typename AT::AccT)std::sqrt(((long double)SSD_/(n_-AT::one())));
    }

    //! Return the variance.
    typename AT::AccT Variance ( void )
    {
      if ( !initialised_ )
      {
        throw std::domain_error
          ("Statistics::MeanSD::Variance() - there is no data!");
      }
      else
        if ( n_ < 2*AT::one() )
        {
        throw std::domain_error
          ("Statistics::MeanSD::Variance() - number of observations < 2");
        }
        else return (SSD_/(n_-AT::one()));
    }

    //! Return the sum squared deviations.
    typename AT::AccT SumSquaredDeviations ( void )
    {
      if ( !initialised_ )
      {
        throw std::domain_error
          ("Statistics::MeanSD::SumSquaredDeviations() - there is no data!");
      }
      else return SSD_;
    }

    //! Return the number of observations.
    typename AT::AccT NumberOfObservations ( void )
    {
      if ( !initialised_ )
      {
        throw std::domain_error
          ("Statistics::MeanSD::NumberOfObservations() - there is no data!");
      }
      else return n_;
    }

    //! Return the minimum.
    typename AT::AccT Minimum ( void )
    {
      if ( !initialised_ )
      {
        throw std::domain_error
          ("Statistics::MeanSD::Minimum() - there is no data!");
      }
      else return minimum_;
    }

    //! Return the maximum.
    typename AT::AccT Maximum ( void )
    {
      if ( !initialised_ )
      {
        throw std::domain_error
          ("Statistics::MeanSD::Maximum() - there is no data!");
      }
      else return maximum_;
    }

    //! Reset.
    void Reset ( void )
    {
      initialised_ = false;
    }

  private:
    typename AT::AccT n_; //! Number of observations
    typename AT::AccT m_; //! Mean
    typename AT::AccT SSD_; //! sum squared deviations
    typename AT::AccT d_; //! The amount that the mean will change with the new data point.
    typename AT::AccT minimum_; //! Mimumum
    typename AT::AccT maximum_; //! Maximum

    bool initialised_;
  };


  //! A trait class used to specify the data type for the varables in the class MeanSD.
  /*!
  CLASS StatisticsTraits

    Provides a mechanism to specify the type of the varables used to
    accululate results in the class MeanSD based upon the type of the
    variable for which the statistics are to be calculated. The intent
    of this trait class is to ensure that the best possible accuracy is
    selected for the calculations.

  \version 1.0
  first version

  \date 23-June-2003

  \author Andrew Maclean


  \par license
  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  YOU, as the USER, take SOLE RESPONSIBILITY for, and are responsible for
  determining the FITNESS and APPLICABILITY of this software for a particular
  purpose. The author or any other person or entity are not liable in any way
  for the use or misuse of any program or software using this software.

  \todo

  \bug

  */
  template<typename T>
    class StatisticsTraits
    {
    public:
      //! The typedef of the trait.
      typedef T AccT;
      //! Define zero based on the type.
      static AccT zero() { return 0; };
      //! Define 1 based on the type.
      static AccT one() { return 1; };
      //! Define -1 based on the type.
      static AccT minusOne() { return -1; };
    };

} // End of namespace Stats
} // Namespace UF.

#endif // MEAN_SD_H
