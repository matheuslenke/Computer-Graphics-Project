#ifndef TIRO_H
#define	TIRO_H
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include "map.h"

class Shot {
    GLdouble gXInit; 
    GLdouble gYInit; 
    GLdouble gX; 
    GLdouble gY; 
    GLfloat gDirectionAng;
    GLfloat gVel;
    GLfloat shotRadius;
private:
    void DrawCirc(GLdouble radius, GLfloat R, GLfloat G, GLfloat B);
    void DrawShot(GLdouble x, GLdouble y);
public:
    Shot(GLdouble x, GLdouble y, GLdouble directionAng, GLdouble shotRadius, GLdouble speed){
        gXInit = x; 
        gYInit = y; 
        gX = x; 
        gY = y; 
        gDirectionAng = directionAng; 
        gVel = speed;
        this->shotRadius = shotRadius;
    };
    void Draw(){ 
        DrawShot(gX, gY);
    };
    void Move(GLdouble timeDifference);
    bool Valid(Map* map);
    void GetPos(GLfloat &xOut, GLfloat &yOut){
        xOut = gX;
        yOut = gY;
    };
};

#endif	/* TIRO_H */

