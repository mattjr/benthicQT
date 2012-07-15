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



#include "MeshFile.h"
//#include "SimulationState.h"
#include <osg/Vec2>
#include <osg/ref_ptr>
#include <QDesktopServices>
#include "BQTDebug.h"
#include <QtGui/QApplication>
#include "BQTMainWindow.h"
#include <QtGui/QClipboard>
#include <QtGui/QMessageBox>
#include "ScreenTools.h"
#include "seabed_slam_file_io.hpp"
#include "MyShaderGen.h"
#include "qgscolorbrewerpalette.h"
#include "LibVT.h"
#include "LibVT_Internal.h"
osg::StateSet* createSS();
osg::Geode* createVTShapes(osg::StateSet* ss);
osg::Geode* createNonVTShapes(osg::StateSet* ss);
void loadShaderSource(osg::Shader* obj, const std::string& fileName, const std::string& prelude);
void loadShaderSourceFromStr(osg::Shader* obj, const unsigned char * shader, const std::string& prelude );


extern unsigned char readback_vert[];

extern unsigned char readback_frag[];


extern unsigned char renderVT_vert[];
extern unsigned char renderVT_frag[];

#define clamp(x, low, high)  (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))
int floorToInt(float x){
    return (int)floor(x);
}

osg::Vec4b getTexel(osg::Image *image, int x, int y)  {
        int levelWidth = image->s();
        int levelHeight = image->t();

        if (x < 0 || y < 0 || x >= levelWidth || y >= levelHeight) {

                                x = clamp(x, 0, levelWidth - 1);
                                y = clamp(y, 0, levelHeight - 1);

        }
        return ((osg::Vec4b *)image->data())[x + levelWidth*y];
}
osg::Vec4b triangle(osg::Image *image, float x, float y)  {
                int xPos = floorToInt(x*image->s()),
                        yPos = floorToInt(y*image->t());
                return getTexel(image, xPos, yPos);
}
double Frac(double arg)
{
    /* Returns fractional part of double argument */
    return arg - floor(arg);
}
osg::Vec2f addrTranslation_1Dto2D( float address1D, osg::Vec2f  texSize )

{



    osg::Vec2f CONV_CONST = osg::Vec2f (( 1.0 / texSize[0]),

            1.0 / (texSize[0] * texSize[1] ));



  // Return a normalized 2D address (with values in [0,1])

  osg::Vec2f  normAddr2D =osg::Vec2f(CONV_CONST.x()*address1D,CONV_CONST.y()*address1D);

  osg::Vec2f  address2D = osg::Vec2f ( Frac(normAddr2D.x()), normAddr2D.y() );

  return address2D;

}
namespace ews {
    namespace app {
        /**
         * Contains classes responsible for business logic associated with drawable objects.
         */
        namespace model {


            MeshFile::MeshFile(QOSGWidget *renderer): _renderer(renderer)
                    //            : QObject(parent)
            {
                //    QObject::connect(this, SIGNAL(dataChanged()), this, SLOT(generatePotential()));
                progress = new QProgressDialog();
                progress->setWindowModality(Qt::WindowModal);
                progress->setCancelButtonText(0);
                shared_uniforms.resize(NUM_UNI_ENUM);
                shared_uniforms[UNI_SHADER_OUT]= new osg::Uniform("shaderOut",0);
                shared_uniforms[UNI_COLORMAP_SIZE]=new osg::Uniform("colormapSize",0);
                shared_uniforms[UNI_DATAUSED]= new osg::Uniform("dataused",0);
                shared_uniforms[UNI_VAL_RANGE]= new osg::Uniform("valrange",osg::Vec2(0.0,0.0));
                shared_uniforms[UNI_TEXSCALE]= new osg::Uniform("texScale",0.0f);
                shared_uniforms[UNI_OPACITY]= new osg::Uniform("opacity",1.0f);


                shader_names.push_back("Texture");
                shader_names.push_back("Aux");
              //  shader_names.push_back("Overlay");
               // shader_names.push_back("Shaded");


              /*  static_shader_colormaps.push_back("Jet");
                static_shader_colormaps.push_back("Rainbow");
                static_shader_colormaps.push_back("Hot");
                static_shader_colormaps.push_back("Bone");
                static_shader_colormaps.push_back("Pink");
                static_shader_colormaps.push_back("Spring");
                static_shader_colormaps.push_back("Summer");
                static_shader_colormaps.push_back("Winter");
                static_shader_colormaps.push_back("Cool");

                static_shader_colormaps.push_back("Grey");*/
                colormap_names= &static_shader_colormaps;

                dataused_names.push_back("Height");
                label_range=osg::Vec2(0,0);
                //_manip=
                _stateset=NULL;
                colorbar=NULL;
                colorbar_hud=NULL;
                textNode=NULL;
                scalebar_hud=NULL;
                shared_tex=new osg::Texture2D();

                shared_tex->setNumMipmapLevels(0);
                shared_tex->setFilter(osg::Texture::MIN_FILTER , osg::Texture::LINEAR);
                shared_tex->setFilter(osg::Texture::MAG_FILTER , osg::Texture::LINEAR);
                shared_tex->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP);
                shared_tex->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP);
                shared_tex->setWrap(osg::Texture::WRAP_R, osg::Texture::CLAMP);
                font_name="fonts/VeraMono.ttf";
                selColorMap=0;
                dataout=0;
                seq_colormap_colors=7.0;
                QgsColorBrewerPalette cb_pal;

