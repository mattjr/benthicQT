#ifdef _MSC_VER
#pragma once
#endif


#if !defined(COLOUR_H)
#define COLOUR_H

#include <string>
#include <vector>
#include <map>
#include <iterator>
#include <sstream>
#include <iomanip>
#include <cmath>

#include "ufWin32Header.h"

namespace UF {

//! A set of classes designed to manipulate colours and their red, green and blue values.
namespace RGBColour {

  //! The minimum of two values.
  template < typename T >
     inline T const& min_( T const& a, T const& b )
  {
    return  (a < b) ? a : b;
  }

  //! The minimum of two values.
  template < typename T >
    inline T* const& min_( T* const& a, T* const& b )
  {
    return  *a < *b ? a : b;
  }

  //! The minimum of three values.
  template < typename T >
    inline T const& min_( T const& a, T const& b, T const& c)
  {
    return min_(min_(a,b),c);
  }

  //! The minimum of three values.
  template < typename T >
    inline T* const& min_( T* const& a, T* const& b, T* const& c)
  {
    return min_(min_(a,b),c);
  }

  //! The maximum of two values.
  template < typename T >
    inline T const& max_( T const& a, T const& b )
  {
    return a > b ? a : b;
  }

  //! The maximum of two values.
  template < typename T >
    inline T* const& max_( T* const& a, T* const& b )
  {
    return  *a > *b ? a : b;
  }

  //! The maximum of three values.
  template < typename T >
    inline T const& max_( T const& a, T const& b, T const& c)
  {
    return max_(max_(a,b),c);
  }

  //! The maximum of three values.
  template < typename T >
    inline T* const& max_( T* const& a, T* const& b, T* const& c)
  {
    return max_(max_(a,b),c);
  }


  //! An interface for storing and retrieving a colour consisting of its red, green and blue values.
  /*!
    CLASS colour_base_<T>

    This class provides the interface for storing and retrieving
    the Red, Green and Blue values of a colour.
    <b>Note:</b><i> It is the responsibility of the user to ensure
    that the values for red, green and blue are consistent with
    the system that they are using. For example, if integral types
    are used, such as unsigned int, int etc., the red, green and
    blue values usually  range between 0...255. Whilst, for floats
    and doubles, the range is usually [0...1].</i>

    This implementation stores the individual colour values as three
    separate variables.
    In the constructors, alpha is set to 1 (opaque), so dependent upon
    the system you are using,  you may have to change this to match the
    maximum opacity. Functions using scale and clamp by default assume
    that they are 1. Thus you may also have to change this if you are
    using floats or doubles not in the range [0...1], you certainly will
    need to change this if you are using integral types.

    This is a base class and Colour uses this to store values in.

    \version 3.0

    \date 22-Aptil-2004

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
  template <typename T>
  class /*UF_COLOUR_EXPORT*/ colour_base_
  {
  public:

    colour_base_ (void):
        r(0)
        , g(0)
        , b(0)
        , a(1)
    {}

    colour_base_ (T const &r, T const &g, T const &b, T const &a = 1) :
      r(r)
      , g(g)
      , b(b)
      , a(a)
    {}

    //! Copy constructor
    colour_base_ ( colour_base_ <T> const & rhs )
    {
      this->r = rhs.r;
      this->g = rhs.g;
      this->b = rhs.b;
      this->a = rhs.a;
    }

    //! Assignment
    colour_base_ &operator = ( colour_base_ < T > const & rhs )
    {
      if ( this == &rhs )
        return *this;

      this->r = rhs.r;
      this->g = rhs.g;
      this->b = rhs.b;
      this->a = rhs.a;
      return *this;
    }

    //! Test for ==
    bool operator == ( colour_base_ < T > const& rhs ) const
    {
        return this->r == rhs.r && this->g == rhs.g && this->b == rhs.b && this->a == rhs.a;
    }
    //! Test for !=
    bool operator != ( colour_base_ < T > const& rhs ) const
    {
      return !(*this == rhs);
    }

    virtual ~colour_base_ (void){};

    T red(void) const { return r; }
    T green(void) const { return g; }
    T blue(void) const { return b; }
    T alpha(void) const { return a; }

    template < typename U >
    void red ( U const & r ) { this->r = (T)r; }
    template < typename U >
    void green ( U const & g ) { this->g = (T)g; }
    template < typename U >
    void blue ( U const & b ) { this->b = (T)b; }
    template < typename U >
    void alpha ( U const & a ) { this->a = (T)a; }

  private:
    T r;
    T g;
    T b;
    T a;
  };


  //! An interface for storing and retrieving a colour consisting of its red, green and blue values.
  /*!
    CLASS colour_base_<unsigned char>

    This is a specialisation of colour_base_<T>.
    This class provides the interface for storing and retrieving
    the Red, Green and Blue values of a colour.

    The colour is stored as a single unsigned int.

    In the constructors, alpha is set to 255 (opaque) and functions using scale
    and clamp assume that they are 255. This is because the default underlying
    type of base_colour is unsigned char.

    \version 3.0

    \date 22-Aptil-2004

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
    template <>
    class /*UF_COLOUR_EXPORT*/ colour_base_ < unsigned char >
    {
    public:

