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
class Enemy; // Forward declaration para evitar dependencia ciclica

class Map {
    GLfloat gX; 
    GLfloat gY;
    GLfloat sizeX;
    GLfloat sizeY;
    list<Platform> platforms;
    list<Enemy*> enemies;
    int gColor;

    void DrawMap(GLfloat x, GLfloat y);
    void DrawRectangle(GLfloat height, GLfloat width, GLfloat R, GLfloat G, GLfloat B);
    void DrawPlatforms();
    void DrawEnemies();
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
    bool ColidesWithAPlatform(GLfloat x, GLfloat y);
    void AddPlatform(Platform p);
    GLfloat GetgX();
    GLfloat GetgY();
    GLfloat GetSizeX();
    GLfloat GetSizeY();
    void AddEnemy(Enemy* e);
    void ExecuteEnemiesActions(GLdouble timeDifference);
    void ChangeEnemiesActions();
    // bool Atingido(Tiro *tiro);
};

#endif	/* MAP_H */

