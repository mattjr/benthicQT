/**
 * Reads in a reference surface and the actual surface.
 * These files are produced by running the x,y,z data through
 * the program DTMConverter.
 * The files can be in binary or text format.
 * The outut is five estimates of volume and five files called
 * FIIK0.vtp - no cuboids.
 * FIIK1.vtp - all cuboids.
 * FIIK2.vtp - cuboids lying above or on the reference surface.
 * FIIK3.vtp - cuboids lying below or on the reference surface.
 * FIIK4.vtp - cuboids lying above and below reference surface.
 * The estimates of volume are based on the properties of the cuboids.
 * You can run ParaView and load FIIK0.vtp, then step through the frames to see
 * the volumes matching the cuboid types.
*/

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdexcept>
#include <sstream>

#include "ufDTMImage.h"
//#include "ufDTMVolume.h"
#include "ufDTMVolumeView.h"

#include <vtkPolyData.h>
#include <vtkXMLPolyDataWriter.h>

using namespace UF;
using namespace UF::DTM;

/**
 * Read in the data.
 */
void ReadInData ( std::ifstream &ifs, DTM::DTMImage &Map, bool IsBinary = true )
{
  if ( IsBinary )
    {
    Map.ReadBinaryHeader(ifs);
    if ( Map.GetScalarType() == 11 )
      Map.ReadBinaryScalars<double>(ifs);
    }
  else
    Map.ReadTextImageData(ifs);
}

/**
 *
 * @param argc
 * @param argv[]
 * @return
 */
int main ( int argc, char* argv[] )
{
  if ( argc != 3 )
    {
//    std::cout << "Need two input files (reference and actual map) and an output file." << std::endl;
    std::cout << "Need two input files (reference and actual map)." << std::endl;
    return 1;
    }

  std::string ifnRef = argv[1];
  std::string ifnAct = argv[2];
//  std::string ofn = argv[3];

  std::cout << "Reference Map: " << ifnRef << std::endl;
  std::cout << "Actual Map: " << ifnAct << std::endl;
//  std::cerr << "Output file: " << ofn << std::endl;

  if ( ifnRef == ifnAct )
   {
   std::cout << "File names cannot be the same." << std::endl;
   return 1;
   }

  std::ifstream ifsRef(ifnRef.c_str(),std::ios::binary);
  if ( !ifsRef )
    {
    std::cout << "Opening of:" << ifnRef << " failed." << std::endl;
    return 1;
    }

  std::ifstream ifsAct(ifnAct.c_str(),std::ios::binary);
  if ( !ifsAct )
    {
    std::cout << "Opening of:" << ifnAct << " failed." << std::endl;
    return 1;
    }
//   std::ofstream ofs(ofn.c_str(),std::ios::binary);
//   if ( !ofs )
//     {
//     std::cout << "Opening of:" << ofn << " failed." << std::endl;
//     return 1;
//     }

//    DTM::DTMVolume Surfs;
    DTM::DTMVolumeView Surfs;
    DTM::DTMImage Map[2];

  ReadInData ( ifsRef, Map[0]); // Reading binary data.
  ReadInData ( ifsAct, Map[1]);

  int dimensions[2][3];
  double origin[2][3];
  double spacing[2][3];
  int indexIncrement[2][3];
  for ( int i = 0; i < 2; ++i )
    {
    Map[i].GetDimensions(dimensions[i]);
    Map[i].GetOrigin(origin[i]);
    Map[i].GetSpacing(spacing[i]);
    Map[i].GetIndexIncrement(indexIncrement[i]);
    }

  for ( int i = 0; i < 2; ++i )
    {
      if ( i == 0 )
        std::cout << "         reference origin: ";
      else
        std::cout << "            actual origin: ";
      for ( int j = 0; j < 3; ++j ) std::cout << " " << std::fixed << std::setprecision(2) << origin[i][j];
      std::cout << std::endl;
      if ( i == 0 )
        std::cout << "     reference dimensions: ";
      else
        std::cout << "        actual dimensions: ";
      for ( int j = 0; j < 3; ++j ) std::cout << " " << dimensions[i][j];
      std::cout << std::endl;
      if ( i == 0 )
        std::cout << "        reference spacing: ";
      else
        std::cout << "           actual spacing: ";
      for ( int j = 0; j < 3; ++j ) std::cout << " " << spacing[i][j];
      std::cout << std::endl;
      if ( i == 0 )
        std::cout << "reference index increment: ";
      else
        std::cout << "   actual index increment: ";
      for ( int j = 0; j < 3; ++j ) std::cout << " " << indexIncrement[i][j];
      std::cout << std::endl;
    }

  try {

    Surfs.SetReferenceSurface(Map[0]);
    Surfs.SetActualSurface(Map[1]);
    // Surfs.CopyActualSurfaceToReferenceSurface();
    // We can filter.
/*
    double Range[2] = {100,300};
    Surfs.SetRange(Range);
    Surfs.FilterOnRangeOn();
    double Height[2] = {1,40};
    Surfs.SetHeight(Height);
    Surfs.FilterOnHeightOff();
    Surfs.SetTolerance(0.01);
    Surfs.FilterOnToleranceOff();
*/
    double v1 = 0;
    double v2 = 0;
    vtkSmartPointer<vtkXMLPolyDataWriter> pdw = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
    vtkPolyData * pd;
    std::string leaf = "FIIK";
    std::string ext = "vtp";
    std::string fn;
    std::ostringstream os;
    for ( int i = 0; i < 5; ++i )
    {
      switch ( i )
     {
       case 0:
         std::cout << "No cuboids selected." << std::endl;
         break;
       case 1:
         std::cout << "All cuboids selected." << std::endl;
         break;
       case 2:
         std::cout << "Cuboids whose volume above the reference surface is > 0 and volume below the reference surface == 0." << std::endl;
         break;
       case 3:
         std::cout << "Cuboids whose volume above the reference surface is == 0 and volume below the reference surface > 0." << std::endl;
         break;
       case 4:
         std::cout << "Cuboids whose volume above the reference surface is > 0 and volume below the reference surface > 0." << std::endl;
         break;
     }

      std::cout << "Reference surface scalars: " << Surfs.GetReferenceSurface()->GetScalars<double>()->size() << std::endl;
      std::cout << "   Actual surface scalars: " << Surfs.GetActualSurface()->GetScalars<double>()->size() << std::endl;
      Surfs.SetCuboidVolumeSelector(i);
      Surfs.DTMVolume::CalculateVolume(v1,v2);
      std::cout << std::setprecision(2) << "Volumes: (+ve: " << std::fixed << v1 << ", -ve: " << v2 << ") Total: " << v1 + v2 << std::endl;
      Surfs.CalculateVolume(v1,v2);
      std::cout << std::setprecision(2) << "Volumes: (+ve: " << std::fixed << v1 << ", -ve: " << v2 << ") Total: " << v1 + v2 << std::endl;
      pd = Surfs.GetCuboids();
      os << i;
      fn = leaf + os.str() + "." + ext;
      os.str("");
      pdw->SetFileName(fn.c_str());
      pdw->SetInput(pd);
      pdw->Write();
    }

  }
  catch (const std::exception &e) {
     std::cerr << " EXCEPTION: " << e.what() << std::endl;
  }


  return 0;
}
