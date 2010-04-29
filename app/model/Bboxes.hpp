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

        Visitor() : count(0), ContinueVisiting(true) {};

        void operator()(const RTree::Leaf * const leaf)
        {
                count++;
        }
};
extern std::map<int,std::string> texture_file_names;
//extern std::vector<GtsBBox *> bboxes_all;
//extern std::map<int,bbox_map_info> gts_trans_map;
bbox_map_info * find_closet_img_idx(RTree *tree,osg::Vec3 pt);
extern RTree *g_bboxtree;
extern bbox_map_info *cur_info;
extern bool validTerrainMouseOver;
RTree *loadBBox(const char *str);



#endif
