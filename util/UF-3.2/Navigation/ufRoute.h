//
// C++ Interface: Route
//
// Description:
//
//
// Author:  Andrew J. P. Maclean, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef NAVIGATION_ROUTE_H
#define NAVIGATION_ROUTE_H

#include "ufWin32Header.h"

#include "ufWaypoint.h"
#include <vector>
#include <set>

namespace UF {
//! Classes for navigation.
namespace Navigation {


//! Route represents route - an ordered list of waypoints representing a series of turn points leading to a destination. 
/*!
    See: http://www.topografix.com/GPX/1/1/#type_rteType
 */
  class UF_NAV_EXPORT Route: public Link
{
public:
  typedef std::vector<Waypoint> TRoute;
  typedef TRoute::iterator TRouteIt;
  typedef TRoute::const_iterator TRouteCIt;
  typedef std::set<std::string> TRouteGPXElements;

  Route():
    number((unsigned int)(-1))
  {
    Init();
  };

  ~Route(){};

  //! Initialise the variables.
  void Init();

public:

  //! Get the route name.
  /*!
      @return GPS name of route.
  */
  std::string GetName() const
  {
    return this->name;
  }

  //! Get the route comment.
  /*!
      @return comment for the route.
  */
  std::string GetComment() const
  {
    return this->comment;
  }

  //! Get the route description.
  /*!
      @return discription for the route.
  */
  std::string GetDescription() const
  {
    return this->description;
  }

  //! Get the source of the route.
  /*!
      @return source of the route.
  */
  std::string GetSource() const
  {
    return this->source;
  }

  //! Get the route number.
  /*!
      @return route number.
  */
  unsigned int GetNumber() const
  {
    return this->number;
  }

  //! Get the route type.
  /*!
      @return route type.
  */
  std::string GetType() const
  {
    return this->type;
  }

  //! Set the route name.
  /*!
      @param name - GPS name of route.
  */
  void SetName(std::string const & name)
  {
    this->name = name;
  }

  //! Set the route comment.
  /*!
      @param comment - the comment for the route.
  */
  void SetComment(std::string const & comment)
  {
    this->comment = comment;
  }

  //! Set the route description.
  /*!
      @param description - the description for the route.
  */
  void SetDescription(std::string const & description)
  {
    this->description = description;
  }

  //! Set the source of the route.
  /*!
      @param source - the source of the route.
  */
  void SetSource(std::string const & source)
  {
    this->source = source;
  }

  //! Set the route number.
  /*!
      @param number - the route number.
  */
  void SetNumber(unsigned int const & number)
  {
    this->number = number;
  }

  //! Set the route number.
  /*!
      @param number - the route number.
  */
  void SetNumber(std::string const & number)
  {
    std::istringstream is;
    is.str(number);
    is >> this->number;
  }

  //! Set the route type.
  /*!
      @param type - the route type.
  */
  void SetType(std::string const & type)
  {
    this->type = type;
  }

 //! Get the route.
  /*!
      @return the address of the route.
  */
  TRoute * GetRoute() { return &this->route; }

  //! Return an XML representation of the route.
  /*!
      @return The route.
  */
  std::string ToXML(int indent = 2);

  //! Return the bounds of the route.
  /*! 
      Only latitude, longitude and elevation are considered.
      The bounds refer only to the existing positions in the route. 
      If the route is a great circle the bounds may lie outside these positions.

      The minimum elevation will be found in the south-eastern bound.

      se - south-eastern bound of the route.
      nw - north-western bound of the route.
   */
  void GetBounds(Waypoint & se, Waypoint & nw);

  //! Get the GPXElements.
  /*!
      @return the address of GPXElements.
  */
  TRouteGPXElements * GetGPXElements() { return &this->GPXElements; }

  //! Set an element.
  /*!
      Elements not set here are processed in the base classes.

      @param key - the attribute.
      @param value - its value.
   */
  template <typename T>
  void SetElement(std::string const & key, T const & value)
  {
    if ( GPXElements.find(key) == GPXElements.end() )
    {
      // Not a valid key.
      return;
    }

    if ( key == "name" )
    {
      SetName(value);
      return;
    }
    if ( key == "cmt" )
    {
      SetComment(value);
      return;
    }
    if ( key == "desc" )
    {
      SetDescription(value);
      return;
    }
    if ( key == "src" )
    {
      SetSource(value);
      return;
    }
    // Can be int or string.
    if ( key == "number" )
    {
      SetNumber(value);
      return;
    }
    if ( key == "type" )
    {
      SetType(value);
      return;
    }
  }

    //! Get the link.
  /*!
      @return the link
   */
  Link GetLink() const
  {
    Link l;
    l.SetHref(this->Link::GetHref());
    l.SetText(this->Link::GetText());
    l.SetType(this->Link::GetType());
    return l;
  }

  //! Set the link.
  /*!
      @param the link
   */
  void SetLink(Link const & l)
  {
    this->Link::SetHref(l.GetHref());
    this->Link::SetText(l.GetText());
    this->Link::SetType(l.GetType());
  }

protected:
  TRouteGPXElements GPXElements;

  //! Indent a string by this amount.
  /*!
      @return A series of spaces corresponding to the indent.
  */
  std::string Indent(int const & indent);

  //! The name of the route.
  std::string name;
  //! Comment about the route.
  std::string comment;
  //! Description about the route.
  std::string description;
  //! Source of the route.
  std::string source;
  //! Track number.
  unsigned int number;
  //! Type of route.
  std::string type;


  //! The route.
  TRoute route;
};

} // Namespace Navigation.
} // Namespace UF.

#endif // NAVIGATION_ROUTE_H
