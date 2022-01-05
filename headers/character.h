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

// Dimensions
#define legMovingSpeed 2.0
#define characterSpeed 0.1
#define characterJumpingSpeed 0.2

class Character {
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
    GLboolean isFacingRight;
    GLboolean isLeg1PrimaryMoving;
    GLboolean isJumping;
    GLboolean hasJumpedToMax;
    GLfloat da;
    GLfloat da2;
    int gColor;
private:
    void DrawCharacter(GLfloat x, GLfloat y);
    void DrawRectangle(GLint height, GLint width, GLfloat R, GLfloat G, GLfloat B);
    void DrawCircle(GLint radius, GLfloat R, GLfloat G, GLfloat B);
    void DrawLegs(GLfloat x, GLfloat y);
public:
    Character(GLfloat x, GLfloat y, GLfloat totalHeight){
        gX = x; 
        gY = y;
        gColor = 0;
        speed = characterSpeed;
        isFacingRight = true;
        isLeg1PrimaryMoving = true;
        this->totalHeight = totalHeight;
        legHeight = totalHeight * 0.2;
        legWidth = totalHeight * 0.1;
        bodyHeight = totalHeight * 0.4;
        bodyWidth = totalHeight *0.2 ;
        radiusHead = totalHeight * 0.1;
        armHeight = totalHeight * 0.4;
        armWidth = 10;
        da = legMovingSpeed;
        da2 = -legMovingSpeed;
        this->jumpingSpeed = characterJumpingSpeed;
        isJumping = false;
        hasJumpedToMax = false;
    };
    void Draw(){ 
        DrawCharacter(gX, gY);
    };
    void MoveInX(bool isToRight, GLdouble timeDiff);
    void StartMoving(bool isToRight);
    bool getIsDirectionToRight();
    void MoveInY(GLdouble timeDiff, bool isPressingJumpButton);
    void StartJumping();
    bool getIsJumping();
    vec3 getPosition();
    // void Recria(GLfloat x, GLfloat y);
    // bool Atingido(Tiro *tiro);
};

#endif	/* CHARACTER_H */