                texture_color_brewer_names_DIV=cb_pal.listSchemesDiv();

            }
            

            /** Create a new barrier and add it to the list. */
            void MeshFile::loadMesh() {
                qDebug() <<"MeshFile::loadMesh()";
                emit loadNewMeshes(this);


            }
            class VTExitHandler : public osgGA::GUIEventHandler // TODO: this exit handler never receives the exit events! mac bug or something else?
            {
            public:
                bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter&)
                {
                    switch(ea.getEventType())
                    {
                        case(osgGA::GUIEventAdapter::KEYUP):
                        {
                            if (ea.getKey()==osgGA::GUIEventAdapter::KEY_Escape)
                            {
                                vtShutdown();
                            }
                            return false;
                        }
                        case(osgGA::GUIEventAdapter::CLOSE_WINDOW):
                        case(osgGA::GUIEventAdapter::QUIT_APPLICATION):
                        {
                            vtShutdown();
                        }
                        default: break;
                    }

                    return false;
                }
            };

            class VTWindowResizeHandler : public osgViewer::WindowSizeHandler
            {
            public:
                bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter&aa)
                {

                    switch(ea.getEventType())
                    {
                        case(osgGA::GUIEventAdapter::RESIZE):
                        {
                            int w = (uint16_t)ea.getWindowWidth(), h = (uint16_t)ea.getWindowHeight();

                            vtReshape(w, h, 0.0, 0.0, 0.0);

                            _texture->setTextureSize(w >> PREPASS_RESOLUTION_REDUCTION_SHIFT, h >> PREPASS_RESOLUTION_REDUCTION_SHIFT);
                            _image->allocateImage(w >> PREPASS_RESOLUTION_REDUCTION_SHIFT, h >> PREPASS_RESOLUTION_REDUCTION_SHIFT, 1, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV);
                        //	image->setInternalTextureFormat(GL_RGBA);
                            _pre_camera->setViewport(0, 0, w >> PREPASS_RESOLUTION_REDUCTION_SHIFT, h >> PREPASS_RESOLUTION_REDUCTION_SHIFT);


                            break;
                        }
                        default:
                            break;
                    }
                    return WindowSizeHandler::handle(ea, aa);
                }


                osg::TextureRectangle* _texture;
                osg::Camera* _pre_camera;
                osg::Image* _image;

            };


            struct PreDrawCallback : public osg::Camera::DrawCallback
            {
                virtual void operator () (const osg::Camera&) const
                {
                    vtMapNewPages();
                }
            };

            struct PostDrawCallback : public osg::Camera::DrawCallback
            {
                PostDrawCallback(osg::Image* image): _image(image)
                {
                }

                virtual void operator () (const osg::Camera&) const
                {
                    if (_image && _image->getPixelFormat()==GL_BGRA && _image->getDataType()==GL_UNSIGNED_INT_8_8_8_8_REV)
                    {
                        //std::string filename = std::string("/Users/julian/Desktop/test.rgb");
                        //osgDB::writeImageFile(*_image, filename);

                        vtExtractNeededPages((const uint32_t *) _image->data());
                    }
                    else
                    {
                        printf("houston we have a problem");
                    }

                }

                osg::Image* _image;
            };

            osg::Group* MeshFile::loadVTModel(osg::Node *vtgeode,osg::Camera *&pre_camera,
                                              osg::TextureRectangle *&texture,osg::Image *&image)

            {
                osg::Group* group = new osg::Group;

            //	osg::StateSet* ss = createSS();
                    //osg::Geode* nonvtgeode = createNonVTShapes(ss);
                osg::Group* vtgroup_prerender = new osg::Group;
                osg::Group* vtgroup_mainpass = new osg::Group;


                texture = new osg::TextureRectangle;
                texture->setTextureSize(_renderer->width() >> PREPASS_RESOLUTION_REDUCTION_SHIFT, _renderer->height() >> PREPASS_RESOLUTION_REDUCTION_SHIFT);
                texture->setInternalFormat(GL_BGRA);
                texture->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR);
                texture->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture2D::LINEAR);

                pre_camera = new osg::Camera;
                pre_camera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                pre_camera->setClearColor(osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f));
                pre_camera->setViewport(0, 0, _renderer->width() >> PREPASS_RESOLUTION_REDUCTION_SHIFT,_renderer->width()>> PREPASS_RESOLUTION_REDUCTION_SHIFT); // TODO: fix the hardcoding
                pre_camera->setRenderOrder(osg::Camera::PRE_RENDER);
                pre_camera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);

                image = new osg::Image;
                image->allocateImage(_renderer->width() >> PREPASS_RESOLUTION_REDUCTION_SHIFT, _renderer->width() >> PREPASS_RESOLUTION_REDUCTION_SHIFT, 1, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV);
                image->setInternalTextureFormat(GL_RGBA);
                pre_camera->attach(osg::Camera::COLOR_BUFFER, image, 0, 0);
                pre_camera->setPostDrawCallback(new PostDrawCallback(image));
                texture->setImage(0, image);


                char *prelude = vtGetShaderPrelude();

                // setup the shaders used for virtual textured objects prepass
                osg::StateSet* vtpreState = vtgroup_prerender->getOrCreateStateSet();
                osg::Program* vtpreProgramObject = new osg::Program;
                osg::Shader* vtpreVertexObject = new osg::Shader( osg::Shader::VERTEX );
                osg::Shader* vtpreFragmentObject = new osg::Shader( osg::Shader::FRAGMENT );
                vtpreProgramObject->addShader( vtpreFragmentObject );
                vtpreProgramObject->addShader( vtpreVertexObject );
                vtpreState->addUniform( new osg::Uniform("pageTableTexture", TEXUNIT_FOR_PAGETABLE) );
                vtpreState->addUniform( new osg::Uniform("mipcalcTexture", TEXUNIT_FOR_MIPCALC) );
                vtpreState->addUniform( new osg::Uniform("mip_bias", vtGetBias()) ); // this shold be done every frame if we want dynamic LoD adjustment


                loadShaderSourceFromStr(vtpreVertexObject, readback_vert, string(prelude));
                loadShaderSourceFromStr( vtpreFragmentObject, readback_frag, string(prelude));

                vtpreState->setAttributeAndModes(vtpreProgramObject, osg::StateAttribute::ON);

                // setup the shaders used for virtual textured objects mainpass
                osg::StateSet* vtmainState = vtgroup_mainpass->getOrCreateStateSet();
                osg::Program* vtmainProgramObject = new osg::Program;
                osg::Shader* vtmainVertexObject = new osg::Shader( osg::Shader::VERTEX );
                osg::Shader* vtmainFragmentObject = new osg::Shader( osg::Shader::FRAGMENT );
                vtmainProgramObject->addShader( vtmainFragmentObject );
                vtmainProgramObject->addShader( vtmainVertexObject );
                vtmainState->addUniform( new osg::Uniform("pageTableTexture", TEXUNIT_FOR_PAGETABLE) ); // TODO: we gotta make sure OSG NEVER USES THESE TEXUNITS!!
                vtmainState->addUniform( new osg::Uniform("physicalTexture", TEXUNIT_FOR_PHYSTEX) );
                vtmainState->addUniform( new osg::Uniform("mip_bias", vtGetBias()) );

                loadShaderSourceFromStr( vtmainVertexObject,  renderVT_vert, string(prelude));
                loadShaderSourceFromStr( vtmainFragmentObject,  renderVT_frag, string(prelude));

                vtmainState->setAttributeAndModes(vtmainProgramObject, osg::StateAttribute::ON); // TODO: barf on shader errors

                free(prelude);



                vtgroup_prerender->addChild(vtgeode);
                vtgroup_mainpass->addChild(vtgeode);

                pre_camera->addChild(vtgroup_prerender);

                group->addChild(pre_camera);
                group->addChild(vtgroup_mainpass);
                    //group->addChild(nonvtgeode);

                return group;
            }
             osg::Node * MeshFile::createVTStuff(osg::Node *node,string tex_name){
               osg::Node *retNode=NULL;
                if(osgDB::fileExists(tex_name))
                {
                    cout <<"Loading Virt Tex\n";
                    cout <<tex_name<<"\n";

                    char ext [5] = "    ";
                            uint8_t border, length;
                            uint32_t dim;
                    bool success = vtScan(tex_name.c_str(), ext, &border, &length, &dim);
                    if(success){
                        GLint textureSize = std::min(osg::Texture2D::getExtensions(0,true)->maxTextureSize(),4096);

                             vtInit(tex_name.c_str(), ext, border, length, dim,textureSize);
                            retNode= loadVTModel(node,pre_camera,texture,image);
                    }
                             else{
                                 fprintf(stderr,"can't open %s\n",tex_name.c_str());
                                 return NULL;
                    }


                _renderer->addEventHandler(new VTWindowResizeHandler());
                         _renderer->addEventHandler(new osgViewer::StatsHandler());

                  _renderer->addEventHandler(new VTExitHandler());
                        _renderer->getCamera()->setPreDrawCallback(new PreDrawCallback());


                              vtReshape(_renderer->width(), _renderer->height(), 0.0, 0.0, 0.0);
                                vtPrepare((const GLuint) 0, (const GLuint) 0);
                                return retNode;
                }
            }

            MeshFile::~MeshFile() {
                vtShutdown();
            }
            void MeshFile::copyCurrentImageClipboard(){
                QClipboard *clipboard = QApplication::clipboard();

                clipboard->setText(curr_img);
            }

            void MeshFile::openCurrentImage()
            {
                std::string imgdir_file;
                char reldir[1024];
                string path;
                QStringList list=getFileNames();
                QStringList::Iterator it = list.begin();
                while( it != list.end() ) {
                    path=osgDB::getFilePath(it->toStdString());
                    if(path.size() ==0)
                        path=".";
                    imgdir_file=string(path+"/imgpath.txt");
                    FILE *fp=fopen(imgdir_file.c_str(),"r");
                    if(fp)
                        fscanf(fp,"%s\n",reldir);
                    else
                        printf("Can't open %s",imgdir_file.c_str());
                    it++;
                }
                QUrl url;
                QString fullpath=string(path+"/"+reldir+"/"+curr_img.toStdString()).c_str();
                qDebug() << fullpath<<"\n";
                if(curr_img.size())
                    url.setUrl(fullpath);
                QDesktopServices::openUrl(url);
            }
            void MeshFile::updateImage(osg::Vec3 v)
            {
                bbox_map_info info;
             //   cout << v << endl;
              //  cout <<_tree<<endl;
                if(find_closet_img_idx(_tree,v,info)){
                    curr_img=(info.leftname).c_str();
                    emit imgLabelChanged(curr_img);
                }else if(curr_img.size()){
                    curr_img="";
                    QString s;
                    s=string("N/A").c_str();
                    emit imgLabelChanged(s);
                    //qDebug()<<"No img at " << v[0] << " " << v[1] << " "<<v[2];
                }

            }
            class JetColorMap :public osgSim::ScalarsToColors{
            public:
              JetColorMap(float min,float max): osgSim::ScalarsToColors(min,max){   }
              virtual osg::Vec4 getColor(float scalar) const{

                float min= std::min(getMin(),getMax());
                float max= std::max(getMin(),getMax());
                float range=max-min;
                float val=(scalar-min)/range;
                osg::Vec4 jet;

                 jet[0] = std::min(4.0f * val - 1.5f,-4.0f * val + 4.5f) ;
                    jet[1] = std::min(4.0f * val - 0.5f,-4.0f * val + 3.5f) ;
                    jet[2] = std::min(4.0f * val + 0.5f,-4.0f * val + 2.5f) ;


                    jet[0] = clamp(jet[0], 0.0f, 1.0f);
                    jet[1] = clamp(jet[1], 0.0f, 1.0f);
                    jet[2] = clamp(jet[2], 0.0f, 1.0f);
                    jet[3] = 1.0;
                    //std::cout << jet << val<<std::endl;
                    return jet;

              }
            };

            class ColorBrewerMapDiscrete :public osgSim::ScalarsToColors{
            public:
                ColorBrewerMapDiscrete(float min,float max,QList<QColor> &palette): osgSim::ScalarsToColors(min,max), mPalette(palette){
                    minV= std::min(getMin(),getMax());
                    maxV= std::max(getMin(),getMax());
                    range=maxV-minV;
                }
                virtual osg::Vec4 getColor(float scalar) const{

                    if ( mPalette.isEmpty() || scalar<=1.0 )
                        return osg::Vec4(0,0,0,1);

                    float val=((scalar-1.0)-minV)/(range-1.0);

                    if (  val < minV || val > maxV )
                        return osg::Vec4(0,0,0,1);

                    int paletteEntry = ( int )( val * mPalette.count() );
                    if ( paletteEntry >= mPalette.count() )
                        paletteEntry = mPalette.count() - 1;
                    QColor c= mPalette.at( paletteEntry );
                    return osg::Vec4(c.red()/255.0,c.green()/255.0,c.blue()/255.0,1.0);


                }
                QList<QColor> &mPalette;
                float minV,maxV,range;
            };


            class ColorBrewerMap :public osgSim::ScalarsToColors{
            public:
                ColorBrewerMap(float min,float max,QList<QColor> &palette): osgSim::ScalarsToColors(min,max), mPalette(palette){
                    minV= std::min(getMin(),getMax());
                    maxV= std::max(getMin(),getMax());
                    range=maxV-minV;
                }
                virtual osg::Vec4 getColor(float scalar) const{

                    if ( mPalette.isEmpty() )
                        return osg::Vec4(0,0,0,1);

                    float val=(scalar-minV)/(range);
                    float x = clamp(val,0.0,1.0) * (mPalette.size() - 1);
                    float x0 = floor(x);
                    int i = (int)x0;
                    if (i == mPalette.size() - 1){
                        QColor c= mPalette.at( i );
                        return osg::Vec4(c.red()/255.0,c.green()/255.0,c.blue()/255.0,1.0);
                    }
                    float dx = x - x0;
                    QColor c1= mPalette.at( i );
                    QColor c2= mPalette.at( i+1 );
                    float r1=1.0-dx;
                    return osg::Vec4((c1.red()/255.0)*r1,(c1.green()/255.0)*r1,(c1.blue()/255.0)*r1,1.0)+
                            osg::Vec4((c2.red()/255.0)*dx,(c2.green()/255.0)*dx,(c2.blue()/255.0)*dx,1.0);



                }
                QList<QColor> &mPalette;
                float minV,maxV,range;
            };


            void MeshFile::createScalarBar_HUD(void)
            {
                if(colorbar)
                    return;
                colorbar = new osgSim::ScalarBar;

                /*
                osg::MatrixTransform * modelview = new osg::MatrixTransform;
                modelview->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
                osg::Matrixd matrix(osg::Matrixd::scale(1000,1000,1000) * osg::Matrixd::translate(120,10,0)); // I've played with these values a lot and it seems to work, but I have no idea why
                modelview->setMatrix(matrix);
                modelview->addChild(colorbar);

                colorbar_hud = new osg::Projection;
                colorbar_hud->setMatrix(osg::Matrix::ortho2D(0,_renderer->width(),0,_renderer->height())); // or whatever the OSG window res is
                colorbar_hud->addChild(modelview);*/

                colorbar_hud = new osg::Camera;
                colorbar_hud->setProjectionMatrix(osg::Matrix::ortho2D(0,_renderer->width(),0,_renderer->height())); // or whatever the OSG window res is
                colorbar_hud->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
                colorbar_hud->setViewMatrix(osg::Matrix::identity());
                colorbar_hud->setDataVariance(osg::Object::DYNAMIC);

               // osg::MatrixTransform * matT = new osg::MatrixTransform;
              //  matT->setMatrix(osg::Matrix::translate(_renderer->width()-width-margin,margin,0));
               // matT->addChild(colorbar);
                colorbar_hud->addChild(colorbar);
                colorbar_hud->setRenderOrder(osg::Camera::POST_RENDER);
                colorbar_hud->setClearMask(GL_DEPTH_BUFFER_BIT);
            }



         void MeshFile::createScaleBar_HUD(void)
            {
                 if(textNode)
                     return;
                 textNode = new osgText::Text;
                osgText::Font* font = osgText::readFontFile(font_name.c_str());
                float margin = 50.0f;

                osg::Vec4 layoutColor(1.0f,1.0f,1.0f,1.0f);
                   float layoutCharacterSize = 18.0f;
                   textNode->setFont(font);
                   textNode->setColor(layoutColor);
                   textNode->setCharacterSize(layoutCharacterSize);

                osg::StateSet * stateset = textNode->getOrCreateStateSet();
                stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

                stateset->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);
                stateset->setRenderBinDetails(11, "RenderBin");



                scalebar_hud = new osg::Camera;
                scalebar_hud->setProjectionMatrix(osg::Matrix::ortho2D(0,_renderer->width(),0,_renderer->height())); // or whatever the OSG window res is
                scalebar_hud->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
                scalebar_hud->setViewMatrix(osg::Matrix::identity());
                scalebar_hud->addChild(createScaleBar(textNode,_renderer->getWWManip(),_renderer->getCamera()));
                scalebar_hud->setRenderOrder(osg::Camera::POST_RENDER);
            scalebar_hud->setClearMask(GL_DEPTH_BUFFER_BIT);
            }
            void MeshFile::updateGlobal(osg::Vec4 v){

                emit posChanged(v);

            }

            void MeshFile::switchMinimap(bool enabled){
                if(enabled) {
                    _mapSwitch->setAllChildrenOn();
                }
                else {
                    _mapSwitch->setAllChildrenOff();
                }

            }
            void MeshFile::setStateSet(osg::StateSet *state){
                _stateset=state;
            }
            void MeshFile::setupPallet(){
                QgsColorBrewerPalette cb_pal;
                int num_labels;
                if(colorbar_hud->getNumChildren() !=0)
                    colorbar_hud->removeChild(0,1);
                colorbar=new osgSim::ScalarBar;
                if(dataout == HEIGHT_DATA){
                    num_labels=seq_colormap_colors;
                    printf("%d bla\n",texture_color_brewer_names_DIV.size());
                    mPalette=cb_pal.getDivScheme( texture_color_brewer_names_DIV[selColorMap]);

                }else if(dataout == LABEL_DATA && label_range[1]>0){
                    num_labels=(int)label_range[1];
                    mPalette=cb_pal.getQualScheme( texture_color_brewer_names_QUAL[selColorMap],num_labels);

                }else
                    return;
                if(!dataImage){
                    return;
                }
                if(/*!colormap_names ||selColorMap >=colormap_names->size() || selColorMap < 0 ||*/ mPalette.size() ==0){
                    fprintf(stderr,"cant get pallet for this selection error %d\n",selColorMap);
                    return;
                }


                osg::Vec4ub *ptr=((osg::Vec4ub *)dataImage->data())+(dataImage->s()*dataImage->t()-1);
                for(int i=0; i<mPalette.size(); i++){
                    *ptr=osg::Vec4ub(mPalette[i].red(),mPalette[i].green(),mPalette[i].blue(),255);
                    ptr--;
                }

                if(dataout == HEIGHT_DATA){
                 /*   colorbar->setScalarsToColors(new JetColorMap(zrange[0],zrange[1]));
                    colorbar->setNumLabels(5);
                    colorbar->setTitle(std::string("Height"));
                    colorbar->setScalarPrinter(sp);*/
                    colorbar=new osgSim::ScalarBar(256,5,new ColorBrewerMap(zrange[0],zrange[1],mPalette),"Height",osgSim::ScalarBar::HORIZONTAL,0.1,new TrunkScalarPrinter);
                }else if(dataout == LABEL_DATA){
                    colorbar=new osgSim::ScalarBar(((label_range[1]+1)*2) +1,((label_range[1]+1)*2) +1,
                                                  new ColorBrewerMapDiscrete(label_range[0],label_range[1]+1.0,mPalette),
                                                  "Labels",osgSim::ScalarBar::HORIZONTAL,0.1,new DiscretLabelPrinter);


                }
                float width=160.0;
                float margin=20.0;
                colorbar->setWidth(width);
                colorbar->setPosition(osg::Vec3(_renderer->width()-width-margin,margin,0));
                colorbar->setDataVariance(osg::Object::DYNAMIC);
                osgSim::ScalarBar::TextProperties tp;
                tp._fontFile = font_name.c_str();
                tp._characterSize=12.0f;
                colorbar->setTextProperties(tp);
               // colorbar->setOrientation(osgSim::ScalarBar::VERTICAL);
                osg::StateSet * stateset = colorbar->getOrCreateStateSet();
                stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

                stateset->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);
                stateset->setRenderBinDetails(11, "RenderBin");
                shared_tex->dirtyTextureObject();
                colorbar_hud->addChild(colorbar);
            }

            void MeshFile::setShaderOut(int index) {
                if(shared_uniforms.size() > UNI_SHADER_OUT && shared_uniforms[UNI_SHADER_OUT])
                    shared_uniforms[UNI_SHADER_OUT]->set(index);
                if(_stateset){
                    unsigned int mode = osg::StateAttribute::OVERRIDE|osg::StateAttribute::OFF;
                    if ( index ==0 ) mode = osg::StateAttribute::INHERIT|osg::StateAttribute::ON;
                    for( unsigned int ii=0; ii<4; ii++ )
                    {
                        _stateset->setTextureMode( ii, GL_TEXTURE_2D, mode );

                    }
                }
            }

            void MeshFile::updateSharedAttribTex() {
                GLint textureSize = osg::Texture2D::getExtensions(0,true)->maxTextureSize();
                QgsColorBrewerPalette cb_pal;
                float max_el,min_el;
                QStringList list=getFileNames();
                QStringList::Iterator it = list.begin();
                while( it != list.end() ) {
                    string path=osgDB::getFilePath(it->toStdString());
                    if(path.size() ==0)
                        path=".";
                    char tmp[8192];
                    int idx;
                    float time;
                    string labelfn=string(path+"/image_label.data");
                    string imgmap_fn=string(path+"/img_num.txt");
                    if(!osgDB::fileExists(labelfn) ){
		      it++;
		      continue;
		    }

                    vector<Image_Label> labels;
                    try{
                        labels= read_image_label_file(labelfn);
                    }catch( std::string error ) {
                        std::cerr << "ERROR Parsing imagelabel- " << error << endl;
                        it++;
			continue;
                    }
                    if(osgDB::fileExists(imgmap_fn)){
                        FILE *fpimgmap=fopen(imgmap_fn.c_str(), "r");
                        if(!fpimgmap){
                            sprintf(tmp,"Cant open file %s\n",imgmap_fn.c_str());
                            QMessageBox::warning( _renderer, QString("Warning:"),QString(tmp),QMessageBox::Ok);
                            it++;
                            continue;
                        }

                        std::map<string,int> remap_fn_idx;
                        while(!feof(fpimgmap)){
                            fscanf(fpimgmap,"%d %f %s",&idx,&time,tmp);
                            remap_fn_idx[tmp]=idx;

                        }

                        for(int i=0; i<(int)labels.size(); i++){
                            string fn=osgDB::getNameLessExtension(labels[i].left_image_name);
                            if(remap_fn_idx.count(fn))
                                labels[i].pose_id=remap_fn_idx[fn];
                        }
                    }
                    int max_poseid=0;
                    for(int i=0; i<(int)labels.size(); i++){
                        if(max_poseid < labels[i].pose_id &&labels[i].pose_id < (textureSize*textureSize))
                            max_poseid=labels[i].pose_id;
                    }
                    current_attributes.resize(max_poseid,0);
                     max_el=-FLT_MAX;
                     min_el=FLT_MAX;
                    for(int i=0; i<(int)labels.size(); i++){
                        //printf("%d %s :%d\n",labels[i].pose_id,labels[i].left_image_name.c_str(),labels[i].label);
                        if(labels[i].pose_id <  current_attributes.size()){
                            current_attributes[labels[i].pose_id ]=labels[i].label;
                            if(max_el < labels[i].label)
                                max_el=labels[i].label;
                            if(min_el > labels[i].label)
                                min_el=labels[i].label;
                        }
                    }
                    if (min_el > 0.0)
                        min_el=0.0;


                    int num_labels=(int)max_el;
                    if(dataused_names.size() < 2)
                        dataused_names.push_back("Labels");

                   // texture_color_brewer_names_SEQ=cb_pal.listSchemesSeq();
                    texture_color_brewer_names_QUAL=cb_pal.listSchemesQual(num_labels);

                    //mPalette=cb_pal.getQualScheme( texture_color_brewer_names_QUAL[selColorMap],num_labels);
                    //printf("Pal size %d %s\n",mPalette.size(),texture_color_brewer_names[0].toAscii().data());
                    label_range=osg::Vec2(min_el,max_el);
                    it++;
               }
                    //printf("Min El %f Max El %f\n",min_el,max_el);
                    int num=current_attributes.size()+cb_pal.max_pal_size();
                    int dim=ceil(sqrt(num));
                    dim=osg::Image::computeNearestPowerOfTwo(dim,1.0);
                    if(dim> textureSize){
                        char str[1024];
                        sprintf(str,"Dimension of texture too high can't show all attribute data %d < %d",dim,textureSize);
                        QMessageBox::warning( _renderer, QString("Warning:"),QString(str),QMessageBox::Ok);
                        dim=textureSize;
                    }
                    dataImage=new osg::Image;
                    dataImage->allocateImage(dim,dim, 1, GL_RGBA,GL_UNSIGNED_BYTE);
                    unsigned char* dataPtr= (unsigned char*)dataImage->data();
                    memset(dataPtr,0,dataImage->getImageSizeInBytes());
                    for(int i=0; i < current_attributes.size() && i < dim*dim; i++){
                       // printf("Pose id %d val %f\n",i,current_attributes[i]);
                            if(current_attributes[i] < min_el)
                                continue;
                            float s=((current_attributes[i])-min_el)/(max_el-min_el);
                            *(((osg::Vec4ub*)dataPtr)+i) = EncodeDepth(s);


                        //printf("1d %0.1f = (%0.20f %0.20f)\n",(float)i,add.x(),add.y());
                       // printf("%0.1f = %d\n",(current_attributes[i]), triangle(dataImage,add.x(),add.y()).r());
                    }
                    //Place pallet in texture
                    shared_tex->setImage(dataImage);
                    setupPallet();

                    if(shared_uniforms.size() > UNI_TEXSCALE && shared_uniforms[UNI_TEXSCALE])
                        shared_uniforms[UNI_TEXSCALE]->set((float)dim);


            }

            void MeshFile::setColorMap(int index) {
                selColorMap=index;
                setupPallet();
                if(shared_uniforms.size() > UNI_COLORMAP_SIZE && shared_uniforms[UNI_COLORMAP_SIZE])
                    shared_uniforms[UNI_COLORMAP_SIZE]->set(mPalette.size());
            }
            void MeshFile::setDataRange(osg::Vec2 range){
                //cout << "Setting data range " << range <<endl;
                if(shared_uniforms.size() > UNI_VAL_RANGE && shared_uniforms[UNI_VAL_RANGE])
                    shared_uniforms[UNI_VAL_RANGE]->set(range);
            }
            void MeshFile::setOpacity(int val) {
                if(shared_uniforms.size() > UNI_OPACITY && shared_uniforms[UNI_OPACITY]){
                    shared_uniforms[UNI_OPACITY]->set(val/255.0f);
                }
            }
            void MeshFile::setDataUsed(int index) {
                if(shared_uniforms.size() > UNI_DATAUSED && shared_uniforms[UNI_DATAUSED]){
                    shared_uniforms[UNI_DATAUSED]->set(index);
                    if(index == HEIGHT_DATA){
                        colormap_names=&texture_color_brewer_names_DIV;
                        dataout=index;
                        emit colorMapChanged(index);
                        setDataRange(zrange);
                    }
                    else if(index == LABEL_DATA) {
                        colormap_names=&texture_color_brewer_names_QUAL;
                        dataout=index;
                        emit colorMapChanged(index);
                        setDataRange(label_range);
                    }

                    else{
                        printf("Set data range based on somthing toDO\n");
                    }
                }
            }


        }
    }
}
