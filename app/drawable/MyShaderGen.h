/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2009 Robert Osfield 
 *
 * This library is open source and may be redistributed and/or modified under  
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or 
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * OpenSceneGraph Public License for more details.
*/

/**
 * \brief    Shader generator framework.
 * \author   Maciej Krol
 */

#ifndef MY_SHADER_STATE_
#define MY_SHADER_STATE_ 1

#include <osgUtil/Export>
#include <osg/NodeVisitor>
#include <osg/State>



class MyShaderGenCache : public osg::Referenced
{
public:
    enum StateMask
    {
        BLEND = 1,
        LIGHTING = 2,
        FOG = 4,
        DIFFUSE_MAP = 8, //< Texture in unit 0
        ATTRIB_MAP = 16  //< coord in unit 1
    };

    typedef std::map<int, osg::ref_ptr<osg::StateSet> > StateSetMap;

    MyShaderGenCache() {};

    void setStateSet(int stateMask, osg::StateSet *program);
    osg::StateSet *getStateSet(int stateMask) const;
    osg::StateSet *getOrCreateStateSet(int stateMask);

protected:
    osg::StateSet *createStateSet(int stateMask) const;
    mutable OpenThreads::Mutex _mutex;
    StateSetMap _stateSetMap;

};

class  MyShaderGenVisitor : public osg::NodeVisitor
{
public:
    MyShaderGenVisitor();
    MyShaderGenVisitor(MyShaderGenCache *stateCache);

    void setStateCache(MyShaderGenCache *stateCache) { _stateCache = stateCache; }
    MyShaderGenCache *getStateCache() const { return _stateCache.get(); }

    /// Top level state set applied as the first one.
    void setRootStateSet(osg::StateSet *stateSet);
    osg::StateSet *getRootStateSet() const { return _rootStateSet.get(); }

    void apply(osg::Node &node);
    void apply(osg::Geode &geode);

    void reset();

protected:
    void update(osg::Drawable *drawable);

    osg::ref_ptr<MyShaderGenCache> _stateCache;
    osg::ref_ptr<osg::State> _state;
    osg::ref_ptr<osg::StateSet> _rootStateSet;
};



#endif