    colour_base_ (void) :
      c(0x000000ff)
    {}

    colour_base_ (unsigned int const & c) :
      c(c)
    {}

    colour_base_ (unsigned char const &r, unsigned char const &g, unsigned char const &b, unsigned char const &a = 255)
    {
      c = ((unsigned int)(r) << 24) + ((unsigned int)(g) << 16) + ((unsigned int)(b) << 8) + ((unsigned int)(a));
    }

    //! Copy constructor
    colour_base_ ( colour_base_ < unsigned char > const & rhs )
    {
      this->c = rhs.c;
    }

    //! Assignment
    colour_base_ &operator = ( colour_base_ < unsigned char > const & rhs )
    {
      if ( this == &rhs )
        return *this;

      this->c = rhs.c;
      return *this;
    }

    //! Test for ==
    bool operator == ( colour_base_ < unsigned char > const& rhs ) const
    {
        return this->c == rhs.c;
    }
    //! Test for !=
    bool operator != ( colour_base_ < unsigned char > const& rhs ) const
    {
      return !(*this == rhs);
    }

    virtual ~colour_base_ (void){};

    unsigned char red(void) const { return (unsigned char)((c>>24) & 0xff); }
    unsigned char green(void) const { return (unsigned char)((c>>16) & 0xff); }
    unsigned char blue(void) const { return (unsigned char)((c>>8) & 0xff); }
    unsigned char alpha(void) const { return (unsigned char)(c & 0xff); }

    void red ( unsigned char r ) { this->c = ((unsigned int)(r) << 24) + (c & 0x00ffffff ); }
    void green ( unsigned char g ) { this->c = ((unsigned int)(g) << 16) + ( c & 0xff00ffff ); }
    void blue ( unsigned char b ) { this->c = ((unsigned int)(b) << 8) + ( c & 0xffff00ff ); }
    void alpha ( unsigned char a ) { this->c = ((unsigned int)(a)) + ( c & 0xffffff00 ); }

    //! Get the value of the colour in the form 0xrrggbbaa.
    unsigned int get_colour( void )  const { return c; }

    //! Set the colour ( the colour must be in the form 0xrrggbbaa.
    void set_colour ( unsigned int const & colour ) { c = colour; };

    //! Return the hexadecimal value of a colour as a string (rrggbb).
    std::string hex( void ) const
    {
      std::ostringstream os;
      unsigned int res = c >> 8;
      os << std::hex;
      os.setf(std::ios::uppercase);
      os << std::setw(6) << std::setfill('0') << res;
      return os.str();
    };

    //! Return the complement of the hexadecimal value of a colour as a string (rrggbb).
    std::string hex_complement ( void ) const
    {
      std::ostringstream os;
      unsigned int res = 0xffffff - (c >> 8);
      os << std::hex;
      os.setf(std::ios::uppercase);
      os << std::setw(6) << std::setfill('0') << res;
      return os.str();
    };

  private:
    unsigned int c;

  };

  //! An interface for storing and retrieving a colour consisting of its name and red, green and blue values.
  /*!
    CLASS Colour<T, U=unsigned char>

    This class provides the interface for storing and retrieving
    the Name, Red, Green and Blue values of a colour.

    In the constructors, alpha is set to 255 (opaque) and functions using scale
    and clamp assume that they are 255. This is because the default underlying
    type of base_colour is unsigned char.

    \version 3.0

    \date 22-Aptil-2004

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
  template < typename T = unsigned char, typename U = unsigned char >
  class /*UF_COLOUR_EXPORT*/ Colour
  {
  public:
    Colour ( void ) :
      name("black")
    {}

    Colour (std::string const & name, T const &r, T const &g, T const &b, T const &a = 1) :
      name(name)
      , value((U)r,(U)g,(U)b,(U)a)
    {}

    Colour (std::string const & name, colour_base_<U> const & value) :
      name(name)
      , value(value)
    {}

    Colour (Colour const &rhs)
    {
      *this = rhs;
    }

    //! Assignment
    Colour &operator = ( Colour<T,U> const & rhs )
    {
      if ( this == &rhs )
        return *this;

      this->name = rhs.name;
      this->value.red(rhs.value.red());
      this->value.green(rhs.value.green());
      this->value.blue(rhs.value.blue());
      this->value.alpha(rhs.value.alpha());
      return *this;
    }

    //! Test for ==
    bool operator == ( Colour<T,U> const & rhs ) const
    {
      return ( this->name == rhs.name &&
        this->value.red() == rhs.value.red() &&
        this->value.green() == rhs.value.green() &&
        this->value.blue() == rhs.value.blue() &&
        this->value.alpha() == rhs.value.alpha()
        );
    }

    //! Test for !=
    bool operator != ( Colour<T,U> const & rhs ) const
    {
      return !(*this == rhs);
    }

