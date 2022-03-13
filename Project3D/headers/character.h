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
    GLdouble gX; 
    GLdouble gY;
    GLdouble gZ;
    GLdouble totalHeight;
    GLdouble legHeight;
    GLdouble legWidth;
    GLdouble bodyHeight;
    GLdouble bodyWidth;
    GLdouble radiusHead;
    GLdouble armHeight ;
    GLdouble armWidth;
    GLdouble leg1Theta1;
    GLdouble leg1Theta2;
    GLdouble leg2Theta1;
    GLdouble leg2Theta2;
    GLdouble armThetaXZ;
    GLdouble armThetaXY;
    GLdouble speed;
    GLdouble jumpingSpeed;
    GLdouble jumpingGround;
    bool isJumping;
    bool hasJumpedToMax;
    GLdouble da;
    GLdouble da2;
    GLdouble groundLimit;
    vec3 bodyColor;
    bool isShooting;
    GLint ammo;
    GLint totalAmmo;
    vec3 shootColor;
    GLdouble lookingAngle;
    vec3 directionVector;

    void DrawCharacter(GLdouble x, GLdouble y, GLdouble gZ);
    void DrawRectangle(GLdouble height, GLdouble width, GLdouble depth, GLfloat R, GLfloat G, GLfloat B);
    void DrawCircle(GLdouble radius, GLfloat R, GLfloat G, GLfloat B);
    void DrawLegs();
    void DrawArms();
    void DrawHitbox();
    void DrawAxes();
    bool CollidesHorizontallyWithAPlatform(Map* map);
    bool CollidesDownWithAPlatform(Map* map);
    bool CollidesUpWithAPlatform(Map* map);

public:
    bool isFacingForward;
    Character(GLdouble x, GLdouble y, GLdouble totalHeight, GLdouble groundLimit, vec3 bodyColor, vec3 shootColor, GLint totalAmmo) {
        gX = x; 
        gY = y;
        gZ = y/4;

        this->totalHeight = totalHeight;
        leg1Theta1 = 0;
        leg1Theta2 = 0;
        leg2Theta1 = 0;
        leg2Theta2 = 0;
        armThetaXZ = 0;
        armThetaXY = 0;
        legHeight = totalHeight * 0.2;
        legWidth = totalHeight * 0.1;
        bodyHeight = totalHeight * 0.4;
        bodyWidth = totalHeight *0.2;
        radiusHead = totalHeight * 0.1;
        armHeight = totalHeight * 0.3;
        armWidth = totalHeight * 0.1;

        speed = totalHeight * 0.0028;
        this->jumpingSpeed = speed * 2;
        isJumping = false;
        hasJumpedToMax = false;
        da = legMovingSpeed;
        da2 = -legMovingSpeed;
        isFacingForward = true;
        this->groundLimit = groundLimit;
        this->jumpingGround = groundLimit;
        this->bodyColor = bodyColor;
        isShooting = false;
        ammo = totalAmmo;
        this->totalAmmo = 100;
        this->shootColor = shootColor;
        lookingAngle = 0;
        directionVector = {0.0, 0.0, 0.0};
        this->directionVector.x = cos(this->lookingAngle * M_PI/180);
        this->directionVector.z = sin(this->lookingAngle * M_PI/180);

    };
    void Draw(){ 
        DrawCharacter(gX, gY, gZ);
    };
    void MoveInXZ(bool isForward, GLdouble timeDiff, Map* map);
    void AnimateLegs();
    void StartMoving(bool isToRight);
    bool getIsDirectionToRight();
    void MoveInY(GLdouble timeDiff, bool isPressingJumpButton, Map* map);
    void StartStanding();
    void StartJumping();
    bool getIsJumping();
    vec3 getPosition();
    GLdouble GetgX();
    GLdouble GetgY();
    GLdouble GetgZ() {
        return this->gZ;
    }
    GLdouble GetCharacterGroundY();
    GLdouble GetCharacterHighestY();
    GLdouble GetBodyWidth();
    GLdouble GetBodyHeight();
    GLdouble GetTotalHeight();
    void MoveArmsAngle(GLdouble x, GLdouble y, GLdouble z);
    Shot* Shoot();
    void RechargeShot();
    bool CollidesWithPoint(GLdouble x, GLdouble y, GLdouble z);
    bool CollidesWithEndOfMap(Map* map);
    void TurnRight(GLdouble timeDiff);
    void TurnLeft(GLdouble timeDiff);
};

#endif	/* CHARACTER_H */

