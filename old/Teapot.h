/* OpenSceneGraph example, osgteapot.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 */

#include <osg/Geode>
#include <osg/TexGen>
#include <osg/Texture2D>

/* Copyright (c) Mark J. Kilgard, 1994. */

/*
 (c) Copyright 1993, Silicon Graphics, Inc.
 
 ALL RIGHTS RESERVED
 
 Permission to use, copy, modify, and distribute this software
 for any purpose and without fee is hereby granted, provided
 that the above copyright notice appear in all copies and that
 both the copyright notice and this permission notice appear in
 supporting documentation, and that the name of Silicon
 Graphics, Inc. not be used in advertising or publicity
 pertaining to distribution of the software without specific,
 written prior permission.
 
 THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU
 "AS-IS" AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR
 OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
 MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  IN NO
 EVENT SHALL SILICON GRAPHICS, INC.  BE LIABLE TO YOU OR ANYONE
 ELSE FOR ANY DIRECT, SPECIAL, INCIDENTAL, INDIRECT OR
 CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER,
 INCLUDING WITHOUT LIMITATION, LOSS OF PROFIT, LOSS OF USE,
 SAVINGS OR REVENUE, OR THE CLAIMS OF THIRD PARTIES, WHETHER OR
 NOT SILICON GRAPHICS, INC.  HAS BEEN ADVISED OF THE POSSIBILITY
 OF SUCH LOSS, HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 ARISING OUT OF OR IN CONNECTION WITH THE POSSESSION, USE OR
 PERFORMANCE OF THIS SOFTWARE.
 
 US Government Users Restricted Rights
 
 Use, duplication, or disclosure by the Government is subject to
 restrictions set forth in FAR 52.227.19(c)(2) or subparagraph
 (c)(1)(ii) of the Rights in Technical Data and Computer
 Software clause at DFARS 252.227-7013 and/or in similar or
 successor clauses in the FAR or the DOD or NASA FAR
 Supplement.  Unpublished-- rights reserved under the copyright
 laws of the United States.  Contractor/manufacturer is Silicon
 Graphics, Inc., 2011 N.  Shoreline Blvd., Mountain View, CA
 94039-7311.
 
 OpenGL(TM) is a trademark of Silicon Graphics, Inc.
 */

namespace ews {
    namespace app {
        namespace drawable {
            
            // The classic OpenGL teapot... taken form glut-3.7/lib/glut/glut_teapot.c
            
            /* Rim, body, lid, and bottom data must be reflected in x and
             y; handle and spout data across the y axis only.  */
            
            static int patchdata[][16] =
            {
                /* rim */
                {102, 103, 104, 105, 4, 5, 6, 7, 8, 9, 10, 11,
                    12, 13, 14, 15},
                /* body */
                {12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
                    24, 25, 26, 27},
                {24, 25, 26, 27, 29, 30, 31, 32, 33, 34, 35, 36,
                    37, 38, 39, 40},
                /* lid */
                {96, 96, 96, 96, 97, 98, 99, 100, 101, 101, 101,
                    101, 0, 1, 2, 3,},
                {0, 1, 2, 3, 106, 107, 108, 109, 110, 111, 112,
                    113, 114, 115, 116, 117},
                /* bottom */
                {118, 118, 118, 118, 124, 122, 119, 121, 123, 126,
                    125, 120, 40, 39, 38, 37},
                /* handle */
                {41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52,
                    53, 54, 55, 56},
                {53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64,
                    28, 65, 66, 67},
                /* spout */
                {68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
                    80, 81, 82, 83},
                {80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91,
                    92, 93, 94, 95}
            };
            /* *INDENT-OFF* */
            
