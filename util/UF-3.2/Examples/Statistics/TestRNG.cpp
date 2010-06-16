#include <iostream>
#include <iomanip>

#include "boost/random.hpp"
#include "ufMeanSD.h"

///////////////////////////////////////////////////////////////////////////////
//! Generate a normal distribution.
/*!
    This class is used to generate a sequence of normally distributed numbers.
    At present it is hardwired to use boost::lagged_fibonacci19937 as the
    pseudo-random number generator.

    The class could be rewritten and templated to make it more general, thereby
    removing the dependence on a particular random number generator and distribution.

    However I am lazy.

    @author    Andrew Maclean
    @date    24-10-2006
    @version  1.0
*/
class Distribution
{
public:

  //! Construct a distribution with the default mean = 0, standard deviation = 1.0.
  Distribution()
  {
     this->distribution = new  boost::normal_distribution<double>;
     this->engine = new boost::lagged_fibonacci19937();
     this->rollItBaby = new boost::variate_generator<boost::lagged_fibonacci19937&, boost::normal_distribution<double> > ((*this->engine),*this->distribution);
  }

  //! Construct a distribution with a user defined mean, standard deviation.
  Distribution(double const & mean, double const & stdDev)
  {
    this->distribution = new  boost::normal_distribution<double>(mean,stdDev);
    this->engine = new boost::lagged_fibonacci19937();
    this->rollItBaby = new boost::variate_generator<boost::lagged_fibonacci19937&, boost::normal_distribution<double> > ((*this->engine),*this->distribution);
  }

  //! Construct a distribution with a user defined mean, standard deviation and seed.
  Distribution(double const & mean, double const & stdDev, unsigned int const & seed)
  {
    this->distribution = new  boost::normal_distribution<double>(mean,stdDev);
    this->engine = new boost::lagged_fibonacci19937((boost::uint32_t)seed);
    this->rollItBaby = new boost::variate_generator<boost::lagged_fibonacci19937&, boost::normal_distribution<double> > ((*this->engine),*this->distribution);
  }

  //! Destructor
  ~Distribution()
  {
    delete this->distribution;
    delete this->engine;
  }

  //! Return a normally distributed random number.
  double operator ()()
  {
    return this->rollItBaby->operator ()();
  }

private:
  //! The random number generator.
  boost::lagged_fibonacci19937 *engine;
  //! The distribution.
  boost::normal_distribution<double> *distribution;
  // Glue the distribution and prng together.
  boost::variate_generator<boost::lagged_fibonacci19937&, boost::normal_distribution<double> > *rollItBaby;
};


//! Produce normally distributed random numbers.
/*!
    @param mean - the mean of the distribution.
    @param stdDev - the standard deviation of the distribution.
    @return a random number from the distribution.
*/
template < typename T >
T randomSampler1( T mean, T stdDev )
{
  //boost::normal_distribution<T> norm_dist(mean, stdDev);
  // we can use other engines instead of this one,
  // e,g, boost::mt19937 which produces
  // randomness out of thin air.
  //static boost::lagged_fibonacci19937 engine;
  //const T value = norm_dist.operator() <boost::lagged_fibonacci19937>((engine));
  //return value;
  boost::normal_distribution<T> norm_dist(mean, stdDev); // The distribution.
  //static boost::lagged_fibonacci19937 engine; // The engine (pseudo-random number generator (prng)).
  static boost::mt19937 engine; // The engine (pseudo-random number generator (prng)).
  // Glue the distribution and prng together.
  //boost::variate_generator<boost::lagged_fibonacci19937&, boost::normal_distribution<T> > rollItBaby((engine),norm_dist);
  boost::variate_generator<boost::mt19937&, boost::normal_distribution<T> > rollItBaby((engine),norm_dist);
  const T value = rollItBaby();
  return value;
}

int main (void)
{

  UF::Statistics::MeanSD<double> msd;
  //Distribution dist(1,0.5,12u);
  Distribution dist(0,0.29);
  for(int i = 0; i < 1000000; ++i)
  {
    msd.Add(dist());
    //msd.Add(randomSampler1<double>(0,0.29));
  }

  std::cout << "Number:             " << std::fixed << msd.NumberOfObservations() << std::endl;
  std::cout << "Mean:               " << msd.Mean() << std::endl;
  std::cout << "Variance:           " << msd.Variance() << std::endl;
  std::cout << "Standard Deviation: " << msd.StandardDeviation() << std::endl;
  std::cout << "Minimum:            " << msd.Minimum() << std::endl;
  std::cout << "Maximum:            " << msd.Maximum() << std::endl;

  msd.Reset();

  for(int i = 0; i < 1000000; ++i)
  {
    msd.Add(randomSampler1<double>(0,0.29));
  }

  std::cout << "Number:             " << std::fixed << msd.NumberOfObservations() << std::endl;
  std::cout << "Mean:               " << msd.Mean() << std::endl;
  std::cout << "Variance:           " << msd.Variance() << std::endl;
  std::cout << "Standard Deviation: " << msd.StandardDeviation() << std::endl;
  std::cout << "Minimum:            " << msd.Minimum() << std::endl;
  std::cout << "Maximum:            " << msd.Maximum() << std::endl;


  return 0;
}