    virtual ~Colour ( void ) { }

    std::string colour_name ( void ) { return this->name; }
    std::string colour_name ( void ) const { return this->name; }

    void colour_name ( std::string const & name ) { this->name = name; }

    colour_base_<U> colour_value ( void ) { return value; }
    colour_base_<U> colour_value ( void ) const { return value; }

    void colour_value ( colour_base_<U> const & value ) { this->value = value; }

    void set_colour (std::string const & name, colour_base_<U> const & value)
    {
      this->name = name;
      this->value = value;
    }

    void set_colour (std::string const & name, T const &r, T const &g, T const &b, T const &a = 1)
    {
      this->name = name;
      this->value.red((U)r);
      this->value.green((U)g);
      this->value.blue((U)b);
      this->value.alpha((U)a);
    }

    void get_colour (std::string & name, T &r, T &g, T &b, T &a)
    {
      name = this->name;
      r = (T)this->value.red();
      g = (T)this->value.green();
      b = (T)this->value.blue();
      a = (T)this->value.alpha();
    }

    void get_colour (std::string & name, T &r, T &g, T &b, T &a) const
    {
      name = this->name;
      r = (T)this->value.red();
      g = (T)this->value.green();
      b = (T)this->value.blue();
      a = (T)this->value.alpha();
    }

    void set_colour_value (T const &r, T const &g, T const &b, T const &a = 1)
    {
      this->value.red((U)r);
      this->value.green((U)g);
      this->value.blue((U)b);
      this->value.alpha((U)a);
    }

    void get_colour_value (T &r, T &g, T &b, T &a)
    {
      r = (T)this->value.red();
      g = (T)this->value.green();
      b = (T)this->value.blue();
      a = (T)this->value.alpha();
    }

    void get_colour_value (T &r, T &g, T &b, T &a) const
    {
      r = (T)this->value.red();
      g = (T)this->value.green();
      b = (T)this->value.blue();
      a = (T)this->value.alpha();
    }

  private:
    std::string name;
    colour_base_<U> value;
  };

  //! The colour map, the key is the colour name.
  typedef std::map< std::string, colour_base_< unsigned char > > TColour_Map;
  //! Iterator for the colour map.
  typedef TColour_Map::iterator TColour_Map_Itr;
  //! Const iterator for the colour map.
  typedef TColour_Map::const_iterator TColour_Map_CItr;

  //! An interface for storing and retrieving a colour consisting of its red, green and blue values.
  /*!
    CLASS Colour_Map

    This is a map of colour names and their values keyed on colour name.

    The default constructor (PrimaryColourMap = false) constructs a colour
    map with a large number of different colours.

    The alternate constructor (PrimaryColourMap = true) constructs a primary
    colour map consisting of just the primary colours (red, green, blue),
    their complements (cyan,magenta,yellow), and, finally, black and white.

    You can easily switch between the two maps by using reinitialise().

    The underlying map is public so that you can easily access the usual map functions.

    You are not restricted to colours in these lists, you may insert
    and/or erase whatever colours that you want by using the appropriate
    functions, namely insert() and erase(). You can also change a colour
    value (the red, green, blue component) by using change().


    An example using both colour maps is:

  \code
    // Here we are assuming that a colour match is most likley
    // to occur with the primary colours.
    // Additionally if the colour is white it will be matched as
    // gray 100 instead of white. This is because the key gray 100
    // occurs before white (both have the same value). So using
    // the primary colour table to establish a match first in this
    // case will ensure that if the colour is white the name white
    // will be selected.

    // Try different values here. If match_value() fails, black is returned.
    unsigned char r = 255, g = 255, b = 255, a = 255;

    // Here we match on primary colours and if it fails, use the full colour map.
    // Colour table with just the primary colours.
    RGBColour::Colour_Map cm(true);

    RGBColour::Colour<unsigned char> colour;

    colour.set_colour_value(r,g,b,a);

    if (!cm.match_value(colour))
    {
      // Match using the full colour table.
      cm.reinitialise(); // Use the full colour table.
      cm.match_value(colour);
    }

    std::cout << "The ambient colour is " << colour.colour_name() << std::endl;

  \endcode

    \version 3.0

    \date 22-Aptil-2004

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
  class /*UF_COLOUR_EXPORT*/ Colour_Map
  {
  public:

    Colour_Map( bool primary_colours = false)
      : primary_colours(primary_colours) 
    { 
      this->init(); 
    }

    //! Copy constructor
    Colour_Map ( Colour_Map const& rhs )
    {
      this->primary_colours = rhs.primary_colours;
      this->colour_map = rhs.colour_map;
    }

    virtual ~Colour_Map ( void ) {};

    //! Test for ==
    bool operator == ( Colour_Map const& rhs ) const
    {
      if ( this->primary_colours != rhs.primary_colours )
        return false;
      else
        return this->colour_map == rhs.colour_map;
    }

    //! Test for !=
    bool operator != ( Colour_Map const& rhs ) const
    {
      return !((*this) == rhs);
    }

