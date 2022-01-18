#include "headers/character.h"
#include <math.h>
#include <iostream>

/* <-- Drawing Section --> */
void Character::DrawCharacter(GLfloat x, GLfloat y)
{
    glPushMatrix();

    glTranslatef(x, y, 0);
    glTranslatef(0, -totalHeight * 0.1, 0);

    // Desenha o corpo do personagem
    DrawRectangle(bodyHeight, bodyWidth, bodyColor.x, bodyColor.y, bodyColor.z);

    // Desenha a cabeça do personagem
    glPushMatrix();
    glTranslatef(0, bodyHeight + radiusHead, 0);
    DrawCircle(radiusHead, bodyColor.x, bodyColor.y, bodyColor.z);
    glPopMatrix();

    // Desenha a perna do personagem
    DrawLegs();

    // Desenha os braços do personagem
    DrawArms();

    // DrawHitbox();

    glPopMatrix();
}

void Character::DrawLegs() {
    // Desenhando a perna esquerda
    glPushMatrix();
    glTranslatef(0, 0, 0);
    glRotatef(this->leg1Theta1, 0, 0, 1);
    glTranslatef(0, -legHeight, 0);
    DrawRectangle(legHeight, legWidth, bodyColor.x + 0.2, bodyColor.y - 0.2, bodyColor.z + 0.2);

    glRotatef(this->leg1Theta2, 0, 0, 1);
    glTranslatef(0, -legHeight, 0);
    DrawRectangle(legHeight, legWidth, bodyColor.x + 0.2, bodyColor.y - 0.2, bodyColor.z + 0.2);
    glPopMatrix();
    
    // Desenhando a perna direita
    glPushMatrix();
    glTranslatef(0, 0, 0);
    glRotatef(this->leg2Theta1, 0, 0, 1);
    glTranslatef(0, -legHeight, 0);
    DrawRectangle(legHeight, legWidth, bodyColor.x + 0.2, bodyColor.y - 0.2, bodyColor.z + 0.2);

    glRotatef(this->leg2Theta2, 0, 0, 1);
    glTranslatef(0, -legHeight, 0);
    DrawRectangle(legHeight, legWidth, bodyColor.x + 0.2, bodyColor.y - 0.2, bodyColor.z + 0.2);
    glPopMatrix();
}

void Character::DrawArms() {
    glPushMatrix();
    if(this->isFacingRight) {
        glTranslatef(0, 0.3 * totalHeight, 0);
        glRotatef(-this->armTheta, 0, 0, 1);

        DrawRectangle(armHeight, armWidth, bodyColor.x + 0.2, bodyColor.y - 0.2, bodyColor.z + 0.2);
    } else {
        glTranslatef(0, 0.3 * totalHeight, 0);
        glRotatef(this->armTheta, 0, 0, 1);

        DrawRectangle(armHeight, armWidth, bodyColor.x + 0.2, bodyColor.y - 0.2, bodyColor.z + 0.2);
    }
    // cout << this->armTheta << endl;
    glPopMatrix();
}

