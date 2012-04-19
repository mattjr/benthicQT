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

#include "MyShaderGen.h"
#include <osg/Geode>
#include <osg/Geometry> // for MyShaderGenVisitor::update
#include <osg/Fog>
#include <sstream>


/// State extended by mode/attribute accessors
class StateEx : public osg::State
{
public:
    StateEx() : State() {}

    osg::StateAttribute::GLModeValue getMode(osg::StateAttribute::GLMode mode,
        osg::StateAttribute::GLModeValue def = osg::StateAttribute::INHERIT) const
    {
        return getMode(_modeMap, mode, def);
    }

    osg::StateAttribute *getAttribute(osg::StateAttribute::Type type, unsigned int member = 0) const
    {
        return getAttribute(_attributeMap, type, member);
    }

    osg::StateAttribute::GLModeValue getTextureMode(unsigned int unit,
        osg::StateAttribute::GLMode mode,
        osg::StateAttribute::GLModeValue def = osg::StateAttribute::INHERIT) const
    {
        return unit < _textureModeMapList.size() ? getMode(_textureModeMapList[unit], mode, def) : def;
    }

    osg::StateAttribute *getTextureAttribute(unsigned int unit, osg::StateAttribute::Type type) const
    {
        return unit < _textureAttributeMapList.size() ? getAttribute(_textureAttributeMapList[unit], type, 0) : 0;
    }

    osg::Uniform *getUniform(const std::string& name) const
    {
        UniformMap::const_iterator it = _uniformMap.find(name);
        return it != _uniformMap.end() ? 
            const_cast<osg::Uniform *>(it->second.uniformVec.back().first) : 0;
    }

protected:

    osg::StateAttribute::GLModeValue getMode(const ModeMap &modeMap,
        osg::StateAttribute::GLMode mode, 
        osg::StateAttribute::GLModeValue def = osg::StateAttribute::INHERIT) const
    {
        ModeMap::const_iterator it = modeMap.find(mode);
        return (it != modeMap.end() && it->second.valueVec.size()) ? it->second.valueVec.back() : def;
    }

    osg::StateAttribute *getAttribute(const AttributeMap &attributeMap,
        osg::StateAttribute::Type type, unsigned int member = 0) const
    {
        AttributeMap::const_iterator it = attributeMap.find(std::make_pair(type, member));
        return (it != attributeMap.end() && it->second.attributeVec.size()) ? 
            const_cast<osg::StateAttribute*>(it->second.attributeVec.back().first) : 0;
    }
};



void MyShaderGenCache::setStateSet(int stateMask, osg::StateSet *stateSet)
{
    OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
    _stateSetMap[stateMask] = stateSet;
}

osg::StateSet *MyShaderGenCache::getStateSet(int stateMask) const
{
    OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
    StateSetMap::const_iterator it = _stateSetMap.find(stateMask);
    return (it != _stateSetMap.end()) ? it->second.get() : 0;
}

osg::StateSet *MyShaderGenCache::getOrCreateStateSet(int stateMask)
{
    OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
    StateSetMap::iterator it = _stateSetMap.find(stateMask);
    if (it == _stateSetMap.end())
    {
        osg::StateSet *stateSet = createStateSet(stateMask);
        _stateSetMap.insert(it, StateSetMap::value_type(stateMask, stateSet));
        return stateSet;
    }
    return it->second.get();
}

