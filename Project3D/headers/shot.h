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
    GLdouble gZ; 
    GLdouble gVel;
    GLdouble shotRadius;
    GLboolean hitted;
    vec3 directionVector;
    vec3 color;
private:
    void DrawCirc(GLdouble radius, GLfloat R, GLfloat G, GLfloat B);
    void DrawShot(GLdouble x, GLdouble y, GLdouble z);
public:
    Shot(GLdouble x, GLdouble y, GLdouble z, vec3 directionVector, GLdouble shotRadius, GLdouble speed, vec3 color){
        gXInit = x; 
        gYInit = y; 
        this->directionVector = directionVector;
        gX = x; 
        gY = y; 
        gZ = z;
        gVel = speed;
        this->shotRadius = shotRadius;
        this->color = color;
        hitted = false;
    };
    void Draw(){ 
        DrawShot(gX, gY, gZ);
    };
    void Move(GLdouble timeDifference);
    bool Valid(Map* map);
    vec3 GetPos(){
        return vec3(this->gX, this->gY, this->gZ);
    };
    GLfloat GetRadius();
    void SetHitted() {
        hitted = true;
    }
};

#endif	/* TIRO_H */

