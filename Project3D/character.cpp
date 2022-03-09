#include "headers/character.h"
#include <math.h>
#include <iostream>

/* <-- Seção de Desenhos --> */
void Character::DrawCharacter(GLfloat x, GLfloat y, GLfloat z)
{
    // if(this->getIsJumping() == true) {
    //     cout << "Personagem pulando" << endl;
    // } else {
    //     cout << "Personagem NÃO pulando" << endl;
    // }
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(lookingAngle, 0, 1, 0);
    glTranslatef(0, -totalHeight * 0.1, 0);
    // Desenha o corpo do personagem
    glPushMatrix();
    glTranslatef(-bodyWidth/2, bodyHeight, 0);
    DrawRectangle(bodyHeight, bodyWidth, bodyWidth, bodyColor.x, bodyColor.y, bodyColor.z);
    glPopMatrix();
    // Desenha a cabeça do personagem
    glPushMatrix();
    glTranslatef(0, bodyHeight + radiusHead, 0);
    DrawCircle(radiusHead, bodyColor.x, bodyColor.y, bodyColor.z);
    glPopMatrix();
    // Desenha a perna do personagem
    DrawLegs();
    // Desenha os braços do personagem
    DrawArms();

    DrawAxes();
    // DrawHitbox();4r

    glPopMatrix();
}

void Character::DrawLegs() {
    // Desenhando a perna esquerda
    glPushMatrix();
    glTranslatef(-legWidth/2, 0, -bodyWidth/2 + legWidth/2);
    glRotatef(this->leg1Theta1, 0, 0, 1);
    DrawRectangle(legHeight, legWidth, legWidth, bodyColor.x + 0.2, bodyColor.y - 0.2, bodyColor.z + 0.2);

    glRotatef(this->leg1Theta2, 0, 0, 1);
    glTranslatef(0, -legHeight, 0);
    DrawRectangle(legHeight, legWidth, legWidth, bodyColor.x + 0.2, bodyColor.y - 0.2, bodyColor.z + 0.2);
    glPopMatrix();
    
    // Desenhando a perna direita
    glPushMatrix();
    glTranslatef(-legWidth/2, 0, bodyWidth/2 - legWidth/2);
    glRotatef(this->leg2Theta1, 0, 0, 1);
    DrawRectangle(legHeight, legWidth, legWidth, bodyColor.x + 0.2, bodyColor.y - 0.2, bodyColor.z + 0.2);

    glRotatef(this->leg2Theta2, 0, 0, 1);
    glTranslatef(0, -legHeight, 0);
    DrawRectangle(legHeight, legWidth, legWidth, bodyColor.x + 0.2, bodyColor.y - 0.2, bodyColor.z + 0.2);
    glPopMatrix();
}

void Character::DrawArms() {
    glPushMatrix();
    if(!this->isFacingRight) {
        glTranslatef(0, 0.3 * totalHeight, 0);
        glRotatef(-this->armTheta, 0, 0, 1);

        DrawRectangle(armHeight, armWidth, armWidth, bodyColor.x + 0.2, bodyColor.y - 0.2, bodyColor.z + 0.2);
    } else {
        glTranslatef(0, 0.3 * totalHeight, 0);
        glRotatef(this->armTheta, 0, 0, 1);

        DrawRectangle(armHeight, armWidth, armWidth, bodyColor.x + 0.2, bodyColor.y - 0.2, bodyColor.z + 0.2);
    }
    // cout << this->armTheta << endl;
    glPopMatrix();
}