osg::StateSet *MyShaderGenCache::createStateSet(int stateMask) const
{
    osg::StateSet *stateSet = new osg::StateSet;
    osg::Program *program = new osg::Program;
    stateSet->setAttribute(program);

    std::ostringstream vert;
    std::ostringstream frag;

    // write varyings
   if ((stateMask & LIGHTING))
    {
        vert << "varying vec3 normalDir;\n";
        vert << "varying vec3 lightDir;\n";
    }
    vert << "varying float height;\n";

    if (stateMask & (LIGHTING | FOG))
    {
        vert << "varying vec3 viewDir;\n";
    }
        if (stateMask & (ATTRIB_MAP))
            frag << "#version 120\n";
    // copy varying to fragment shader
    frag << vert.str();

    // write uniforms and attributes
    int unit = 0;
    if (stateMask & DIFFUSE_MAP)
    {
        osg::Uniform *diffuseMap = new osg::Uniform("diffuseMap", unit++);
        stateSet->addUniform(diffuseMap);
        frag << "uniform sampler2D diffuseMap;\n";
    }

    if(stateMask & (ATTRIB_MAP)){
        frag << "uniform sampler2D attribSampler;\n";
        frag << "uniform float texScale;\n";

        stateSet->addUniform( new osg::Uniform("attribSampler", TEXUNIT_ATTRIB) );
    }

    frag << "uniform int shaderOut;\n";

    /*if (stateMask & NORMAL_MAP)
    {
        osg::Uniform *normalMap = new osg::Uniform("normalMap", unit++);
        stateSet->addUniform(normalMap);
        frag << "uniform sampler2D normalMap;\n";
        program->addBindAttribLocation("tangent", 6);
        vert << "attribute vec3 tangent;\n";
    }*/

    vert << "\n"\
        "void main()\n"\
        "{\n"\
        "  gl_Position = ftransform();\n";

    if (stateMask & (DIFFUSE_MAP ))
    {
        vert << "  gl_TexCoord[0] = gl_MultiTexCoord0;\n";
    }

    if (stateMask & (ATTRIB_MAP))
    {
        vert << "  gl_TexCoord[1] = gl_MultiTexCoord1;\n";
    }

    vert << "  height = gl_Vertex.z;\n";

   /* if (stateMask & NORMAL_MAP)
    {
        vert << 
            "  vec3 n = gl_NormalMatrix * gl_Normal;\n"\
            "  vec3 t = gl_NormalMatrix * tangent;\n"\
            "  vec3 b = cross(n, t);\n"\
            "  vec3 dir = -vec3(gl_ModelViewMatrix * gl_Vertex);\n"\
            "  viewDir.x = dot(dir, t);\n"\
            "  viewDir.y = dot(dir, b);\n"\
            "  viewDir.z = dot(dir, n);\n"\
            "  vec4 lpos = gl_LightSource[0].position;\n"\
            "  if (lpos.w == 0.0)\n"\
            "    dir = lpos.xyz;\n"\
            "  else\n"\
            "    dir += lpos.xyz;\n"\
            "  lightDir.x = dot(dir, t);\n"\
            "  lightDir.y = dot(dir, b);\n"\
            "  lightDir.z = dot(dir, n);\n";
    }
    else*/ if (stateMask & LIGHTING)
    {
        vert << 
            "  normalDir = gl_NormalMatrix * gl_Normal;\n"\
            "  vec3 dir = -vec3(gl_ModelViewMatrix * gl_Vertex);\n"\
            "  viewDir = dir;\n"\
            "  vec4 lpos = gl_LightSource[0].position;\n"\
            "  if (lpos.w == 0.0)\n"\
            "    lightDir = lpos.xyz;\n"\
            "  else\n"\
            "    lightDir = lpos.xyz + dir;\n";
    }
    else if (stateMask & FOG)
    {
        vert << 
            "  viewDir = -vec3(gl_ModelViewMatrix * gl_Vertex);\n"\
            "  gl_FrontColor = gl_Color;\n";
    }
    else
    {
        vert << "  gl_FrontColor = gl_Color;\n";
    }
    
    vert << "}\n";
        if (stateMask & ATTRIB_MAP)
            {
                frag <<
                        "uniform int colormap;\n"\
                        "uniform vec2 valrange;\n"\
                        "uniform int dataused;\n"\
                        "vec2 addrTranslation_1DtoRECT(float address1D, float texSize) \n"\
                        "{\n"\
                        "   float CONV_CONST = 1.0 / texSize; \n"\
                        "   float normAddr1D = address1D * CONV_CONST;  \n"
                        "   return vec2( fract(normAddr1D)*texSize,normAddr1D); ;\n"
                        "}\n"
                        "vec4 doMap(float val,const vec3 iMap[9],int iMapSize){\n"\
                        " float x = clamp(val,0.0,1.0) * (iMapSize - 1);\n"
                        " float x0 = floor(x);\n"\
                        "int i = int(x0);\n"\
                        " if (i == iMapSize - 1)\n"\
                        "{\n"\
                        "return vec4(iMap[i].x,iMap[i].y,iMap[i].z,1.0);\n"\
                        "  }\n"\
                        " float dx = x - x0;\n"\
                        "vec3 tmp=iMap[i] * (1.0 - dx) + iMap[i + 1] * dx;\n"\
                        "return vec4(tmp.x,tmp.y,tmp.z,1.0);\n"\
                        "}\n"\
                        "vec4 doMap17(float val,const vec3 iMap[17],int iMapSize){\n"\
                        " float x = clamp(val,0.0,1.0) * (iMapSize - 1);\n"
                        " float x0 = floor(x);\n"\
                        "int i = int(x0);\n"\
                        " if (i == iMapSize - 1)\n"\
                        "{\n"\
                        "return vec4(iMap[i].x,iMap[i].y,iMap[i].z,1.0);\n"\
                        "  }\n"\
                        " float dx = x - x0;\n"\
                        "vec3 tmp=iMap[i] * (1.0 - dx) + iMap[i + 1] * dx;\n"\
                        "return vec4(tmp.x,tmp.y,tmp.z,1.0);\n"\
                        "}\n"\
                        "vec4 jet(float val){\n"\
                        " const vec3 keys[9]=vec3[9]("\
                        "vec3(0., 0., .5),\n"\
                        " vec3(0., 0., 1.),\n"\
                        " vec3(0., .5, 1.), \n"\
                        "vec3(0., 1., 1.),\n"\
                        "vec3(.5, 1., .5),\n"\
                        "vec3(1., 1., 0.), \n"\
                        "vec3(1., .5, 0.), \n"\
                        "vec3(1., .0, 0.), \n"\
                        "vec3(.5, .0, 0.) ); \n"\
                        "return doMap(val, keys, 9);\n"\
                        "}\n"\
                        "vec4 hot(float val){\n"\
                        " const vec3 keys[9]=vec3[9]("\
                        "vec3(0.     , 0.     , 0.),\n"\
                        " vec3(1. / 3., 0.     , 0.),\n"\
                        " vec3(2. / 3., 0.     , 0.), \n"\
                        "vec3(1.     , 0.     , 0.),\n"\
                        "vec3(1.     , 1. / 3., 0.),\n"\
                        "vec3(1.     , 2. / 3., 0.), \n"\
                        "vec3(1.     , 1.     , 0.), \n"\
                        "vec3(1.     , 1.     , .5), \n"\
                        "vec3(1.     , 1.     , 1.) ); \n"\
                        "return doMap(val, keys, 9);\n"\
                        "}\n"\
                         "vec4 grey(float val){\n"\
                        " float x = clamp(val,0.0,1.0);\n"\
                        "return vec4(x,x,x,1.0);\n"\
                        "}\n"\
                        "vec4 spring(float val){\n"\
                       " float x = clamp(val,0.0,1.0);\n"\
                       "return vec4(1.0,x,1.0-x,1.0);\n"\
                       "}\n"\
                        "vec4 summer(float val){\n"\
                       " float x = clamp(val,0.0,1.0);\n"\
                       "return vec4(x,(1.0+x)/2.0,0.4,1.0);\n"\
                       "}\n"\
                        "vec4 winter(float val){\n"\
                       " float x = clamp(val,0.0,1.0);\n"\
                       "return vec4(0.0,1.0-x,0.5,1.0);\n"\
                       "}\n"\
                        "vec4 cool(float val){\n"\
                       " float x = clamp(val,0.0,1.0);\n"\
                       "return vec4(x,1.0-x,1.0,1.0);\n"\
                       "}\n"\
                        "vec4 bone(float val){\n"\
                        " const vec3 keys[9]=vec3[9]("\
                        "vec3(0.     , 0.     , 0.),\n"\
                        " vec3(0.3194, 0.3194, 0.4444),\n"\
                        " vec3(0.6528, 0.7778, 0.7778),\n"\
                        "vec3(1.    , 1.    , 1.    ),\n"\
                        "vec3(0.     , 0.     , 0.),\n"\
                        "vec3(0.     , 0.     , 0.),\n"\
                        "vec3(0.     , 0.     , 0.),\n"\
                        "vec3(0.     , 0.     , 0.),\n"\
                        "vec3(0.     , 0.     , 0.) ); \n"\
                        "return doMap(val, keys, 4);\n"\
                        "}\n"\
                        "vec4 rainbow(float val){\n"\
                        " const vec3 keys[9]=vec3[9]("\
                        "vec3(1., 0., 0.),\n"\
                        " vec3(1., 1., 0.),\n"\
                        " vec3(0., 1., 0.),\n"\
                        "vec3(0., 1., 1.),\n"\
                        "vec3(0., 0., 1.),\n"\
                        "vec3(1., 0., 1.),\n"\
                        "vec3(1., 0., 0.),\n"\
                        "vec3(0.     , 0.     , 0.),\n"\
                        "vec3(0.     , 0.     , 0.) ); \n"\
                        "return doMap(val, keys, 7);\n"\
                        "}\n"\
                        "vec4 pink(float val){\n"\
                        " const vec3 keys[17]=vec3[17]("\
                        "vec3(0.    , 0.    , 0.    ),\n"\
                                "vec3(0.2955, 0.1782, 0.1782),\n"\
                                "vec3(0.4303, 0.2722, 0.2722),\n"\
                                "vec3(0.5320, 0.3412, 0.3412),\n"\
                                "vec3(0.6172, 0.3984, 0.3984),\n"\
                                "vec3(0.6920, 0.4484, 0.4484),\n"\
                                "vec3(0.7594, 0.4933, 0.4933),\n"\
                                "vec3(0.7868, 0.5842, 0.5345),\n"\
                                "vec3(0.8133, 0.6627, 0.5727),\n"\
                                "vec3(0.8389, 0.7328, 0.6086),\n"\
                                "vec3(0.8637, 0.7968, 0.6424),\n"\
                                "vec3(0.8879, 0.8560, 0.6746),\n"\
                                "vec3(0.9114, 0.9114, 0.7052),\n"\
                                "vec3(0.9344, 0.9344, 0.7893),\n"\
                                "vec3(0.9567, 0.9567, 0.8653),\n"\
                                "vec3(0.9786, 0.9786, 0.9351),\n"\
                                "vec3(1.    , 1.    , 1.    ));\n"\
                        "return doMap17(val, keys, 17);\n"\
                        "}\n"\
                        "vec4 colormapGetColor(float val,int map){\n"\
                        "if(map==0)\n"\
                        "return jet(val);\n"\
                        "else if(map==1)\n"\
                        "return rainbow(val);\n"\
                        "else if(map==2)\n"\
                        "return hot(val);\n"\
                        "else if(map==3)\n"\
                        "return bone(val);\n"\
                        "else if(map==4)\n"\
                        "return pink(val);\n"\
                        "else if(map==5)\n"\
                        "return spring(val);\n"\
                        "else if(map==6)\n"\
                        "return summer(val);\n"\
                        "else if(map==7)\n"\
                        "return winter(val);\n"\
                        "else if(map==8)\n"\
                        "return cool(val);\n"\
                        "else \n"\
                        "return grey(val);\n"\
                        "}\n";

            }
    frag << "\n"\
        "void main()\n"\
        "{\n";

   /* if (stateMask & DIFFUSE_MAP)
    {
        frag << "  vec4 base = texture2D(diffuseMap, gl_TexCoord[0].st);\n";
    }*/
    /*else
    {
        frag << "  vec4 base = vec4(1.0);\n";
    }*/

   /* if (stateMask & NORMAL_MAP)
    {
        frag << "  vec3 normalDir = texture2D(normalMap, gl_TexCoord[0].st).xyz*2.0-1.0;\n";
    }    if (stateMask & FOG)

*/


    if (stateMask & (LIGHTING))
    {
        frag <<
            "  vec4 color;\n"\
            "  vec4 base;\n"\
            "  float val;\n"\
            "  if(shaderOut == 1) {\n";
        if(stateMask & (ATTRIB_MAP)){
                frag << "if(dataused==0){\n";
                frag << "float range= valrange.y-valrange.x;\n";
                frag << "val = (height-valrange.x)/range;\n";
                frag << "base = colormapGetColor(val,colormap);";
                frag << "}\n";
                frag << "else if(dataused==1){\n";
                frag << "vec2 twoDIdx = addrTranslation_1DtoRECT(gl_TexCoord[1].t,texScale);\n ";
                frag << "   vec2 mult=(2.0*twoDIdx- 1.0)/(2.0*texScale);\n";
                frag << "base = texture2D(attribSampler,mult);\n";
                frag << "base = colormapGetColor(base.x,colormap);\n";
                frag << "}\n";

        }
         //  frag << "   vec4 base = colormapGetColor(gl_TexCoord[0].s,colormap);\n";
        else
            frag << "  vec4 base = gl_Color;";

        frag <<
            "   vec3 nd = normalize(normalDir);\n"\
            "   vec3 ld = normalize(lightDir);\n"\
            "   vec3 vd = normalize(viewDir);\n"\
            "   color = gl_FrontLightModelProduct.sceneColor;\n"\
            "   color += gl_FrontLightProduct[0].ambient;\n"\
            "   float diff = max(dot(ld, nd), 0.0);\n"\
            "   color += gl_FrontLightProduct[0].diffuse * diff;\n"\
            "   color *= base;\n"\
            "   if (diff > 0.0)\n"\
            "   {\n"\
            "        vec3 halfDir = normalize(ld+vd);\n"\
            "        color.rgb += base.a * gl_FrontLightProduct[0].specular.rgb * \n"\
            "        pow(max(dot(halfDir, nd), 0.0), gl_FrontMaterial.shininess);\n"\
            "   }\n"\
            "   color.rgb *= (gl_TexCoord[1].s);\n"\
            "  }else\n";

    }

    {
        frag << "{\n";
        if (stateMask & DIFFUSE_MAP)
            frag << "  vec4 base = texture2D(diffuseMap, gl_TexCoord[0].st);\n";
        else
            frag << "  vec4 base = vec4(1.0);\n";
        frag << "  color = base;\n";
        frag << "}\n";

    }

    if (!(stateMask & LIGHTING))
    {
        frag << "  color *= gl_Color;\n";
    }

    if (stateMask & FOG)
    {
        frag << 
            "  float d2 = dot(viewDir, viewDir);//gl_FragCoord.z/gl_FragCoord.w;\n"\
            "  float f = exp2(-1.442695*gl_Fog.density*gl_Fog.density*d2);\n"\
            "  color.rgb = mix(gl_Fog.color.rgb, color.rgb, clamp(f, 0.0, 1.0));\n";
    }
    
    frag << "  gl_FragColor = color;\n";
    frag << "}\n";

    std::string vertstr = vert.str();
    std::string fragstr = frag.str();

    OSG_DEBUG << "MyShaderGenCache Vertex shader:\n" << vertstr << std::endl;
    OSG_DEBUG << "MyShaderGenCache Fragment shader:\n" << fragstr << std::endl;

    program->addShader(new osg::Shader(osg::Shader::VERTEX, vertstr));
    program->addShader(new osg::Shader(osg::Shader::FRAGMENT, fragstr));


    return stateSet;
}

