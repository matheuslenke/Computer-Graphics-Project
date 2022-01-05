#ifndef MAP_H
#define	MAP_H
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif
#include <list>
#include "platform.h"

// Dimensions
using namespace std;

class Map {
    GLfloat gX; 
    GLfloat gY;
    GLfloat sizeX;
    GLfloat sizeY;
    list<Platform> platforms;
    int gColor;
private:
    void DrawMap(GLfloat x, GLfloat y);
    void DrawRectangle(GLint height, GLint width, GLfloat R, GLfloat G, GLfloat B);
    void DrawPlatforms();
    void DrawOutline();
public:
    Map(GLfloat x, GLfloat y, GLfloat sizeX, GLfloat sizeY){
        gX = x; 
        gY = y;
        this->sizeX = sizeX;
        this->sizeY = sizeY;
    };
    void Draw(){ 
        DrawMap(gX, gY);
    };
    void CreateMapFromSVG();
    
    // void Recria(GLfloat x, GLfloat y);
    // bool Atingido(Tiro *tiro);
};

#endif	/* MAP_H */