    //! Assignment
    Colour_Map &operator = ( Colour_Map const& rhs )
    {
      if ( this == &rhs )
        return *this;

      this->primary_colours = rhs.primary_colours;
      this->colour_map = rhs.colour_map;
      return *this;
    }

    //! Reinitialise the colour map.
    /*!
       This function will clear the current colour map
       and reset the map to its initial state.

       If PrimaryColourmap is true then the map consisting only
       of the primaries, their complements, black and white are
       constructed.
       If it is false (the default) then all the colours are used
       to construct the colour map.
      @return void :
    */
    void reinitialise ( bool primary_colours = false )
    {
      this->primary_colours = primary_colours;
      this->colour_map.clear();
      init();
    }

    //! Returns true if the colour map is a primary colour map.
    /*!
       Note that inserting a colour into a primary colour
       map will mean that this function will return false.

       @return bool : True if it is a primary colour map.
     */
    bool is_a_primary_colour_map ( void ) { return this->primary_colours; };

    //! If it is a primary colour map, change it to a full colour map.
    /*!
       Note that this function will only execute iff IsAPrimaryColourMap()
       is true.

       @return void :
     */
    void change_to_full_colour_map ( void ) { if (this->primary_colours) init(); }

    //! Get the names of all the colours.
    /*!
      @return std::vector<std::string> get_names  : the names
    */
    std::vector<std::string> get_names ( void )
    {
      std::vector<std::string> s;

      for(TColour_Map_Itr i = colour_map.begin(); i != colour_map.end(); ++i)
      {
        if (!i->first.empty()) // All colours have a name.
          s.push_back(i->first);
      }
      return ( s );
    }

    //! Insert an element into the map.
    /*!
      If the key exists false is returned and the value in the map
      is unchanged.
      @param colour : the colour

      @return bool Insert  : true if the key does not exist and false if the key exists.
    */
    bool insert ( Colour<unsigned char> &colour )
    {
      std::pair<TColour_Map_Itr, bool> ib;
      ib = colour_map.insert(TColour_Map::value_type(colour.colour_name(),colour.colour_value()));
      return ib.second;
    }

    //! Change the value of a key in the map.
    /*!
      If the key does not exist then insert it and the value.
      @param colour : the colour

      @return bool change  : true if the value was changed.
    */
    bool change ( Colour<unsigned char> &colour )
    {
      erase(colour);
      return (insert(colour));
    }

    //! Erase a colour and return the number of elements removed.
    /*!
      @param name : the name of the colour

      @return std::size_t Erase  : the number of elements removed
    */
    std::size_t erase ( std::string const &name)
    {
      return colour_map.erase(name);
    }

    //! Erase a colour and return the number of elements removed.
    /*!
      @param colour : the colour to erase

      @return std::size_t Erase  : the number of elements removed
    */
    std::size_t erase ( Colour<unsigned char> &colour )
    {
      return colour_map.erase(colour.colour_name());
    }

    //! Return the colour corresponding to a name.
    /*!
      If the name is not present in the map then the colour name and colour is unchanged.

      @param &colour : the colour

      @return bool : True if the colour was found, false otherwise.
    */
    bool colour(Colour<unsigned char> & colour)
    {
      TColour_Map_Itr p = colour_map.find(colour.colour_name());
      if ( p != colour_map.end() )
      {
        colour.colour_name(p->first);
        colour.colour_value(p->second);
        return true;
      }
      return false;
    }

    //! Return the colour corresponding to a name.
    /*!
      If the name is not present in the map then the colour corresponding
      to black is returned but with an empty name. So the caller should
      check for an empty name.

      @param &name : the name of the colour to look for.

      @return bool : The colour if found, black otherwise.
    */
    Colour<unsigned char> colour( std::string const & name) const
    {
      Colour<unsigned char> colour;
      TColour_Map_CItr p = colour_map.find(name);
      if ( p != colour_map.end() )
      {
        colour.colour_name(p->first);
        colour.colour_value(p->second);
      }
      return colour;
    }

    //! Match the r,g,b,a values of a colour to the first one found in the map.
    /*!
        If true then the name in colour will be changed to match that in the map.

        @param &colour : the colour to be matched.
        @param &useAlpha : If true, then the the alpha value is also used when matching a colour.

        @return bool : True if the colour was found, false otherwise.
      */
    bool match_value(Colour<unsigned char> & colour, const bool & useAlpha = false )
    {
      // Attempt to find the first match.
      for ( TColour_Map_Itr p = colour_map.begin(); p != colour_map.end(); ++p )
      {
        unsigned char r, g, b, a;
        colour.get_colour_value(r,g,b,a);
        if ( p->second.red()   == r &&
             p->second.green() == g &&
             p->second.blue()  == b
           )
        {
          if ( useAlpha )
          {
            if ( p->second.alpha() == a )
            {
              colour.colour_name(p->first);
              return true;
            }
            else
              return false;
          }
          colour.colour_name(p->first);
          return true;
        }
      }
      return false;
    }

