#ifndef CHARACTER_H
#define	CHARACTER_H
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif
#include "utils.h"
#include "map.h"
#include "shot.h"
#include <vector>

// Dimensions
#define legMovingSpeed 2.0
#define characterSpeed 0.02
#define characterJumpingSpeed 0.04
#define characterAmmo 1

struct hitBox {
    vec2 upLeft;
    vec2 upRight;
    vec2 downLeft;
    vec2 downRight;
    hitBox(vec2 upl, vec2 upr, vec2 dl, vec2 dr): upLeft(upl), upRight(upr), downLeft(dl), downRight(dr) {}
};

class Character {
protected:
    GLfloat gX; 
    GLfloat gY;
    GLfloat totalHeight;
    GLfloat legHeight;
    GLfloat legWidth;
    GLfloat bodyHeight;
    GLfloat bodyWidth;
    GLfloat radiusHead;
    GLfloat armHeight ;
    GLfloat armWidth;
    GLfloat leg1Theta1;
    GLfloat leg1Theta2;
    GLfloat leg2Theta1;
    GLfloat leg2Theta2;
    GLfloat armTheta;
    GLfloat speed;
    GLfloat jumpingSpeed;
    GLfloat jumpingGround;
    GLboolean isJumping;
    GLboolean hasJumpedToMax;
    GLfloat da;
    GLfloat da2;
    GLfloat groundLimit;
    vec3 bodyColor;
    GLboolean isShooting;
    GLint ammo;
    GLint totalAmmo;
    vec3 shootColor;
    vector<Shot*> shots;

    void DrawCharacter(GLfloat x, GLfloat y);
    void DrawRectangle(GLfloat height, GLfloat width, GLfloat R, GLfloat G, GLfloat B);
    void DrawCircle(GLfloat radius, GLfloat R, GLfloat G, GLfloat B);
    void DrawLegs();
    void DrawArms();
    void DrawHitbox();
    bool CollidesLeftWithAPlatform(GLfloat inc, Map* map);
    bool CollidesRightWithAPlatform(GLfloat inc, Map* map);
    bool CollidesDownWithAPlatform(Map* map);
    bool CollidesUpWithAPlatform(Map* map);

public:

    GLboolean isFacingRight;
    Character(GLfloat x, GLfloat y, GLfloat totalHeight, GLfloat groundLimit, vec3 bodyColor, vec3 shootColor, GLint totalAmmo) {
        gX = x; 
        gY = y;
        speed = characterSpeed;
        isFacingRight = true;
        this->totalHeight = totalHeight;
        leg1Theta1 = 0;
        leg1Theta2 = 0;
        leg2Theta1 = 0;
        leg2Theta2 = 0;
        legHeight = totalHeight * 0.2;
        legWidth = totalHeight * 0.1;
        bodyHeight = totalHeight * 0.4;
        bodyWidth = totalHeight *0.2;
        radiusHead = totalHeight * 0.1;
        armHeight = totalHeight * 0.3;
        armWidth = totalHeight * 0.1;
        armTheta = 90;
        da = legMovingSpeed;
        da2 = -legMovingSpeed;
        this->jumpingSpeed = characterJumpingSpeed;
        isJumping = false;
        hasJumpedToMax = false;
        this->groundLimit = groundLimit;
        this->jumpingGround = groundLimit;
        this->bodyColor = bodyColor;
        isShooting = false;
        ammo = totalAmmo;
        this->totalAmmo = totalAmmo;
        this->shootColor = shootColor;

    };
    void Draw(){ 
        DrawCharacter(gX, gY);
    };
    void MoveInX(bool isToRight, GLdouble timeDiff, Map* map);
    void StartMoving(bool isToRight);
    bool getIsDirectionToRight();
    void MoveInY(GLdouble timeDiff, bool isPressingJumpButton, Map* map);
    void StartStanding();
    void StartJumping();
    bool getIsJumping();
    vec3 getPosition();
    GLfloat GetgX();
    GLfloat GetgY();
    GLfloat GetCharacterGroundY();
    GLfloat GetCharacterHighestY();
    void MoveArmsAngle(GLfloat x, GLfloat y);
    void Shoot();
    vector<Shot*>& GetShots();
    void RechargeShot();
    // void Recria(GLfloat x, GLfloat y);
    // bool Atingido(Tiro *tiro);
};

#endif	/* CHARACTER_H */

