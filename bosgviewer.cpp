/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2006 Robert Osfield 
 *
 * This application is open source and may be redistributed and/or modified   
 * freely and without restriction, both in commercial and non commercial applications,
 * as long as this copyright notice is maintained.
 * 
 * This application is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#include <osgDB/ReadFile>
#include "MyShaderGen.h"

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgGA/StateSetManipulator>
#include <osgGA/AnimationPathManipulator>
#include <osgGA/TerrainManipulator>
#include <osg/io_utils>
#include <iostream>

#include "MeshFile.h"



float DecodeDepth( const osg::Vec4ub &shadowSample )

{

        return shadowSample[0] / 1.0f +

           shadowSample[1] / 256.0f +

               shadowSample[2] / 65536.0f +

               shadowSample[3] / 16777216.0f;

}
float DecodeDepth( const osg::Vec4f &shadowSample )

{

        return (255.0f*shadowSample[0]) / 1.0f +

           (255.0f*shadowSample[1]) / 256.0f +

               (255.0f*shadowSample[2]) / 65536.0f +

               (255.0f*shadowSample[3]) / 16777216.0f;

}
float unpackDepth( const osg::Vec4f rgba_depth ) {
    osg::Vec4f bit_shift = osg::Vec4f( 1.0 / ( 16777216.0 ), 1.0 / ( 65536.0 ), 1.0 / 256.0, 1.0 );
    return ( rgba_depth* bit_shift );
}
/*osg::Vec4f packDepth( const float depth ) {
    const osg::Vec4f bitShift = osg::Vec4f( 16777216.0, 65536.0, 256.0, 1.0 );
    const osg::Vec4f bitMask = osg::Vec4f( 0.0, 1.0 / 256.0, 1.0 / 256.0, 1.0 / 256.0 );
    osg::Vec4f res = modf( depth * bitShift );
    res -= res.xxyz * bitMask;
    return res;
}*/
double Fract(double arg)
{
    /* Returns fractional part of double argument */
    return arg - floor(arg);
}
osg::Vec4f pack_depth(float depth)
{
    const osg::Vec4f bit_shift(256.0*256.0*256.0, 256.0*256.0, 256.0, 1.0);
    const osg::Vec4f bit_mask(0.0, 1.0/256.0, 1.0/256.0, 1.0/256.0);
    osg::Vec4f res = osg::Vec4f(Fract(depth * bit_shift[0]),
                                Fract(depth * bit_shift[1]),
                                Fract(depth * bit_shift[2]),
                                Fract(depth * bit_shift[3]));
    res[0] -= res[0] * bit_mask[0];
    res[1] -= res[0] * bit_mask[1];
    res[2] -= res[1] * bit_mask[2];
    res[3] -= res[2] * bit_mask[3];

    return res;
}
float unpack_depth(osg::Vec4f rgba_depth)
{
    const osg::Vec4f bit_shift(1.0/(256.0*256.0*256.0), 1.0/(256.0*256.0), 1.0/256.0, 1.0);
    float depth = (rgba_depth* bit_shift);
    return depth;
}
int main(int argc, char** argv)
{
    // use an ArgumentParser object to manage the program arguments.
    osg::ArgumentParser arguments(&argc,argv);

    arguments.getApplicationUsage()->setApplicationName(arguments.getApplicationName());
    arguments.getApplicationUsage()->setDescription(arguments.getApplicationName()+
            " is an example of conversion of fixed function pipeline to GLSL");
    arguments.getApplicationUsage()->setCommandLineUsage(arguments.getApplicationName()+" [options] filename ...");

    osgViewer::Viewer viewer(arguments);

    unsigned int helpType = 0;
    if ((helpType = arguments.readHelpType()))
    {
        arguments.getApplicationUsage()->write(std::cout, helpType);
        return 1;
    }
    
    // report any errors if they have occurred when parsing the program arguments.
    if (arguments.errors())
    {
        arguments.writeErrorMessages(std::cout);
        return 1;
    }
    
    if (arguments.argc()<=1)
    {
        arguments.getApplicationUsage()->write(std::cout,osg::ApplicationUsage::COMMAND_LINE_OPTION);
        return 1;
    }

    // set up the camera manipulators.
    {
        osg::ref_ptr<osgGA::KeySwitchMatrixManipulator> keyswitchManipulator = new osgGA::KeySwitchMatrixManipulator;

        keyswitchManipulator->addMatrixManipulator( '1', "Trackball", new osgGA::TrackballManipulator() );
        keyswitchManipulator->addMatrixManipulator( '2', "Flight", new osgGA::FlightManipulator() );
        keyswitchManipulator->addMatrixManipulator( '3', "Drive", new osgGA::DriveManipulator() );
        keyswitchManipulator->addMatrixManipulator( '4', "Terrain", new osgGA::TerrainManipulator() );

        std::string pathfile;
        char keyForAnimationPath = '5';
        while (arguments.read("-p",pathfile))
        {
            osgGA::AnimationPathManipulator* apm = new osgGA::AnimationPathManipulator(pathfile);
            if (apm || !apm->valid()) 
            {
                unsigned int num = keyswitchManipulator->getNumMatrixManipulators();
                keyswitchManipulator->addMatrixManipulator( keyForAnimationPath, "Path", apm );
                keyswitchManipulator->selectMatrixManipulator(num);
                ++keyForAnimationPath;
            }
        }

        viewer.setCameraManipulator( keyswitchManipulator.get() );
    }

    // add the state manipulator
    viewer.addEventHandler( new osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()) );
    
    // add the thread model handler
    viewer.addEventHandler(new osgViewer::ThreadingHandler);

    // add the window size toggle handler
    viewer.addEventHandler(new osgViewer::WindowSizeHandler);
        
    // add the stats handler
    viewer.addEventHandler(new osgViewer::StatsHandler);

    // add the help handler
    viewer.addEventHandler(new osgViewer::HelpHandler(arguments.getApplicationUsage()));

    // add the record camera path handler
    viewer.addEventHandler(new osgViewer::RecordCameraPathHandler);

    // add the LOD Scale handler
    viewer.addEventHandler(new osgViewer::LODScaleHandler);

    // add the screen capture handler
    viewer.addEventHandler(new osgViewer::ScreenCaptureHandler);
    
    // Register shader generator callback
    ShaderGenReadFileCallback *readFileCallback = new ShaderGenReadFileCallback;
    // All read nodes will inherit root state set.
    osg::Group *g=new osg::Group;
    osg::StateSet *ss=g->getOrCreateStateSet();
    std::vector<osg::Uniform *> shared_uniforms;
    vector<float>current_attributes(1224,0.0);
    for(int i=0; i< 1224; i++){
        current_attributes[i]=(float)i;
    }
    float maxel=*std::max_element(current_attributes.begin(),current_attributes.end());
    float minel=*std::min_element(current_attributes.begin(),current_attributes.end());

    shared_uniforms.resize(NUM_UNI_ENUM);
    shared_uniforms[UNI_SHADER_OUT]= new osg::Uniform("shaderOut",1);
    shared_uniforms[UNI_COLORMAP_SIZE]=new osg::Uniform("colormapSize",0);
    shared_uniforms[UNI_DATAUSED]= new osg::Uniform("dataused",0);
    shared_uniforms[UNI_VAL_RANGE]= new osg::Uniform("valrange",osg::Vec2(minel,maxel));
    shared_uniforms[UNI_TEXSCALE]= new osg::Uniform("texScale",256.0f);
    for(int i=0; i < shared_uniforms.size(); i++)
        ss->addUniform(shared_uniforms[i]);
    ss->getUniform("texScale")->set(256.0f);
    osg::Texture2D *shared_tex=new osg::Texture2D();

    shared_tex->setNumMipmapLevels(0);
    shared_tex->setFilter(osg::Texture::MIN_FILTER , osg::Texture::LINEAR);
    shared_tex->setFilter(osg::Texture::MAG_FILTER , osg::Texture::LINEAR);
    shared_tex->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP);
    shared_tex->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP);
    shared_tex->setWrap(osg::Texture::WRAP_R, osg::Texture::CLAMP);
    osg::Image *dataImage=new osg::Image;
    int dim=256;
    dataImage->allocateImage(dim,dim, 1, GL_RGBA,GL_UNSIGNED_BYTE);
    unsigned char* dataPtr= (unsigned char*)dataImage->data();
    memset(dataPtr,0,dataImage->getImageSizeInBytes());