            static float cpdata[][3] =
            {
                {0.2f, 0.f, 2.7f}, {0.2f, -0.112f, 2.7f}, {0.112f, -0.2f, 2.7f}, {0.f,
                -0.2f, 2.7f}, {1.3375f, 0.f, 2.53125f}, {1.3375f, -0.749f, 2.53125f},
                {0.749f, -1.3375f, 2.53125f}, {0.f, -1.3375f, 2.53125f}, {1.4375f,
                0.f, 2.53125f}, {1.4375f, -0.805f, 2.53125f}, {0.805f, -1.4375f,
                2.53125f}, {0.f, -1.4375f, 2.53125f}, {1.5f, 0.f, 2.4f}, {1.5f, -0.84f,
                2.4f}, {0.84f, -1.5f, 2.4f}, {0.f, -1.5f, 2.4f}, {1.75f, 0.f, 1.875f},
                {1.75f, -0.98f, 1.875f}, {0.98f, -1.75f, 1.875f}, {0.f, -1.75f,
                1.875f}, {2.f, 0.f, 1.35f}, {2.f, -1.12f, 1.35f}, {1.12f, -2.f, 1.35f},
                {0.f, -2.f, 1.35f}, {2.f, 0.f, 0.9f}, {2.f, -1.12f, 0.9f}, {1.12f, -2.f,
                0.9f}, {0.f, -2.f, 0.9f}, {-2.f, 0.f, 0.9f}, {2.f, 0.f, 0.45f}, {2.f, -1.12f,
                0.45f}, {1.12f, -2.f, 0.45f}, {0.f, -2.f, 0.45f}, {1.5f, 0.f, 0.225f},
                {1.5f, -0.84f, 0.225f}, {0.84f, -1.5f, 0.225f}, {0.f, -1.5f, 0.225f},
                {1.5f, 0.f, 0.15f}, {1.5f, -0.84f, 0.15f}, {0.84f, -1.5f, 0.15f}, {0.f,
                -1.5f, 0.15f}, {-1.6f, 0.f, 2.025f}, {-1.6f, -0.3f, 2.025f}, {-1.5f,
                -0.3f, 2.25f}, {-1.5f, 0.f, 2.25f}, {-2.3f, 0.f, 2.025f}, {-2.3f, -0.3f,
                2.025f}, {-2.5f, -0.3f, 2.25f}, {-2.5f, 0.f, 2.25f}, {-2.7f, 0.f,
                2.025f}, {-2.7f, -0.3f, 2.025f}, {-3.f, -0.3f, 2.25f}, {-3.f, 0.f,
                2.25f}, {-2.7f, 0.f, 1.8f}, {-2.7f, -0.3f, 1.8f}, {-3.f, -0.3f, 1.8f},
                {-3.f, 0.f, 1.8f}, {-2.7f, 0.f, 1.575f}, {-2.7f, -0.3f, 1.575f}, {-3.f,
                -0.3f, 1.35f}, {-3.f, 0.f, 1.35f}, {-2.5f, 0.f, 1.125f}, {-2.5f, -0.3f,
                1.125f}, {-2.65f, -0.3f, 0.9375f}, {-2.65f, 0.f, 0.9375f}, {-2.f,
                -0.3f, 0.9f}, {-1.9f, -0.3f, 0.6f}, {-1.9f, 0.f, 0.6f}, {1.7f, 0.f,
                1.425f}, {1.7f, -0.66f, 1.425f}, {1.7f, -0.66f, 0.6f}, {1.7f, 0.f,
                0.6f}, {2.6f, 0.f, 1.425f}, {2.6f, -0.66f, 1.425f}, {3.1f, -0.66f,
                0.825f}, {3.1f, 0.f, 0.825f}, {2.3f, 0.f, 2.1f}, {2.3f, -0.25f, 2.1f},
                {2.4f, -0.25f, 2.025f}, {2.4f, 0.f, 2.025f}, {2.7f, 0.f, 2.4f}, {2.7f,
                -0.25f, 2.4f}, {3.3f, -0.25f, 2.4f}, {3.3f, 0.f, 2.4f}, {2.8f, 0.f,
                2.475f}, {2.8f, -0.25f, 2.475f}, {3.525f, -0.25f, 2.49375f},
                {3.525f, 0.f, 2.49375f}, {2.9f, 0.f, 2.475f}, {2.9f, -0.15f, 2.475f},
                {3.45f, -0.15f, 2.5125f}, {3.45f, 0.f, 2.5125f}, {2.8f, 0.f, 2.4f},
                {2.8f, -0.15f, 2.4f}, {3.2f, -0.15f, 2.4f}, {3.2f, 0.f, 2.4f}, {0.f, 0.f,
                3.15f}, {0.8f, 0.f, 3.15f}, {0.8f, -0.45f, 3.15f}, {0.45f, -0.8f,
                3.15f}, {0.f, -0.8f, 3.15f}, {0.f, 0.f, 2.85f}, {1.4f, 0.f, 2.4f}, {1.4f,
                -0.784f, 2.4f}, {0.784f, -1.4f, 2.4f}, {0.f, -1.4f, 2.4f}, {0.4f, 0.f,
                2.55f}, {0.4f, -0.224f, 2.55f}, {0.224f, -0.4f, 2.55f}, {0.f, -0.4f,
                2.55f}, {1.3f, 0.f, 2.55f}, {1.3f, -0.728f, 2.55f}, {0.728f, -1.3f,
                2.55f}, {0.f, -1.3f, 2.55f}, {1.3f, 0.f, 2.4f}, {1.3f, -0.728f, 2.4f},
                {0.728f, -1.3f, 2.4f}, {0.f, -1.3f, 2.4f}, {0.f, 0.f, 0.f}, {1.425f,
                -0.798f, 0.f}, {1.5f, 0.f, 0.075f}, {1.425f, 0.f, 0.f}, {0.798f, -1.425f,
                0.f}, {0.f, -1.5f, 0.075f}, {0.f, -1.425f, 0.f}, {1.5f, -0.84f, 0.075f},
                {0.84f, -1.5f, 0.075f}
            };
            
