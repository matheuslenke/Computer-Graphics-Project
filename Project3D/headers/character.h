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
#define legMovingSpeed 2.5
#define characterAmmo 1
#define turningSpeed 0.1

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
    GLfloat gZ;
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
    GLdouble jumpingSpeed;
    GLfloat jumpingGround;
    bool isJumping;
    bool hasJumpedToMax;
    GLfloat da;
    GLfloat da2;
    GLfloat groundLimit;
    vec3 bodyColor;
    bool isShooting;
    GLint ammo;
    GLint totalAmmo;
    vec3 shootColor;
    GLfloat lookingAngle;

    void DrawCharacter(GLfloat x, GLfloat y, GLfloat gZ);
    void DrawRectangle(GLfloat height, GLfloat width, GLfloat depth, GLfloat R, GLfloat G, GLfloat B);
    void DrawCircle(GLfloat radius, GLfloat R, GLfloat G, GLfloat B);
    void DrawLegs();
    void DrawArms();
    void DrawHitbox();
    bool CollidesLeftWithAPlatform(GLfloat inc, Map* map);
    bool CollidesRightWithAPlatform(GLfloat inc, Map* map);
    bool CollidesDownWithAPlatform(GLfloat inc, Map* map);
    bool CollidesUpWithAPlatform(GLfloat inc, Map* map);

public:
    bool isFacingRight;
    Character(GLfloat x, GLfloat y, GLfloat totalHeight, GLfloat groundLimit, vec3 bodyColor, vec3 shootColor, GLint totalAmmo) {
        gX = x; 
        gY = y;
        gZ = y/4;

        this->totalHeight = totalHeight;
        leg1Theta1 = 0;
        leg1Theta2 = 0;
        leg2Theta1 = 0;
        leg2Theta2 = 0;
        armTheta = 90;
        legHeight = totalHeight * 0.2;
        legWidth = totalHeight * 0.1;
        bodyHeight = totalHeight * 0.4;
        bodyWidth = totalHeight *0.2;
        radiusHead = totalHeight * 0.1;
        armHeight = totalHeight * 0.3;
        armWidth = totalHeight * 0.1;

        speed = totalHeight * 0.0028;
        this->jumpingSpeed = speed * 2;
        isJumping = true;
        hasJumpedToMax = false;
        da = legMovingSpeed;
        da2 = -legMovingSpeed;
        isFacingRight = true;
        this->groundLimit = groundLimit;
        this->jumpingGround = groundLimit;
        this->bodyColor = bodyColor;
        isShooting = false;
        ammo = totalAmmo;
        this->totalAmmo = totalAmmo;
        this->shootColor = shootColor;
        lookingAngle = 0;

    };
    void Draw(){ 
        DrawCharacter(gX, gY, gZ);
    };
    void MoveInXZ(bool isToRight, GLdouble timeDiff, Map* map);
    void StartMoving(bool isToRight);
    bool getIsDirectionToRight();
    void MoveInY(GLdouble timeDiff, bool isPressingJumpButton, Map* map);
    void StartStanding();
    void StartJumping();
    bool getIsJumping();
    vec3 getPosition();
    GLfloat GetgX();
    GLfloat GetgY();
    GLfloat GetgZ() {
        return this->gZ;
    }
    GLfloat GetCharacterGroundY();
    GLfloat GetCharacterHighestY();
    GLfloat GetBodyWidth();
    GLfloat GetBodyHeight();
    GLfloat GetTotalHeight();
    void MoveArmsAngle(GLfloat x, GLfloat y);
    Shot* Shoot();
    void RechargeShot();
    bool CollidesWithPoint(GLfloat x, GLfloat y);
    bool CollidesWithEndOfMap(Map* map);
    void TurnRight(GLdouble timeDiff);
    void TurnLeft(GLdouble timeDiff);
};

#endif	/* CHARACTER_H */