  private:

      //! Initialise the colour map.
      /*!
      * Insert the colours into the map
      *
      * @return void  :
      */
      void init( void );

      //! Parse a string consisting of "name r g b" into its constituents.
      /*!
        Assumes that the string has the form:
        "colourname red green blue"
        where each value is separated by whitespace.

        @param &s : the string to be parsed
        @param &colour : the resultant colour

      */
      void decode_colour_table(std::string const& s, Colour<unsigned char> & colour);

  public:
    // The map of colours. This is public so that the user has full access to all the map functions/operators.
    TColour_Map colour_map;

  private:
    //! If this is true then the map consists of only the primary colours.
    bool primary_colours;

  };

  //! The value of the colour.
  /*!
    STRUCT V

    The value of the colour.

    \version 1.0

    \date 22-Aptil-2004

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
  template < typename T = double >
    struct /*UF_COLOUR_EXPORT*/ V
    {
      //! Return the value of the colour.
      /*!
        The value is returned as the maximum
        of the red, green and blue value.
        Assumes that the red, green and blue values are
        in the range [0...scale].

        @return double  : the value V [0...1]
      */
      double operator () ( T const & red, T const & blue, T const & green, T const & scale = 1 ) const
      {
        double r = red;
        double g = green;
        double b = blue;
        double s = scale;
        if ( s != 1 )
          return max_(r,g,b)/s;
        else
          return max_(r,g,b);
      }
    };

  //! The lightness of the colour.
  /*!
    STRUCT L

    The lightness of the colour.

    \version 1.0

    \date 22-Aptil-2004

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
  template < typename T = double >
    struct /*UF_COLOUR_EXPORT*/ L
    {
    //! Return the lightness of the colour.
    /*!
      The lightness is returned as the average of
      the maximum and minimum of the red, green
      and blue value.
      Assumes that the red, green and blue values are
      in the range [0...scale].

      @return double  : the lightness L [0...1]
    */
      double operator () ( T const & red, T const & blue, T const & green, T const & scale = 1 ) const
      {
        double r = red;
        double g = green;
        double b = blue;
        double s = scale;
        double tmp1 = max_(r,g,b);
        double tmp2 = min_(r,g,b);
        if ( s != 1 )
          return (tmp1 + tmp2) / (2.0*s);
        else
          return (tmp1 + tmp2) / 2.0;
      }
    };

  //! The brightness of the colour.
  /*!
    STRUCT B

    The brightness of the colour.

    \version 1.0

    \date 22-Aptil-2004

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
  template < typename T = double >
    struct /*UF_COLOUR_EXPORT*/ B
    {
      //! Return the brightness of the colour.
      /*!
        The brightness is returned as the average
        of the red, green and blue value.
        Assumes that the red, green and blue values are
        in the range [0...scale].

        @return double  : the brightness B [0...1]
      */
      double operator () ( T const & red, T const & blue, T const & green, T const & scale = 1 ) const
      {
        double r = red;
        double g = green;
        double b = blue;
        double s = scale;
        if ( s != 1 )
          return (r+g+b) / (3.0*s);
        else
          return (r+g+b) / 3.0;
      }
    };

  //! The luminance of the colour.
  /*!
    STRUCT L

    The luminance of the colour.

    \version 1.0

    \date 22-Aptil-2004

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
  template < typename T = double >
    struct /*UF_COLOUR_EXPORT*/ Y
    {
      //! Return the luminance of the colour.
      /*!
        The luminance is regarded as the closest
        match of brightness to human perception.
        The coefficients for r, g, b, are supposed
        to mimic the sensitivity of the human eye.
        Assumes that the red, green and blue values are
        in the range [0...scale].

        @return double  : the luminance Y [0...1]
      */
      double operator () ( T const & red, T const & blue, T const & green, T const & scale = 1 ) const
      {
        double r = red;
        double g = green;
        double b = blue;
        double s = scale;
        if ( s != 1 )
          return (0.30*r+0.59*g+0.11*b)/s;
        else
          return (0.30*r+0.59*g+0.11*b);
      }
    };

  //! The CIE luminance of the colour.
  /*!
    STRUCT Y709

    The CIE luminance of the colour.

    \version 1.0

    \date 22-Aptil-2004

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
  template < typename T = double >
    struct /*UF_COLOUR_EXPORT*/ Y709
    {
      //! Return the CIE luminance of the colour.
      /*!
        The CIE coefficients for r, g, b, are supposed
        to mimic the sensitvity of the human eye.
        Assumes that the red, green and blue values are
        in the range [0...scale].

        @return double  : the luminance Y [0...1]
      */
      double operator () ( T const & red, T const & blue, T const & green, T const & scale = 1 ) const
      {
        double r = red;
        double g = green;
        double b = blue;
        double s = scale;
        if ( s != 1 )
          return (0.2125*r+0.7154*g+0.0721*b)/s;
        else
          return (0.2125*r+0.7154*g+0.0721*b);
      }
    };