            static float tex[2][2][2] =
            {
                { {0.f, 0.f}, {1.f, 0.f}},
                { {0.f, 1.f}, {1.f, 1.f}}
            };
            
            /* *INDENT-ON* */
            
            static void
            teapot(GLint grid, GLenum type)
            {
                float p[4][4][3], q[4][4][3], r[4][4][3], s[4][4][3];
                long i, j, k, l;
                
                glPushAttrib(GL_ENABLE_BIT | GL_EVAL_BIT);
                glEnable(GL_AUTO_NORMAL);
                glEnable(GL_NORMALIZE);
                glEnable(GL_MAP2_VERTEX_3);
                glEnable(GL_MAP2_TEXTURE_COORD_2);
                for (i = 0; i < 10; i++) {
                    for (j = 0; j < 4; j++) {
                        for (k = 0; k < 4; k++) {
                            for (l = 0; l < 3; l++) {
                                p[j][k][l] = cpdata[patchdata[i][j * 4 + k]][l];
                                q[j][k][l] = cpdata[patchdata[i][j * 4 + (3 - k)]][l];
                                if (l == 1)
                                    q[j][k][l] *= -1.0;
                                if (i < 6) {
                                    r[j][k][l] =
                                    cpdata[patchdata[i][j * 4 + (3 - k)]][l];
                                    if (l == 0)
                                        r[j][k][l] *= -1.0;
                                    s[j][k][l] = cpdata[patchdata[i][j * 4 + k]][l];
                                    if (l == 0)
                                        s[j][k][l] *= -1.0;
                                    if (l == 1)
                                        s[j][k][l] *= -1.0;
                                }
                            }
                        }
                    }
                    glMap2f(GL_MAP2_TEXTURE_COORD_2, 0, 1, 2, 2, 0, 1, 4, 2,
                            &tex[0][0][0]);
                    glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4,
                            &p[0][0][0]);
                    glMapGrid2f(grid, 0.0, 1.0, grid, 0.0, 1.0);
                    glEvalMesh2(type, 0, grid, 0, grid);
                    glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4,
                            &q[0][0][0]);
                    glEvalMesh2(type, 0, grid, 0, grid);
                    if (i < 6) {
                        glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4,
                                &r[0][0][0]);
                        glEvalMesh2(type, 0, grid, 0, grid);
                        glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4,
                                &s[0][0][0]);
                        glEvalMesh2(type, 0, grid, 0, grid);
                    }
                }
                glPopAttrib();
            }
            
            
            /**
             * An OSG wrapper for the classic OpenGL teapot.
             */
            class Teapot :  public osg::Drawable  {
            public:
                Teapot() {}
                
                /** Copy constructor using CopyOp to manage deep vs shallow copy.*/
                Teapot(const Teapot& teapot,const osg::CopyOp& copyop=osg::CopyOp::SHALLOW_COPY):
                osg::Drawable(teapot,copyop) {}
                
                META_Object(myTeapotApp,Teapot)
                
                
                // the draw immediate mode method is where the OSG wraps up the drawing of
                // of OpenGL primitives.
                virtual void drawImplementation(osg::RenderInfo&) const
                {
                    // teapot(..) doens't use vertex arrays at all so we don't need to toggle their state
                    // if we did we'd need to something like following call
                    // state.disableAllVertexArrays(), see src/osg/Geometry.cpp for the low down.
                    
                    // just call the OpenGL code.
                    teapot(14,GL_FILL);
                }
                
                
                // we need to set up the bounding box of the data too, so that the scene graph knows where this
                // objects is, for both positioning the camera at start up, and most importantly for culling.
                virtual osg::BoundingBox computeBound() const
                {
                    osg::BoundingBox bbox;
                    
                    // follow is some truely horrible code required to calculate the 
                    // bounding box of the teapot.  Have used the original code above to do
                    // help compute it.
                    float p[4][4][3], q[4][4][3], r[4][4][3], s[4][4][3];
                    long i, j, k, l;
                    
                    for (i = 0; i < 10; i++) {
                        for (j = 0; j < 4; j++) {
                            for (k = 0; k < 4; k++) {
                                
                                for (l = 0; l < 3; l++) {
                                    p[j][k][l] = cpdata[patchdata[i][j * 4 + k]][l];
                                    q[j][k][l] = cpdata[patchdata[i][j * 4 + (3 - k)]][l];
                                    if (l == 1)
                                        q[j][k][l] *= -1.0;
                                    
                                    if (i < 6) {
                                        r[j][k][l] =
                                        cpdata[patchdata[i][j * 4 + (3 - k)]][l];
                                        if (l == 0)
                                            r[j][k][l] *= -1.0;
                                        s[j][k][l] = cpdata[patchdata[i][j * 4 + k]][l];
                                        if (l == 0)
                                            s[j][k][l] *= -1.0;
                                        if (l == 1)
                                            s[j][k][l] *= -1.0;
                                    }
                                }
                                
                                bbox.expandBy(osg::Vec3(p[j][k][0],p[j][k][1],p[j][k][2]));
                                bbox.expandBy(osg::Vec3(q[j][k][0],q[j][k][1],q[j][k][2]));
                                
                                if (i < 6)
                                {
                                    bbox.expandBy(osg::Vec3(r[j][k][0],r[j][k][1],r[j][k][2]));
                                    bbox.expandBy(osg::Vec3(s[j][k][0],s[j][k][1],s[j][k][2]));
                                }
                                
                                
                            }
                        }
                    }
                    
                    return bbox;
                }
                
            protected:
                
                virtual ~Teapot() {}
                
            };
            
            
            osg::Geode* createTeapot()
            {
                osg::Geode* geode = new osg::Geode();
                
                // add the teapot to the geode.
                geode->addDrawable( new Teapot );
                
                // add a reflection map to the teapot.     
                osg::Image* image = NULL; //osgDB::readImageFile("Images/reflect.rgb");
                if (image)
                {
                    osg::Texture2D* texture = new osg::Texture2D;
                    texture->setImage(image);
                    
                    osg::TexGen* texgen = new osg::TexGen;
                    texgen->setMode(osg::TexGen::SPHERE_MAP);
                    
                    osg::StateSet* stateset = new osg::StateSet;
                    stateset->setTextureAttributeAndModes(0,texture,osg::StateAttribute::ON);
                    stateset->setTextureAttributeAndModes(0,texgen,osg::StateAttribute::ON);
                    
                    geode->setStateSet(stateset);
                }
                
                return geode;
            }
        }
    }
}
