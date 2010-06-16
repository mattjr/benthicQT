#!/usr/bin/python
#
# Andrew J. P. Maclean
# 13 Oct 2005
#
# Take a polydata file and write out the point cloud.

import string
import vtk

from Numeric import *
import sys
from sys import argv

if len(argv)>1:
        filename = argv[1]
        output = str(filename[:-3])+"txt"

else :
        print """Usage: 'python VTKToPoints.py file'
Writes out the point cloud from a vtk polydata file called
<file> to a file called <file>.txt."""
        sys.exit(0)


r = vtk.vtkDataSetReader()
r.SetFileName(filename)
r.Update()

# If it is an unstructured grid, use this filter
# to convert it to polydata.
g = vtk.vtkGeometryFilter()
g.SetInput(r.GetOutput())

pd = vtk.vtkPolyData();
pd.DeepCopy(r.GetOutput())


# Get the points and write them out.
n = pd.GetNumberOfPoints()
if n <= 0:
	print """No Points to process."""
	sys.exit(0)

print """Number of points: """, n
out = open(output, 'w')
print "Output to:", output

dim = 3
for i in range(n):
	pt = pd.GetPoint(i)
	#print i, pt
	for j in range(dim):
		out.write(str(pt[j]) + " ")
	out.write("\n")
out.close()