MyShaderGenVisitor::MyShaderGenVisitor() :
    NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN),
    _stateCache(new MyShaderGenCache),
    _state(new StateEx)
{
}

MyShaderGenVisitor::MyShaderGenVisitor(MyShaderGenCache *stateCache) :
    NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN),
    _stateCache(stateCache),
    _state(new StateEx)
{
}

void MyShaderGenVisitor::setRootStateSet(osg::StateSet *stateSet)
{
    if (_rootStateSet.valid())
        _state->removeStateSet(0);
    _rootStateSet = stateSet;
    if (_rootStateSet.valid())
        _state->pushStateSet(_rootStateSet.get());
}

void MyShaderGenVisitor::reset()
{
    //_state->popAllStateSets();
    if (_rootStateSet.valid())
        _state->pushStateSet(_rootStateSet.get());
}

void MyShaderGenVisitor::apply(osg::Node &node)
{
    osg::StateSet *stateSet = node.getStateSet();

    if (stateSet)
        _state->pushStateSet(stateSet);

    traverse(node);

    if (stateSet)
        _state->popStateSet();
}

void MyShaderGenVisitor::apply(osg::Geode &geode)
{
    osg::StateSet *stateSet = geode.getStateSet();
    if (stateSet)
        _state->pushStateSet(stateSet);

    for (unsigned int i=0; i<geode.getNumDrawables(); ++i)
    {
        osg::Drawable *drawable = geode.getDrawable(i);
        osg::StateSet *ss = drawable->getStateSet();
        if (ss)
            _state->pushStateSet(ss);

        update(drawable);

        if (ss)
            _state->popStateSet();
    }

    if (stateSet)
        _state->popStateSet();
}

