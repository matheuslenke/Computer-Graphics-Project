#include "headers/character.h"
#include <math.h>
#include <iostream>

void Character::DrawCharacter(GLfloat x, GLfloat y)
{
    glPushMatrix();

    glTranslatef(x, y, 0);

    // Desenha o corpo do personagem
    DrawRectangle(bodyHeight, bodyWidth, 0, 1, 0);

    // Desenha a cabeça do personagem
    glPushMatrix();
    glTranslatef(0, bodyHeight + radiusHead, 0);
    DrawCircle(radiusHead, 0, 1, 0);
    glPopMatrix();

    // Desenha a perna do personagem
    glPushMatrix();
    DrawLegs(x, y);
    glPopMatrix();

    glPopMatrix();
}

void Character::DrawLegs(GLfloat x, GLfloat y) {
    // Desenhando a perna esquerda
    glPushMatrix();
    glTranslatef(0, 0, 0);
    glRotatef(this->leg1Theta1, 0, 0, 1);
    glTranslatef(0, -legHeight, 0);
    DrawRectangle(legHeight, legWidth, 1, 0, 0);

    glRotatef(this->leg1Theta2, 0, 0, 1);
    glTranslatef(0, -legHeight, 0);
    DrawRectangle(legHeight, legWidth, 1, 0, 0);
    glPopMatrix();
    
    // Desenhando a perna direita
    glPushMatrix();
    glTranslatef(0, 0, 0);
    glRotatef(this->leg2Theta1, 0, 0, 1);
    glTranslatef(0, -legHeight, 0);
    DrawRectangle(legHeight, legWidth, 0, 0, 1);

    glRotatef(this->leg2Theta2, 0, 0, 1);
    glTranslatef(0, -legHeight, 0);
    DrawRectangle(legHeight, legWidth, 0, 0, 1);
    glPopMatrix();
}

void Character::DrawRectangle(GLint height, GLint width, GLfloat R, GLfloat G, GLfloat B) {
    /* Define cor dos vértices com os valores R, G e B variando de 0.0 a 1.0 */
    glColor3f (R, G, B);

    glBegin(GL_POLYGON);
        glVertex3f (0, 0, 0.0);
        glVertex3f (width / 2 , 0, 0.0);
        glVertex3f (width / 2 , height, 0.0);
        glVertex3f (- width / 2, height, 0.0);
        glVertex3f (- width / 2, 0, 0.0);
        glVertex3f (0, 0, 0.0);
    glEnd();
}

void Character::DrawCircle(GLint radius, GLfloat R, GLfloat G, GLfloat B)
{
    glColor3f(R, G, B);
    int numsegments = 25;

    glBegin(GL_POLYGON);
      for(int ii = 0; ii < numsegments; ii++)
      {
        float theta = 2.0f * 3.1415926f * float(ii) / float(numsegments);//get the current angle

        float x = radius * cosf(theta);//calculate the x component
        float y = radius * sinf(theta);//calculate the y component

        glVertex2f(x, y);//output vertex
      }


    glEnd();
}

void Character::StartMoving(bool isToRight) {
    if (isJumping) {
        if (isToRight) {
            this->isFacingRight = true;
            this->leg1Theta1 = 40;
            this->leg1Theta2 = -100;
            this->leg2Theta1 = 45;
            this->leg2Theta2 = -100;
        } else {
            this->isFacingRight = false;
            this->leg1Theta1 = -40;
            this->leg1Theta2 = 100;
            this->leg2Theta1 = -45;
            this->leg2Theta2 = 100;
        }
    }
    else if (isToRight ) {
        this->isFacingRight = true;
        leg1Theta1 = 30;
        leg2Theta1 = -30;
        da = -legMovingSpeed;
        da2 = legMovingSpeed;
    } else {
        this->isFacingRight = false;
        leg1Theta1 = -30;
        leg2Theta1 = 30;
        da = legMovingSpeed;
        da2 = -legMovingSpeed;
    }
}

void Character::MoveInX(bool isToRight, GLdouble timeDiff) {
    if (isToRight) {
        this->gX += this->speed * timeDiff;
        std::cout << this->speed * timeDiff << std::endl;
        if (this->isJumping) { return; }
        leg1Theta2 = -10;
        leg2Theta2 = -10;

        if(this->leg1Theta1 > 30) { da = -legMovingSpeed; }
        if(this->leg1Theta1 < -30) { da = legMovingSpeed;}

        if(this->leg2Theta1 > 30) { da2 = -legMovingSpeed; }
        if(this->leg2Theta1 < -30) { da2 = legMovingSpeed; }

        leg1Theta1 += da;
        leg2Theta1 += da2;

    } else {
        this->gX -= this->speed * timeDiff;
        if (this->isJumping) { return; }
        leg1Theta2 = 10;
        leg2Theta2 = 10;

        if(this->leg1Theta1 > 30) { da = -legMovingSpeed; }
        if(this->leg1Theta1 < -30) { da = legMovingSpeed;}

        if(this->leg2Theta1 > 30) { da2 = -legMovingSpeed; }
        if(this->leg2Theta1 < -30) { da2 = legMovingSpeed; }

        leg1Theta1 += da;
        leg2Theta1 += da2;
    }
}

void Character::MoveInY(GLdouble timeDiff, bool isPressingJumpButton) {
    
    if (this->isJumping) {
        if (isPressingJumpButton) {
            if(this->gY >= 3 * totalHeight) {
                hasJumpedToMax = true;
            }
            if(this->gY < 3 * totalHeight && hasJumpedToMax == false) {
                this->gY += this->jumpingSpeed * timeDiff;
            } else if(this->gY > 0) {
                this->gY -= this->jumpingSpeed * timeDiff;
            }
        } else if (this->gY > 0) {
                this->gY -= this->jumpingSpeed * timeDiff;
                hasJumpedToMax = true;
        } 
        if(this->gY <= 0) {
            isJumping = false;
            hasJumpedToMax = false;
            std::cout << this->isJumping << std::endl;
            this->leg1Theta1 = 0;
            this->leg1Theta2 = 0;
            this->leg2Theta1 = 0;
            this->leg2Theta2 = 0;
            if(isPressingJumpButton) {
                this->StartJumping();
            }
        }
    }
}

void Character::StartJumping() {
    this->isJumping = true;
    if(isFacingRight) {
        this->leg1Theta1 = 40;
        this->leg1Theta2 = -100;
        this->leg2Theta1 = 45;
        this->leg2Theta2 = -100;
    } else {
        this->leg1Theta1 = -40;
        this->leg1Theta2 = 100;
        this->leg2Theta1 = -45;
        this->leg2Theta2 = 100;
    }
}

bool Character::getIsDirectionToRight() {
    return this->isFacingRight;
}

bool Character::getIsJumping() {
    return this->isJumping;
}

vec3 Character::getPosition() {
    return vec3(this->gX, this->gY, 0);
}

// bool Character::Atingido(Tiro *tiro)
// {
//     // GLfloat tX, tY;
//     // tiro->GetPos(tX, tY);
//     // if(tX >= gX - radiusAlvo && tX <= gX + radiusAlvo
//     //     && tY <= gY + radiusAlvo && tY >= gY - radiusAlvo) {
//     //         return true;
//     // }
//     // return false;
// }
