#ifndef UTILS_H
#define	UTILS_H
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

struct vec3 {
	float x, y, z;
	vec3() :x(0.0), y(0.0), z(0.0) {}
	vec3(float x_, float y_, float z_) :x(x_), y(y_), z(z_) {}
	vec3 operator+(const vec3& v)const { return vec3(x + v.x, y + v.y, z + v.z); }
	vec3 operator*(float k) { return vec3(x*k, y*k, z*k); }
};

#endif