void Character::DrawRectangle(GLfloat height, GLfloat width, GLfloat depth, GLfloat R, GLfloat G, GLfloat B) {
    /* Define cor dos vértices com os valores R, G e B variando de 0.0 a 1.0 */
    glColor3f (R, G, B);

  glBegin(GL_QUADS);
        glNormal3f(0,1,0);
        glVertex3f (0, 0, -depth/2);
        glNormal3f(0,1,0);
        glVertex3f (width, 0,-depth/2);
        glNormal3f(0,1,0);
        glVertex3f (width , 0, depth/2);
        glNormal3f(0,1,0);
        glVertex3f (0, 0, depth/2);
        glNormal3f(0,1,0);
        glVertex3f (0, 0, -depth/2);
    glEnd();

    // Face inferior
    glBegin(GL_QUADS);
        glNormal3f(0,-1,0);
        glVertex3f (0, -height, -depth/2);
        glNormal3f(0,-1,0);
        glVertex3f (width, -height,-depth/2);
        glNormal3f(0,-1,0);
        glVertex3f (width , -height, depth/2);
        glNormal3f(0,-1,0);
        glVertex3f (0, -height, depth/2);
        glNormal3f(0,-1,0);
        glVertex3f (0, -height, -depth/2);
    glEnd();

    // Faces laterais
    glBegin(GL_QUADS); // 1
        glNormal3f(-1,0,0);
        glVertex3f (0, 0, -depth/2);
        glNormal3f(-1,0,0);
        glVertex3f (0, -height, -depth/2);
        glNormal3f(-1,0,0);
        glVertex3f (0, -height, depth/2);
        glNormal3f(-1,0,0);
        glVertex3f (0, 0, depth/2);
        glNormal3f(-1,0,0);
        glVertex3f (0, 0, -depth/2);
    glEnd();
    glBegin(GL_QUADS); // 2
        glNormal3f(0,0,1);
        glVertex3f (0, 0, depth/2);
        glNormal3f(0,0,1);
        glVertex3f (0, -height, depth/2);
        glNormal3f(0,0,1);
        glVertex3f (width, -height, depth/2);
        glNormal3f(0,0,1);
        glVertex3f (width, 0, depth/2);
        glNormal3f(0,0,1);
        glVertex3f (0, 0, depth/2);
    glEnd();
    glBegin(GL_QUADS); // 3
        glNormal3f(1,0,0);
        glVertex3f (width, 0, -depth/2);
        glNormal3f(1,0,0);
        glVertex3f (width, -height, -depth/2);
        glNormal3f(1,0,0);
        glVertex3f (width, -height, depth/2);
        glNormal3f(1,0,0);
        glVertex3f (width, 0, depth/2);
        glNormal3f(1,0,0);
        glVertex3f (width, 0, -depth/2);
    glEnd();
    glBegin(GL_QUADS); // 4
        glNormal3f(0,0, -1);
        glVertex3f (0, 0, -depth/2);
        glNormal3f(0,0, -1);
        glVertex3f (width, 0, -depth/2);
        glNormal3f(0,0, -1);
        glVertex3f (width, -height, -depth/2);
        glNormal3f(0,0, -1);
        glVertex3f (0, -height, -depth/2);
        glNormal3f(0,0, -1);
        glVertex3f (0, 0, -depth/2);
    glEnd();
}

void Character::DrawCircle(GLfloat radius, GLfloat R, GLfloat G, GLfloat B)
{
    glColor3f(R, G, B);
    glutSolidSphere(radius, 20, 10);
    glEnd();
}

