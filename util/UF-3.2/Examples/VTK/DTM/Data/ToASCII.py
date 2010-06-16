#!/usr/bin/python
#
# Andrew J. P. Maclean
# 13 Oct 2005
#
# Take a binary file with a vtk extension and convert it
# to an ASCII vtk file.

import string
import vtk

from Numeric import *
import sys
from sys import argv

if len(argv) == 3:
        infilename = argv[1]
        outfilename = argv[2]

else :
        print """Usage: 'python ToASCII.py infile outfile'
Converts the binary vtk polydata file called
<infile> to an ASCII file called <outfile>."""
        sys.exit(0)

if infilename == outfilename:
	print """File names cannot be the same."""
	sys.exit(0)

print "Output to:", outfilename

r = vtk.vtkDataSetReader()
r.SetFileName(infilename)
r.Update()

# If it is an unstructured grid, use this filter
# to convert it to polydata.
#g = vtk.vtkGeometryFilter()
#g.SetInput(r.GetOutput())

# Write it out.
w = vtk.vtkDataSetWriter()
w.SetInput( r.GetOutput() )
#w.SetInput( g.GetOutput() )
w.SetFileName(outfilename)
w.Write()

