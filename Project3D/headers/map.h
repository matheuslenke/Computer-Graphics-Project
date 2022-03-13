#ifndef MAP_H
#define	MAP_H
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif
#include <vector>
#include "platform.h"
#include "utils.h"

// Dimensions
using namespace std;
// Forward declaration para evitar dependencia ciclica
class Enemy; 
class Character;
class Shot;

class Map {
    GLfloat gX; 
    GLfloat gY;
    GLfloat gZ;
    GLfloat sizeX;
    GLfloat sizeY;
    GLfloat sizeZ;
    vector<Platform> platforms;
    vector<Enemy*> enemies;
    vector <Shot*> enemyShots;
    int gColor;

    void DrawMap(GLfloat x, GLfloat y, GLfloat z);
    void DrawRectangle(GLfloat height, GLfloat width, GLfloat R, GLfloat G, GLfloat B);
    void DrawPlatforms();
    void DrawEnemies();
    void DrawOutline();
public:
    Map(GLfloat x, GLfloat y, GLfloat sizeX, GLfloat sizeY){
        gX = x; 
        gY = y;
        gZ = 0;
        this->sizeX = sizeX;
        this->sizeY = sizeY;
        this->sizeZ = sizeY/2;
    };
    ~Map();
    void Draw(){ 
        DrawMap(gX, gY, gZ);
    };
    void CreateMapFromSVG();
    bool ColidesWithAPlatform(GLfloat x, GLfloat y, GLfloat z);
    vec2* GetPlatformLimitsAtPoint(GLfloat x, GLfloat y);
    void AddPlatform(Platform p);
    GLfloat GetgX();
    GLfloat GetgY();
    GLfloat GetSizeX();
    GLfloat GetSizeY();
    GLfloat GetSizeZ() {
        return this->sizeZ;
    }
    void AddEnemy(Enemy* e);
    void ExecuteEnemiesActions(GLdouble timeDifference, Character* player, GLboolean stopMoving);
    void ChangeEnemiesActions();
    void RechargeEnemies();
    void DrawShots();
    void MoveShots(GLdouble timeDifference);
    void CheckIfEnemyIsHit(vector<Shot*> playerShots);
    GLboolean CheckIfPlayerIsHit(Character* player);
    bool ColidesWithRightEnd(GLfloat x);
    bool CollidesWithEnemy(GLfloat x, GLfloat y, GLfloat z);
};

#endif	/* MAP_H */

