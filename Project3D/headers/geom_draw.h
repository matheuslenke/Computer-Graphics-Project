#ifndef GEOM_DRAW_H
#define	GEOM_DRAW_H
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#define WIDTH_FROM_BOTTOM -1
#define WIDTH_FROM_CENTER 0

void drawRectangle(GLdouble height, GLdouble width, GLdouble depth, GLfloat R, GLfloat G, GLfloat B, int widthDrawStart = WIDTH_FROM_BOTTOM);
void drawRectangleTextured(GLdouble height, GLdouble width, GLdouble depth, GLfloat R, GLfloat G, GLfloat B, GLuint texture,  float texSizeFactor=1, int slices=1 );



#endif	/* GEOM_DRAW_H */