  //! Convert a RGB value to an HSV value.
  /*!
    The Hue/Saturation Value system (or also called Hexcone model) was created by
    Smith in 1978. It was for an aesthetic purpose, accessing color by family,
    purity and intensity rather than by its component. With that model it becomes
    easy to produce some kind of hellish brown or some kind of
    you-know-that-color-in-between-(x) and (y).
    The H value is a 360 degree value through color families.
    The S (or Saturation) is the degree of strength of a color. Greater is S, the
    purest is the color. if S max is 1.00, then Hue=red and S=1.00 would produce an
    intense red (reproduced by RGB (max,0,0))
    Finally, the V, for value, is the darkness/lightness of a color. More V is
    great, more the color is close to white.
    See: http://astronomy.swin.edu.au/~pbourke/colour/conversion.html

    The conversions are based on FORTRAN rountines found at:
    http://www.psc.edu/~burkardt/src/colors/colors.html

    \version 1.0

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
  struct /*UF_COLOUR_EXPORT*/ RGB_HSV {
    //! Convert a RGB value to an HSV value.
    /*!
      This functor converts a RGB value to an HSV value.
       r,g,b values are from 0 to 1
       h = [0,360], s = [0,1], v = [0,1]
       if s == 0, then h = -1 (undefined)

      @param r : red value
      @param g : green value
      @param b : blue value
      @param &h : hue value
      @param &s : saturation value
      @param &v : brightness (luminance) value

      @return  :
    */
    void operator() (
      double r,
      double g,
      double b,
      double& h,
      double& s,
      double& v)
    {

      double cmin, cmax, delta;
      cmin = min_(r,g,b);
      cmax = max_(r,g,b);

      v = cmax;        //  the lightness
      delta = cmax - cmin;
      s = ( delta == 0 || cmax == 0 )? 0 : delta / cmax;
      if ( s == 0 )
      {
        // s == 0, h is undefined so set it to -1
        h = -1;
        return;
      }
      h = (r == cmax)? ( g - b ) / delta:  // between yellow and magenta
        (g == cmax)? 2 + ( b - r ) / delta:  // between cyan and yellow
              4 + ( r - g ) / delta;  // between magenta and cyan
      h *= 60; // Convert to degrees
      if( h < 0 )
        h += 360;
    }

  };

  //! Convert an HSV value to a RGB value.
  /*!
    The Hue/Saturation Value system (or also called Hexcone model) was created by
    Smith in 1978. It was for an aesthetic purpose, accessing color by family,
    purity and intensity rather than by its component. With that model it becomes
    easy to produce some kind of hellish brown or some kind of
    you-know-that-color-in-between-(x) and (y).
    The H value is a 360 degree value through color families.
    The S (or Saturation) is the degree of strength of a color. Greater is S, the
    purest is the color. If S max is 1.00, then Hue=red and S=1.00 would produce an
    intense red (reproduced by RGB (max,0,0))
    Finally, the V, for value, is the darkness/lightness of a color. More V is
    great, more the color is close to white.
    See: http://astronomy.swin.edu.au/~pbourke/colour/conversion.html

    The conversions are based on FORTRAN rountines found at:
    http://www.psc.edu/~burkardt/src/colors/colors.html

    \version 1.0

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
  struct /*UF_COLOUR_EXPORT*/ HSV_RGB
  {
    //! Convert an HSV value to a RGB value.
    /*!
      This functor converts an HSV value to a RGB value.
       r,g,b values are from 0 to 1
       h = [0,360], s = [0,1], v = [0,1]
       if s == 0, then h = -1 (undefined)

      @param h : hue value
      @param s : saturation value
      @param v : brightness (luminance) value
      @param &r : red value
      @param &g : green value
      @param &b : blue value

      @return  :
    */
    void operator() (
      double h,
      double s,
      double v,
      double& r,
      double& g,
      double& b)
    {
      int i;
      double f, p, q, t;
      if( s == 0 ) {
        // achromatic (grey)
        r = g = b = v;
        return;
      }
      h /= 60;      // sector 0 to 5
      i = (int)std::floor( h );
      f = h - i;      // fractional part of h
      p = v * ( 1 - s );
      q = v * ( 1 - s * f );
      t = v * ( 1 - s * ( 1 - f ) );
      switch( i ) {
        case 0:
          r = v; g = t; b = p; break;
        case 1:
          r = q; g = v; b = p; break;
        case 2:
          r = p; g = v; b = t; break;
        case 3:
          r = p; g = q; b = v; break;
        case 4:
          r = t; g = p; b = v; break;
        default:    // case 5
          r = v; g = p; b = q; break;
      }
    }
  };