void Character::DrawAxes()
{
    GLfloat color_r[] = { 1.0, 0.0, 0.0, 1.0 };
    GLfloat color_g[] = { 0.0, 1.0, 0.0, 1.0 };
    GLfloat color_b[] = { 0.0, 0.0, 1.0, 1.0 };

    glPushAttrib(GL_ENABLE_BIT);
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
 
        //x axis
        glPushMatrix();
            glColor3fv(color_r);
            glScalef(5, 0.3, 0.3);
            glTranslatef(0.5, 0, 0); // put in one end
            glutSolidCube(1.0);
        glPopMatrix();

        //y axis
        glPushMatrix();
            glColor3fv(color_g);
            glRotatef(90,0,0,1);
            glScalef(5, 0.3, 0.3);
            glTranslatef(0.5, 0, 0); // put in one end
            glutSolidCube(1.0);
        glPopMatrix();

        //z axis
        glPushMatrix();
            glColor3fv(color_b);
            glRotatef(-90,0,1,0);
            glScalef(5, 0.3, 0.3);
            glTranslatef(0.5, 0, 0); // put in one end
            glutSolidCube(1.0);
        glPopMatrix();
    glPopAttrib();
    
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

/* <-- Seção de Animações --> */
void Character::StartMoving(bool isToRight) {
    if (isJumping) {
        // if (isToRight) {
        //     this->isFacingRight = true;
        //     this->leg1Theta1 = 40;
        //     this->leg1Theta2 = -100;
        //     this->leg2Theta1 = 45;
        //     this->leg2Theta2 = -100;
        // } else {
        //     this->isFacingRight = false;
        //     this->leg1Theta1 = -40;
        //     this->leg1Theta2 = 100;
        //     this->leg2Theta1 = -45;
        //     this->leg2Theta2 = 100;
        // }
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

/* <-- Seção de Movimentos e Lógica --> */
void Character::MoveInXZ(bool isToRight, GLdouble timeDiff, Map* map) {

    if (isToRight) {
        this->isFacingRight = true;
        GLfloat incx = this->speed * timeDiff * this->directionVector.x;
        GLfloat incz = -this->speed * timeDiff * this->directionVector.z;
        if (CollidesRightWithAPlatform(incx, map) == true) {
            return;
        }
        this->gX += incx;
        this->gZ += incz;
        if (this->isJumping) { return; }
        if(this->leg1Theta1 > 30) { da = -legMovingSpeed; }
        if(this->leg1Theta1 < -30) { da = legMovingSpeed; }

        if(this->leg2Theta1 > 30) { da2 = -legMovingSpeed; }
        if(this->leg2Theta1 < -30) { da2 = legMovingSpeed; }

        leg1Theta1 += da;
        leg2Theta1 += da2;

    } else {
        this->isFacingRight = false;
        GLfloat incx = this->speed * timeDiff * -this->directionVector.x;
        GLfloat incz = -this->speed * timeDiff * -this->directionVector.z;
        if (CollidesLeftWithAPlatform(incx, map) == true) {
            return;
        }
        this->gX += incx;
        this->gZ += incz;
        if (this->isJumping) { return; }

        if(this->leg1Theta1 > 30) { da = -legMovingSpeed; }
        if(this->leg1Theta1 < -30) { da = legMovingSpeed; }

        if(this->leg2Theta1 > 30) { da2 = -legMovingSpeed; }
        if(this->leg2Theta1 < -30) { da2 = legMovingSpeed; }

        leg1Theta1 += da;
        leg2Theta1 += da2;
    }
}

void Character::MoveInY(GLdouble timeDiff, bool isPressingJumpButton, Map* map) {
    GLfloat inc = this->jumpingSpeed * timeDiff;
    if (CollidesDownWithAPlatform(inc, map) && !this->isJumping) {
        return;
    }

    if (CollidesUpWithAPlatform(inc, map)) {
        hasJumpedToMax = true;
    }

    if (this->isJumping) {
        if (isPressingJumpButton) {
            GLfloat cground = this->GetCharacterGroundY();
            if(cground >= this->jumpingGround + 3 * totalHeight) {
                hasJumpedToMax = true;
            }
            if(cground <= this->jumpingGround + 3 * totalHeight && hasJumpedToMax == false) {
                this->gY += inc;
            } else if(this->gY >= this->groundLimit) {
                this->gY -= inc;
            }
        } else if (!CollidesDownWithAPlatform(inc, map)) {
            this->gY -= inc;
            hasJumpedToMax = true;
        } 
        if(CollidesDownWithAPlatform(inc, map)) {
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
    } else  {
        // isJumping = true;
        this->gY -= inc;
    }
}

void Character::MoveArmsAngle(GLfloat x, GLfloat y) {
    GLfloat armX = this->gX;
    GLfloat armY = this->gY + 0.3 * totalHeight;
    // cout << this->gX << " : " << x << endl;
    if (isFacingRight) {
        GLfloat theta = atan2(y - armY, x - armX);
        GLfloat degrees = theta * (180/M_PI); // Convertendo de radianos para graus
        // cout << degrees << endl;
        if (degrees < 45 && degrees > -45 ) {
            this->armTheta = 90 - theta * (180/M_PI);
        }
    } else {
        GLfloat theta = atan2(y - armY, armX - x);
        GLfloat degrees = theta * (180/M_PI); // Convertendo de radianos para graus
        // cout << degrees << endl;
        if (degrees < 45 && degrees > -45 ) {
            this->armTheta = 90 - theta * (180/M_PI) ;
        }
    }
}

void Character::TurnRight(GLdouble timeDiff) {
    if (this->isJumping == true) {
        // cout << "Pulando!" << endl;
        return;
    }
    if (this->lookingAngle <= -360) {
        this->lookingAngle = 0;
    }
    this->lookingAngle -= timeDiff * turningSpeed;
    this->directionVector.x = cos((this->lookingAngle) * M_PI/180 );
    this->directionVector.z = sin(this->lookingAngle * M_PI/180);
    // cout << "Direction Vector: " << this->directionVector.x << " , " << this->directionVector.z << endl;
    // cout << "Looking Angle: " << this->lookingAngle << endl;
}

void Character::TurnLeft(GLdouble timeDiff) {
    if (this->isJumping == true) {
        // cout << "Pulando!" << endl;
        return;
    }
    if (this->lookingAngle >= 360) {
        this->lookingAngle = 0;
    }
    this->lookingAngle += timeDiff * turningSpeed;
    this->directionVector.x = cos((this->lookingAngle)  * M_PI/180);
    this->directionVector.z = sin(this->lookingAngle * M_PI/180);
    // cout << "Direction Vector: " << this->directionVector.x << " , " << this->directionVector.z << endl;
    // cout << "Looking Angle: " << this->lookingAngle << endl;
}

// Funcao auxiliar de rotacao para posicionar o tiro
void RotatePointUtil(GLfloat x, GLfloat y, GLfloat height, GLfloat angle, GLfloat &xOut, GLfloat &yOut){
    yOut = y + height*sin (angle*M_PI/180);
    xOut = x + height*cos (angle*M_PI/180);
}

Shot* Character::Shoot() {
    if (this->ammo > 0) {
        // cout << ammo << endl;
        GLfloat tX = this->gX, tY = this->gY, tTheta = 90 + this->armTheta;
        tY += 0.2 * totalHeight;

        if (isFacingRight) {
            tTheta = 180 - tTheta;
            RotatePointUtil(tX, tY, this->armHeight , tTheta, tX, tY);
        } else {
            RotatePointUtil(tX, tY, this->armHeight , tTheta, tX, tY);
        }
        this->ammo -= 1;
        Shot* shot = new Shot(tX, tY, tTheta, armWidth * 0.6, this->speed * 2, this->shootColor);
        return shot;
    }
    return nullptr;
}

void Character::RechargeShot() {
    if(this->ammo <= totalAmmo) {
        this->ammo += 1;
    }
}

bool Character::CollidesDownWithAPlatform(GLfloat inc, Map* map) {
    if (GetCharacterGroundY() + 0.1 * this->GetTotalHeight() <= this->groundLimit) {
        return true;
    }
    if (inc == 0) { return false;}
    if (map->ColidesWithAPlatform(this->gX - this->bodyWidth/2, this->gY - 0.6*totalHeight - inc, this->gZ + inc) == true
        ||
        map->CollidesWithEnemy(this->gX - this->bodyWidth/2, this->gY - 0.6*totalHeight - inc) == true
        ) {
        return true;
    }
    if (map->ColidesWithAPlatform(this->gX + this->bodyWidth/2, this->gY - 0.6*totalHeight - inc, this->gZ + inc) == true
        ||
        map->CollidesWithEnemy(this->gX + this->bodyWidth/2, this->gY - 0.6*totalHeight - inc) == true
    ) {
        return true;
    }
    return false;
}

bool Character::CollidesUpWithAPlatform(GLfloat inc, Map* map) {
    if (map->ColidesWithAPlatform(this->gX - this->bodyWidth/2, this->gY + 0.5*totalHeight + inc, this->gZ + inc) == true
        ||
        map->CollidesWithEnemy(this->gX - this->bodyWidth/2, this->gY + 0.5*totalHeight + inc) == true   
    ) {
        return true;
    }
    if (map->ColidesWithAPlatform(this->gX + this->bodyWidth/2, this->gY + 0.5*totalHeight + inc, this->gZ + inc) == true
        ||
        map->CollidesWithEnemy(this->gX + this->bodyWidth/2, this->gY + 0.5*totalHeight + inc) == true  
    ) {
        return true;
    }

    return false;
}

bool Character::CollidesLeftWithAPlatform(GLfloat inc, Map* map) {
    for(double i = -0.49; i<= 0.41; i+=0.1) {
        if (map->ColidesWithAPlatform(this->gX - this->bodyWidth/2 - inc, this->gY + i*totalHeight, this->gZ + inc) == true
            ||
            map->CollidesWithEnemy(this->gX - this->bodyWidth/2 - inc, this->gY + i*totalHeight) == true
        ) {
            return true;
        }
    }
    return false;
}

bool Character::CollidesRightWithAPlatform(GLfloat inc, Map* map) {
    for(double i = -0.49; i<= 0.41; i+=0.1) {
        if (map->ColidesWithAPlatform(this->gX + this->bodyWidth/2 + inc, this->gY + i*totalHeight, this->gZ + inc) == true
            ||
            map->CollidesWithEnemy(this->gX + this->bodyWidth/2 + inc, this->gY + i*totalHeight) == true
        ) {
            return true;
        }
    }
    return false;
}

bool Character::CollidesWithPoint(GLfloat x, GLfloat y) {
    if(x > this->gX - bodyWidth/2 && x < this->gX + bodyWidth/2) {
        if(y > this->gY - 0.5 * totalHeight && y < this->gY + 0.5 * totalHeight) {
            return true;
        }
    }
    return false;
}

bool Character::CollidesWithEndOfMap(Map* map) {
    if(map->ColidesWithRightEnd(this->gX + bodyWidth/2 + armWidth) == true) {
        return true;
    }
    return false;
}

/* <-- Getters e Setters --> */
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
    return this->gY - 0.51* this->totalHeight;
}

GLfloat Character::GetCharacterHighestY() {
    return this->gY + 0.51* this->totalHeight;
}

GLfloat Character::GetBodyWidth() {
    return this->bodyWidth;
}

GLfloat Character::GetTotalHeight() {
    return this->totalHeight;
}
