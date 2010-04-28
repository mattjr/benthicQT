/* Copyright 2010 NVIDIA Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Developed by Mustard Seed Software, LLC 
 * http://mseedsoft.com
 */

#include "WaterSurfaceGeom.h"
#include <osg/Shape>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/PolygonMode>
#include <osg/Material>
#include "VertexHeightUpdater.h"

namespace ews {
    namespace app {
        namespace drawable {
            using ews::app::model::WaveMedium;
            using osg::ref_ptr;
            using namespace osg;
            
            const Vec3 WATER_COLOR(0.1f, 0.1f, 1.f);
            const float WATER_OPACITY = 1.f;

            WaterSurfaceGeom::WaterSurfaceGeom(WaveMedium& settings) : 
            DrawableQtAdapter(), _settings(settings) {

                QObject::connect(&_settings, SIGNAL(sizeChanged(int,int)), this, SLOT(updateWaterGeometry()));
                QObject::connect(&_settings, SIGNAL(resolutionChanged(int)), this, SLOT(updateWaterGeometry()));
                
                osg::StateSet* state = getOrCreateStateSet(); 
                osg::ref_ptr<Material> mat = new Material; 
                mat->setAmbient(Material::FRONT, Vec4(WATER_COLOR, WATER_OPACITY));
                mat->setDiffuse(Material::FRONT, Vec4(WATER_COLOR, WATER_OPACITY)); 
                mat->setSpecular(Material::FRONT, Vec4(0.f, 0.f, 0.f, WATER_OPACITY)); 
                mat->setShininess(Material::FRONT, 30.f); 
                mat->setColorMode(osg::Material::AMBIENT_AND_DIFFUSE);
                state->setAttribute(mat.get());
                state->setMode(GL_BLEND, osg::StateAttribute::ON); // Activate blending (for transparency)
                
                updateWaterGeometry();
            }

            
            void WaterSurfaceGeom::updateWaterGeometry() {

                removeChildren(0, getNumChildren());
                
                unsigned int rez = _settings.getLatticeDivisionsPerCentimeter();
                unsigned int numX = _settings.getWidth() * rez;
                unsigned int numY = _settings.getLength() * rez;
                unsigned int totalVerts = numX * numY;
                
                ref_ptr<Geode> geode = new Geode;
                if(false) {
                    // Turn on wireframe mode.
                    ref_ptr<osg::StateSet> state = geode->getOrCreateStateSet();
                
                    // Create a PolygonMode attribute 
                    ref_ptr<osg::PolygonMode> pm = new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE); 
                    // Force wireframe rendering. 
                    state->setAttributeAndModes(pm.get(), osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE); 
                }
                
                addChild(geode.get());

                ref_ptr<Geometry> geom = new Geometry;
                geode->addDrawable(geom.get());
                
                // Code below adapted from demo file osgparametric.cpp
                ref_ptr<Vec3Array> vertices = new Vec3Array( totalVerts );
                ref_ptr<Vec2Array> texCoords = new Vec2Array( totalVerts );
                
                float dx = 1/(float) rez;
                float dy = dx;
                Vec3 row(0.0f, 0.0f,0.0f);
                Vec2 rowTex(0, 0);
                
                // Concise way of allocating grid vertices with texture coordinates.
                unsigned int vertIndex = 0;
                for(unsigned int y = 0; y < numY; ++y) {
                    Vec3 column = row;
                    rowTex.y() = y/(float)(numY - 1);
                    Vec2 colTex = rowTex;
                    for(unsigned int x = 0 ; x < numX; ++x) {
                        (*vertices)[vertIndex] = column;
                        
                        colTex.x() = x/(float)(numX - 1);
                        (*texCoords)[vertIndex] = colTex;

                        column.x() += dx;
                        vertIndex++;
                    }        
                    row.y() += dy;
                }
                
                // Add the vertices to the geometry, and tag them as 
                // being stored as a VBO.
                geom->setVertexArray(vertices.get());
                ref_ptr<VertexBufferObject> vbObject = new VertexBufferObject;
                vertices->setVertexBufferObject(vbObject.get());
                    
                // Set the texture coordinates.
                geom->setTexCoordArray(TEX_ID, texCoords.get());
                
                // Set the default normal (adjusted by vertex shader).
                ref_ptr<Vec3Array> n = new Vec3Array; 
                geom->setNormalArray(n.get()); 
                geom->setNormalBinding(Geometry::BIND_OVERALL); 
                n->push_back(Vec3(0, 0, 1));
                
                
                // Create the vertex mesh as triangle strips, stored as an EBO
                ref_ptr<ElementBufferObject> ebo = new ElementBufferObject;
                for(unsigned int y = 0; y < numY-1; y++) {
                    unsigned int elementIndex = 0;
                    ref_ptr<DrawElementsUInt> elements = new DrawElementsUInt(GL_TRIANGLE_STRIP, numX * 2);
                    unsigned int index = y * numX;
                    for(unsigned int x = 0; x < numX; x++) {
                        (*elements)[elementIndex++] = index + numX;
                        (*elements)[elementIndex++] = index++;
                    }
                    geom->addPrimitiveSet(elements.get()); 
                    
                    if (ebo) elements->setElementBufferObject(ebo.get());   
                }
                
                // geom->setUseVertexBufferObjects(true);
                
                // Register the delegate responsible for updating height and 
                // doing shading.
                setUpdateCallback(new VertexHeightUpdater(_settings, geom));
            }
 
        }
    }
}