printf("min %f %f\n",minel,maxel);
    for(int i=0;  i < dim*dim; i++){
       // osg::Vec2f add=addrTranslation_1Dto2D((float)i,osg::Vec2f((float)dim,(float)dim));
        if(i==273){
            printf("current_attributes[%d] %f\n",i,current_attributes[i]);
            float s=((current_attributes[i])-minel)/(maxel-minel);
            osg::Vec4f f=pack_depth(s);
            osg::Vec4ub l(f[0]*255.0,f[1]*255.0,f[2]*255.0,f[3]*255.0);

            *(((osg::Vec4ub*)dataPtr)+i) = EncodeDepth(s);//l;
        }

    //    printf("1d %0.1f = (%0.20f %0.20f)\n",(float)i,add.x(),add.y());
      //  printf("%0.1f = %d\n",(current_attributes[i]), triangle(dataImage,add.x(),add.y()).r());
    }

  /*  vector<float> tmp;
    for(int i=0; i< 100; i++)
    tmp.push_back(153.723f+(rand()/(float)RAND_MAX));

      maxel=*std::max_element(tmp.begin(),tmp.end());
      minel=*std::min_element(tmp.begin(),tmp.end());
     for(int i=0; i<tmp.size(); i++){
         float s=(tmp[i]-minel)/(maxel-minel);
         osg::Vec4f f=pack_depth(s);
         osg::Vec4ub l(f[0]*255.0,f[1]*255.0,f[2]*255.0,f[3]*255.0);
         osg::Vec4f f2(l[0]/255.0,l[1]/255.0,l[2]/255.0,l[3]/255.0);
    //osg::Vec4f f2=osg::Vec4f(f[0]/255.0,f[1]/255.0,f[2]/255.0,f[3]/255.0);
    float t=(unpack_depth(f2)*(maxel-minel))+minel;
  //  float t2=(DecodeDepth(f2)*(maxel-minel))+minel;
   // cout <<f<<endl;
  //  cout <<f2<<endl;


    printf("Envcode %f Deccoce %f \n",tmp[i],t);
}*/
    /*for(int i=0; i <  dim*dim*4; i++){
        osg::Vec2f add=addrTranslation_1Dto2D((float)i,osg::Vec2f((float)dim,(float)dim));


        *(dataPtr+(i)) = (unsigned char)clamp(255,0,255);
        //printf("1d %0.1f = (%0.20f %0.20f)\n",(float)i,add.x(),add.y());
        //printf("%0.1f = %d\n",(current_attributes[i]), triangle(dataImage,add.x(),add.y()).r());
    }*/


    shared_tex->setImage(dataImage);
    ss->setTextureAttribute(TEXUNIT_ATTRIB,shared_tex);

    readFileCallback->setRootStateSet(ss);

  //  ss->setTextureAttribute(TEXUNIT_ATTRIB,_dataModel.getSharedTex());
    osgDB::Registry::instance()->setReadFileCallback(readFileCallback);

    // load the data
    osg::ref_ptr<osg::Node> loadedModel = osgDB::readNodeFiles(arguments);
    if (!loadedModel) 
    {
        std::cout << arguments.getApplicationName() <<": No data loaded" << std::endl;
        return 1;
    }

    // any option left unread are converted into errors to write out later.
    arguments.reportRemainingOptionsAsUnrecognized();

    // report any errors if they have occurred when parsing the program arguments.
    if (arguments.errors())
    {
        arguments.writeErrorMessages(std::cout);
        return 1;
    }
    g->addChild(loadedModel.get());
    viewer.setSceneData( g);

    viewer.realize();

    return viewer.run();

}
