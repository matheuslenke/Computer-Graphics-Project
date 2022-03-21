#ifndef COLLISION_H
#define COLLISION_H

#include <tgmath.h> 
#include <iostream>

#include "geom_draw.h"
#include "color.h"
#include "point.h"

// axis-aligned bounding boxes (AABB)
struct Aabb{
  GLdouble xmin;
  GLdouble xmax;
  GLdouble ymin;
  GLdouble ymax;
  GLdouble zmin;
  GLdouble zmax;

  Aabb(){
    xmin = 0.;
    xmax = 0.;
    ymin = 0.;
    ymax = 0.;
    zmin = 0.;
    zmax = 0.;
  };
  Aabb( GLdouble x_min,GLdouble x_max, GLdouble y_min, GLdouble y_max, GLdouble z_min, GLdouble z_max){
    xmin = x_min;
    xmax = x_max;
    ymin = y_min;
    ymax = y_max;
    zmin = z_min;
    zmax = z_max;
  };

  bool collides(Aabb b){
    return (xmin <= b.xmax && xmax >= b.xmin) 
    && (ymin <= b.ymax && ymax >= b.ymin)
    && (zmin <= b.zmax && zmax >= b.zmin);
  };

  bool collides(Point p){
    return (p.x >= xmin  && p.x <= xmax) 
    && (p.y >= ymin  && p.y <= ymax)
    && (p.z >= zmin  && p.z <= zmax);
  };

  void draw(){
    drawRectangle(xmin, ymin, xmax-xmin, ymax-ymin, Color::yellow());
  };

  void printf(){
    std::printf("xmin: %f, xmax: %f, ymin: %f, ymax: %f, zmin: %f, zmax: %f\n", xmin,xmax,ymin,ymax,zmin,zmax);
  }
};

#endif