void MyShaderGenVisitor::update(osg::Drawable *drawable)
{
    // update only geometry due to compatibility issues with user defined drawables
    osg::Geometry *geometry = drawable->asGeometry();
#if 0
    if (!geometry)
        return;
#endif

    StateEx *state = static_cast<StateEx *>(_state.get());
    // skip nodes without state sets
    if (state->getStateSetStackSize() == (_rootStateSet.valid() ? 1u : 0u))
        return;

    // skip state sets with already attached programs
    if (state->getAttribute(osg::StateAttribute::PROGRAM))
        return;

    int stateMask = 0;
    //if (state->getMode(GL_BLEND) & osg::StateAttribute::ON)
    //    stateMask |= MyShaderGen::BLEND;
    //if (state->getMode(GL_LIGHTING) & osg::StateAttribute::ON)
        stateMask |= MyShaderGenCache::LIGHTING;
    if (state->getMode(GL_FOG) & osg::StateAttribute::ON)
        stateMask |= MyShaderGenCache::FOG;
    if (state->getTextureAttribute(0, osg::StateAttribute::TEXTURE))
        stateMask |= MyShaderGenCache::DIFFUSE_MAP;

    //if (state->getTextureAttribute(1, osg::StateAttribute::TEXTURE) && geometry!=0 &&
   //     geometry->getVertexAttribArray(6)) //tangent
        stateMask |= MyShaderGenCache::ATTRIB_MAP;

    // Get program and uniforms for accumulated state.
    osg::StateSet *progss = _stateCache->getOrCreateStateSet(stateMask);
    // Set program and uniforms to the last state set.
    osg::StateSet *ss = const_cast<osg::StateSet *>(state->getStateSetStack().back());
    ss->setAttribute(progss->getAttribute(osg::StateAttribute::PROGRAM));
    ss->setUniformList(progss->getUniformList());
    
    // remove any modes that won't be appropriate when using shaders
    if ((stateMask&MyShaderGenCache::LIGHTING)!=0)
    {
        ss->removeMode(GL_LIGHTING);
        ss->removeMode(GL_LIGHT0);
    }
    if ((stateMask&MyShaderGenCache::FOG)!=0)
    {
        ss->removeMode(GL_FOG);
    }
    if ((stateMask&MyShaderGenCache::DIFFUSE_MAP)!=0) ss->removeTextureMode(0, GL_TEXTURE_2D);
    //if ((stateMask&MyShaderGenCache::NORMAL_MAP)!=0) ss->removeTextureMode(1, GL_TEXTURE_2D);
}