  //! Convert a RGB value to an HLS value.
  /*!
    The HLS model is in some fashion a boosted HSV model. Even if it's based on the
    Ostwald system, created around 1930 (not sure: maybe it was 1931). It forms a
    dodecahedron formed by two Hexcone base to base, with one sharp point black, and
    with the other white. It has some disadvantages since the purety of color is
    not at maxvalue but at its half ( = 0.5). It makes it a little harder to use
    with the common 3 axis (sliders if you use Mac or Windows) H,L,S. By the way: H
    is for Hue, L for lightness. Lightness of 1=white, 0=black and S is still
    saturation.
    See: http://astronomy.swin.edu.au/~pbourke/colour/conversion.html

    The conversions are based on FORTRAN rountines found at:
    http://www.psc.edu/~burkardt/src/colors/colors.html

    \version 1.0

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
  struct /*UF_COLOUR_EXPORT*/ RGB_HLS
  {
    //! Convert a RGB value to an HLS value.
    /*!
      This functor converts a RGB value to an HLS value.
       r,g,b values are from 0 to 1
       h = [0,360], s = [0,1], l = [0,1]
       if s == 0, then h = -1 (undefined)

      @param r : red value
      @param g : green value
      @param b : blue value
      @param &h : hue value
      @param &l : lightness value
      @param &s : saturation value

      @return  :
    */
    void operator() (
      double r,
      double g,
      double b,
      double& h,
      double& l,
      double& s )
    {
      // Compute lightness.
      double rgbmax = max_(r,g,b);
      double rgbmin = min_(r,g,b);
      l = ( rgbmax + rgbmin ) / 2.0;

      // Compute saturation.
      if ( rgbmax == rgbmin )
        s = 0.0;
      else
      {
        if ( l <= 0.5 )
          s = ( rgbmax - rgbmin ) / ( rgbmax + rgbmin );
        else
          s = ( rgbmax - rgbmin ) / ( 2.0 - rgbmax - rgbmin );
      }

      // Compute the hue.
      double rc = 0, gc = 0, bc = 0;

      if ( rgbmax == rgbmin )
        h = -1.0; // Undefined
      else
      {
        rc = ( rgbmax - r ) / ( rgbmax - rgbmin );
        gc = ( rgbmax - g ) / ( rgbmax - rgbmin );
        bc = ( rgbmax - b ) / ( rgbmax - rgbmin );
      }

      if ( r == rgbmax ) h = bc - gc;
      else if ( g == rgbmax ) h = 2.0 + rc - bc;
      else h = 4.0 + gc - rc;

      h = h * 60.0;

      // Make sure H lies between 0 and 360.0.
      if ( h < 0.0 ) h = h + 360.0;
    }
  };

  //! Convert an HLS value to a RGB value.
  /*!
    The HLS model is in some fashion a boosted HSV model. Even if it's based on the
    Ostwald system, created around 1930 (not sure: maybe it was 1931). It forms a
    dodecahedron formed by two Hexcone base to base, with one sharp point black, and
    with the other white. It has some disadvantages since the purety of color is
    not at maxvalue but at its half ( = 0.5). It makes it a little harder to use
    with the common 3 axis (sliders if you use Mac or Windows) H,L,S. By the way: H
    is for Hue, L for lightness. Lightness of 1=white, 0=black and S is still
    saturation.
    See: http://astronomy.swin.edu.au/~pbourke/colour/conversion.html

    The conversions are based on FORTRAN rountines found at:
    http://www.psc.edu/~burkardt/src/colors/colors.html

    \version 1.0

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
  struct /*UF_COLOUR_EXPORT*/ HLS_RGB
  {
    //! Convert an HLS value to a RGB value.
    /*!
      This functor converts an HLS value to a RGB value.
       r,g,b values are from 0 to 1
       h = [0,360], s = [0,1], l = [0,1]
       if s == 0, then h = -1 (undefined)

      @param r : red value
      @param g : green value
      @param b : blue value
      @param &h : hue value
      @param &l : lightness value
      @param &s : saturation value

      @return  :
    */
    void operator() (
      double h,
      double l,
      double s,
      double& r,
      double& g,
      double& b )
    {
      double m1, m2;

      if ( l <= 0.5 ) m2 = l + l * s;
      else m2 = l + s - l * s;

      m1 = 2.0 * l - m2;

      if ( s == 0.0 )
      {
        r = l;
        g = l;
        b = l;
      }
      else
      {
        r = hls_value ( m1, m2, h + 120.0 );
        g = hls_value ( m1, m2, h );
        b = hls_value ( m1, m2, h - 120.0 );
      }
    }

  private:

    double hls_value ( double n1, double n2, double h )
    {
      double hue = h;
      if ( hue > 360.0 ) hue = hue - 360.0;
      if ( hue < 0.0 ) hue = hue + 360.0;

      if ( hue < 60.0 )
        return n1 + ( n2 - n1 ) * hue / 60.0;
      else if ( hue < 180.0 )
        return n2;
      else if ( hue < 240.0 )
        return n1 + ( n2 - n1 ) * ( 240.0 - hue ) / 60.0;
      else
        return n1;
    }
  };

//////////// Some useful ordering classes.