void Character::DrawRectangle(GLfloat height, GLfloat width, GLfloat R, GLfloat G, GLfloat B) {
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

void Character::DrawCircle(GLfloat radius, GLfloat R, GLfloat G, GLfloat B)
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

void Character::DrawHitbox() {
    hitBox hb = hitBox(
        vec2(- (bodyWidth/2),  0.5*totalHeight),
        vec2(+ (bodyWidth/2),  0.5*totalHeight),
        vec2(- (bodyWidth/2), -0.5*totalHeight),
        vec2(+ (bodyWidth/2), -0.5*totalHeight)
    );
    glColor3f (1, 1, 1);

    glPushMatrix();
    glTranslatef(0, totalHeight * 0.1, 0);
    glBegin(GL_LINE_LOOP);
        glVertex3f (hb.downLeft.x, hb.downLeft.y, 0.0);
        glVertex3f (hb.upLeft.x, hb.upLeft.y, 0.0);
        glVertex3f (hb.upRight.x, hb.upRight.y, 0.0);
        glVertex3f (hb.downRight.x, hb.downRight.y, 0.0);
    glEnd();

    glPopMatrix();
}

/* <-- Animation Section --> */
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
    else if (isToRight) {
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

void Character::StartJumping() {
    this->isJumping = true;
    this->jumpingGround = GetCharacterGroundY();
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

void Character::StartStanding() {
    this->leg1Theta1 = 0;
    this->leg1Theta2 = 0;
    this->leg2Theta1 = 0;
    this->leg2Theta2 = 0;
}

/* <-- Moving and Logic Section --> */
void Character::MoveInX(bool isToRight, GLdouble timeDiff, Map* map) {
    if (isToRight) {
        GLfloat inc = this->speed * timeDiff;
        if (CollidesRightWithAPlatform(inc, map) == true) {
            return;
        }
        this->gX += inc;
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
        GLfloat inc = this->speed * timeDiff;
        if (CollidesLeftWithAPlatform(inc, map) == true) {
            return;
        }
        this->gX -= inc;
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

void Character::MoveInY(GLdouble timeDiff, bool isPressingJumpButton, Map* map) {
    if (CollidesDownWithAPlatform(map) && !this->isJumping) {
        return;
    }
    GLfloat inc = this->jumpingSpeed * timeDiff;

    if (CollidesUpWithAPlatform(map)) {
        hasJumpedToMax = true;
    }

    if (this->isJumping) {
        if (isPressingJumpButton) {
            if(this->gY >= this->jumpingGround + 3 * totalHeight) {
                hasJumpedToMax = true;
            }
            if(this->gY <= this->jumpingGround + 3 * totalHeight && hasJumpedToMax == false) {
                this->gY += inc;
            } else if(this->gY >= this->groundLimit) {
                this->gY -= inc;
            }
        } else if (!CollidesDownWithAPlatform(map)) {
            this->gY -= inc;
            hasJumpedToMax = true;
        } 
        if(CollidesDownWithAPlatform(map)) {
            isJumping = false; 
            hasJumpedToMax = false;
            this->leg1Theta1 = 0;
            this->leg1Theta2 = 0;
            this->leg2Theta1 = 0;
            this->leg2Theta2 = 0;
            if(isPressingJumpButton) {
                this->StartJumping();
            }
        }
    } else {
        isJumping = true;
        this->gY -= inc;
    }
}

void Character::MoveArmsAngle(GLfloat x, GLfloat y) {
    GLfloat armX = this->gX;
    GLfloat armY = this->gY + 0.3 * totalHeight;
    GLfloat theta = atan2(y - armY, x - armX);

    GLfloat degrees = theta * 2 * (180/M_PI); // Convertendo de radianos para graus
    if (degrees < 45 && degrees > -45 ) {
        this->armTheta = 90 - theta * (180/M_PI);
    }

}

Shot* Character::Shoot() {
    if (!this->isShooting) {
        this->isShooting = true;
        GLfloat tX = this->gX, tY = this->gY, tTheta = 90 + this->armTheta;
        tX += bodyWidth / 2;
        tY += 0.3 * totalHeight;

        if (isFacingRight) {
            tTheta = 180 - tTheta;
        }
        // RotatePoint(tX, tY, this->armHeight , -this->armTheta, tX, tY);

        std::cout << "Ponto criado: (" <<  tX << "," << tY << ")" << std::endl;
        return new Shot(tX, tY, tTheta, armWidth, this->speed * 2);
    }
}

void Character::RechargeShot() {
    this->isShooting = false;
}

bool Character::CollidesDownWithAPlatform(Map* map) {
    if (GetCharacterGroundY() <= this->groundLimit) {
        this->gY = groundLimit + 0.6 * totalHeight;
        return true;
    }
    if (map->ColidesWithAPlatform(this->gX - this->bodyWidth/2, this->gY - 0.60*totalHeight) == true) {
        return true;
    }
    if (map->ColidesWithAPlatform(this->gX + this->bodyWidth/2, this->gY - 0.60*totalHeight) == true) {
        return true;
    }
    return false;
}

bool Character::CollidesUpWithAPlatform(Map* map) {
    if (map->ColidesWithAPlatform(this->gX - this->bodyWidth/2, this->gY + 0.5*totalHeight) == true) {
        return true;
    }
    if (map->ColidesWithAPlatform(this->gX + this->bodyWidth/2, this->gY + 0.5*totalHeight) == true) {
        return true;
    }

    return false;
}

bool Character::CollidesLeftWithAPlatform(GLfloat inc, Map* map) {
    for(double i = -0.49; i<= 0.49; i+=0.1) {
        if (map->ColidesWithAPlatform(this->gX - this->bodyWidth/2 - inc, this->gY + i*totalHeight) == true) {
            return true;
        }
    }
    return false;
}

bool Character::CollidesRightWithAPlatform(GLfloat inc, Map* map) {
    for(double i = -0.49; i<= 0.49; i+=0.1) {
        if (map->ColidesWithAPlatform(this->gX + this->bodyWidth/2 + inc, this->gY + i*totalHeight) == true) {
            return true;
        }
    }
    return false;
}

/* <-- Getters and Setters Section --> */
bool Character::getIsDirectionToRight() {
    return this->isFacingRight;
}

bool Character::getIsJumping() {
    return this->isJumping;
}

vec3 Character::getPosition() {
    return vec3(this->gX, this->gY, 0);
}

GLfloat Character::GetgX() {
    return this->gX;
}

GLfloat Character::GetgY() {
    return this->gY;
}

GLfloat Character::GetCharacterGroundY() {
    return this->gY - 0.5* this->totalHeight;
}

GLfloat Character::GetCharacterHighestY() {
    return this->gY + 0.5* this->totalHeight;
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
