#ifndef PLATFORM_H
#define	PLATFORM_H
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif
#include "utils.h"

// Dimensions

class Platform {
    GLfloat gX; 
    GLfloat gY;
    GLfloat gZ;
    GLfloat sizeX;
    GLfloat sizeY;
    GLfloat sizeZ;
    int gColor;
private:
    void DrawPlatform(GLfloat x, GLfloat y, GLfloat z);
    void DrawRectangle(GLfloat height, GLfloat width, GLfloat depth, GLfloat R, GLfloat G, GLfloat B);
public:
    Platform(GLfloat x, GLfloat y, GLfloat sizeX, GLfloat sizeY, GLfloat sizeZ){
        gX = x; 
        gY = y;
        gZ = 0;
        this->sizeX = sizeX;
        this->sizeY = sizeY;
        this->sizeZ = sizeZ;
    };
    void Draw(){ 
        DrawPlatform(gX, gY, gZ);
    };
    bool isColiding(GLfloat x, GLfloat y);
    vec2 *ReturnPlatformXLimits() {
        vec2* pos = new vec2(this->gX, this->gX + sizeX);
        return pos;
    }
    // void Recria(GLfloat x, GLfloat y);
    // bool Atingido(Tiro *tiro);
};

#endif	/* PLATFORM_H */

