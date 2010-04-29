#ifndef BBOXES_H
#define BBOXES_H
#include <stdlib.h>
#include <stdio.h>
#include <RStarTree.h>
#include <map>
#include <vector>
#include <string>
#include <osg/Vec3>
using namespace std;

typedef struct _bbox_map_info{
  string leftname;
  string rightname;
  double time;
  int count;
}bbox_map_info;
typedef RStarTree<bbox_map_info, 3, 8, 64> 	RTree;
typedef RTree::BoundingBox			BoundingBox;
struct Visitor {
        int count;
        bool ContinueVisiting;
        const RTree::Leaf *found;
        Visitor() : count(0), ContinueVisiting(true),found(NULL) {};

        void operator()(const RTree::Leaf * const leaf)
        {
            count++;
            found=leaf;
            ContinueVisiting=false;

        }
};

bool find_closet_img_idx(RTree *tree,osg::Vec3 pt,bbox_map_info &boxinfo);
RTree *loadBBox(const char *str);



#endif