  //! Store the h(ue), s(aturation), v(alue) and name and provide weak ordering.
  /*!
    CLASS CKey_hsv

    This class stores the h(ue), s(aturation), v(alue) and name and provides weak ordering.

    \version 1.0

    \date 22-Aptil-2004

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
  class /*UF_COLOUR_EXPORT*/ CKey_hsv
  {
  public:

    CKey_hsv (double const & h,
      double const & s,
      double const & v,
      RGBColour::Colour<unsigned char> const & colour )// :
    {
      this->h = h;
      this->s = s;
      this->v = v;
      this->colour = colour;
    }

    bool operator<(CKey_hsv const &rhs) const
    {
      return this->h < rhs.h ||
              ( this->h == rhs.h &&
                ( this->s < rhs.s ||
                  ( this->s == rhs.s &&
                    ( this->v < rhs.v ||
                      ( this->v == rhs.v &&
                          this->colour.colour_name().compare(rhs.colour.colour_name()) < 0 ) ) ) ) );
    }

  private:
    double h;
    double s;
    double v;
    RGBColour::Colour<unsigned char> colour;
  };

  //! Less functor.
  struct /*UF_COLOUR_EXPORT*/ CKey_hsv_less
  {
    bool operator() ( CKey_hsv const & lhs, CKey_hsv const & rhs ) const
    {
      return lhs < rhs;
    }
  };

  //! Store the h(ue), l(ightness), s(aturation) and name and provide weak ordering.
  /*!
    CLASS CKey_hls

    This class stores the h(ue), l(ightness), s(aturation) and name and provides weak ordering.

    \version 1.0

    \date 22-Aptil-2004

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
  class /*UF_COLOUR_EXPORT*/ CKey_hls
  {
  public:

    CKey_hls (double const & h,
      double const & l,
      double const & s,
      RGBColour::Colour<unsigned char> const & colour )
    {
      this->h = h;
      this->l = l;
      this->s = s;
      this->colour = colour;
    }

    bool operator<(CKey_hls const &rhs) const
    {
      return this->h < rhs.h ||
              ( this->h == rhs.h &&
                ( this->l < rhs.l ||
                  ( this->l == rhs.l &&
                    ( this->s < rhs.s ||
                      ( this->s == rhs.s &&
                          this->colour.colour_name().compare(rhs.colour.colour_name()) < 0 ) ) ) ) );
    }

  private:
    double h;
    double l;
    double s;
    RGBColour::Colour<unsigned char> colour;
  };

  //! Less functor.
  struct /*UF_COLOUR_EXPORT*/ CKey_hls_less
  {
    bool operator() ( CKey_hls const & lhs, CKey_hls const & rhs ) const
    {
      return lhs < rhs;
    }
  };

  //! Store the red, green, blue values and name and provide weak ordering.
  /*!
    CLASS CKey_rgb

    This class stores the red, green, blue values and name and provides weak ordering.

    \version 1.0

    \date 22-Aptil-2004

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
  class /*UF_COLOUR_EXPORT*/ CKey_rgb
  {
  public:

    CKey_rgb (RGBColour::Colour<unsigned char> const & colour )
    {
      this->colour = colour;
    }

    bool operator<(CKey_rgb const &rhs) const
    {
      unsigned char lr, lg, lb, la;
      this->colour.get_colour_value(lr,lg,lb,la);
      unsigned char rr, rg, rb, ra;
      rhs.colour.get_colour_value(rr,rg,rb,ra);

      return lr < rr ||
              ( lr == rr &&
                ( lg < rg ||
                  ( lg == rg &&
                    ( lb < rb ||
                      ( lb == rb &&
                          this->colour.colour_name().compare(rhs.colour.colour_name()) < 0 ) ) ) ) );
    }

  private:
    RGBColour::Colour<unsigned char> colour;
  };

  //! Less functor.
  struct /*UF_COLOUR_EXPORT*/ CKey_rgb_less
  {
    bool operator() ( CKey_rgb const & lhs, CKey_rgb const & rhs ) const
    {
      return lhs < rhs;
    }
  };


  //! Convert a colour from one type to another. Make sure the conversion makes sense.
  template < typename T, typename U = unsigned char, typename V = unsigned char, typename W = unsigned char >
    struct /*UF_COLOUR_EXPORT*/ ConvertColourType
    {
      // Do the conversion, the scale is 1 by default.
      void operator() (RGBColour::Colour<T,U> const & old_colour, RGBColour::Colour<V,W> & new_colour, const double & scale = 1.0
         )
      {
        RGBColour::colour_base_<U> cbo;
        cbo = old_colour.colour_value();
        RGBColour::colour_base_<W> cbn;
        cbn.red((W)((double)cbo.red()*scale));
        cbn.green((W)((double)cbo.green()*scale));
        cbn.blue((W)((double)cbo.blue()*scale));
        cbn.alpha((W)((double)cbo.alpha()*scale));
        new_colour.set_colour(old_colour.colour_name(),cbn);
      }

  };


} // namespace Colour

} // Namespace UF

#endif // !defined(COLOUR_H)
