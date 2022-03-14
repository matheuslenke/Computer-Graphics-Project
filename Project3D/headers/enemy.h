#ifndef ENEMY_H
#define	ENEMY_H
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include "utils.h"
#include "character.h"

#include <vector>
#include <iostream>

using namespace std;

class Enemy : public Character {
protected:
    /* 0 - Andar
       1 - Atirar
       2 - Ficar parado
    */
    vector<int> actingPattern;
    int actualAction;
    GLboolean foundGround = false;
    GLboolean isOnPlatform = false;
    GLdouble minWalkingX;
    GLdouble maxWalkingX;
    GLboolean CollidesHorizontallyWithAPlatform(Map* map);
    GLboolean CollidesUpWithCharacter(Character* character);
    GLboolean CollidesDownWithCharacter(Character* character);
    GLboolean CollidesRightWithCharacter(Character* character);
    GLboolean CollidesLeftWithCharacter(Character* character);
public:
    Enemy(GLfloat x, GLfloat y, GLfloat totalHeight, GLfloat groundLimit, vec3 bodyColor, vec3 shootColor, GLint ammo) : Character(x, y, totalHeight, groundLimit, bodyColor, shootColor, ammo) {
        actualAction = 0;
    }
    void DrawEnemy() {
        Character::Draw();
    }
    void MoveInY(GLdouble timeDiff, Map* map, Character* player);
    void MoveInX(GLdouble timeDiff, Map* map, Character* player);
    void InsertAction(int action);
    void NextAction();
    Shot* DoAction(GLfloat timeDiff, Map* map, Character* player, GLboolean stopMoving);
    void AdjustArms(vec3 playerPosition);
};

#endif	/* ENEMY_H */

