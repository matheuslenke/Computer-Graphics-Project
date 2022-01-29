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
#include "utils.h"

class Shot {
    GLdouble gXInit; 
    GLdouble gYInit; 
    GLdouble gX; 
    GLdouble gY; 
    GLdouble gDirectionAng;
    GLdouble gVel;
    GLdouble shotRadius;
    GLboolean hitted;
    vec3 color;
private:
    void DrawCirc(GLdouble radius, GLfloat R, GLfloat G, GLfloat B);
    void DrawShot(GLdouble x, GLdouble y);
public:
    Shot(GLdouble x, GLdouble y, GLdouble directionAng, GLdouble shotRadius, GLdouble speed, vec3 color){
        gXInit = x; 
        gYInit = y; 
        gX = x; 
        gY = y; 
        gDirectionAng = directionAng; 
        gVel = speed;
        this->shotRadius = shotRadius;
        this->color = color;
        hitted = false;
    };
    void Draw(){ 
        DrawShot(gX, gY);
    };
    void Move(GLdouble timeDifference);
    bool Valid(Map* map);
    vec2 GetPos(){
        return vec2(this->gX, this->gY);
    };
    GLfloat GetRadius();
    void SetHitted() {
        hitted = true;
    }
};

#endif	/* TIRO_H */

