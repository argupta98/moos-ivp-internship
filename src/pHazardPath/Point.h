#ifndef POINT_DEF
#define POINT_DEF

#include <iostream>
#include "MOOS/libMOOS/MOOSLib.h"

using namespace std;

class Point
{
 public:
  Point(string point_rep);
  double distance_to(Point other);
  bool equals(Point other);
  bool in(vector<Point> vec);
 public:
  string point_string;
  int x_val;
  int y_val;
  int point_id;
  bool west;
};

#endif
