#include "headers/character.h"
#include <math.h>
#include <iostream>

/* <-- Seção de Desenhos --> */
void Character::DrawCharacter(GLdouble x, GLdouble y, GLdouble z)
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

    // DrawAxes();
    // DrawHitbox();

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

void Character::DrawRectangle(GLdouble height, GLdouble width, GLdouble depth, GLfloat R, GLfloat G, GLfloat B) {
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

void Character::DrawCircle(GLdouble radius, GLfloat R, GLfloat G, GLfloat B)
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
    GLdouble radius = this->bodyWidth;
    GLdouble height = this->totalHeight;
    GLdouble angle = 0.0;
    GLdouble angle_stepsize = 0.1;
    GLdouble x              = 0.0;
    GLdouble y              = 0.0;
    glPushMatrix();
    glTranslatef(0, 0.6 * this->totalHeight, 0);
    glRotatef(90, 1, 0, 0);
       /** Draw the tube */
    glColor3ub(1,1,1);
    glBegin(GL_QUAD_STRIP);
    angle = 0.0;
        while( angle < 2*M_PI ) {
            x = radius * cos(angle);
            y = radius * sin(angle);
            glVertex3f(x, y , height);
            glVertex3f(x, y , 0.0);
            angle = angle + angle_stepsize;
        }
        glVertex3f(radius, 0.0, height);
        glVertex3f(radius, 0.0, 0.0);
    glEnd();

    /** Draw the circle on top of cylinder */
    glColor3ub(1,1,1);
    glBegin(GL_POLYGON);
    angle = 0.0;
        while( angle < 2*M_PI ) {
            x = radius * cos(angle);
            y = radius * sin(angle);
            glVertex3f(x, y , height);
            angle = angle + angle_stepsize;
        }
        glVertex3f(radius, 0.0, height);
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
void Character::MoveInXZ(bool isForward, GLdouble timeDiff, Map* map) {

    if (isForward) {
        // this->isFacingRight = true;
        GLdouble incx = this->speed * timeDiff * this->directionVector.x;
        GLdouble incz = -this->speed * timeDiff * this->directionVector.z;
        this->gX += incx;
        this->gZ += incz;
        if (CollidesRightWithAPlatform(map) == true) {
            this->gX -= incx;
            this->gZ -= incz;
            return;
        }
        if (this->isJumping) { return; }
        this->AnimateLegs();

    } else {
        // this->isFacingRight = false;
        GLdouble incx = this->speed * timeDiff * -this->directionVector.x;
        GLdouble incz = -this->speed * timeDiff * -this->directionVector.z;
        this->gX += incx;
        this->gZ += incz;

        if (CollidesLeftWithAPlatform(map) == true) {
            this->gX -= incx;
            this->gZ -= incz;
            return;
        }
        if (this->isJumping) { return; }
        this->AnimateLegs();
    }
}

void Character::AnimateLegs() {
    if(this->leg1Theta1 > 30) { this->da = -legMovingSpeed; }
    if(this->leg1Theta1 < -30) { this->da = legMovingSpeed; }
    if(this->leg2Theta1 > 30) { this->da2 = -legMovingSpeed; }
    if(this->leg2Theta1 < -30) { this->da2 = legMovingSpeed; }
    this->leg1Theta1 += this->da;
    this->leg2Theta1 += this->da2;
}

void Character::MoveInY(GLdouble timeDiff, bool isPressingJumpButton, Map* map) {
    GLdouble inc = this->jumpingSpeed * timeDiff;
    if (this->isJumping) {
        if (isPressingJumpButton) {
            GLdouble cground = this->GetCharacterGroundY();
            if(cground >= this->jumpingGround + 3 * totalHeight) {
                hasJumpedToMax = true;
            }
            if(cground <= this->jumpingGround + 3 * totalHeight && hasJumpedToMax == false) {
                this->gY += inc;
            } else if(this->gY >= this->groundLimit) {
                this->gY -= inc;
            }
            if (CollidesUpWithAPlatform(map)) {
                this->gY -= inc;
                hasJumpedToMax = true;
            }
        } else {
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
    } else  {
        // isJumping = true;
        this->gY -= inc;
        if (CollidesDownWithAPlatform(map) && !this->isJumping) {
            this->gY += inc;
            return;
        }
    }
}

void Character::MoveArmsAngle(GLdouble x, GLdouble y) {
    GLdouble armX = this->gX;
    GLdouble armY = this->gY + 0.3 * totalHeight;
    // cout << this->gX << " : " << x << endl;
    if (isFacingRight) {
        GLdouble theta = atan2(y - armY, x - armX);
        GLdouble degrees = theta * (180/M_PI); // Convertendo de radianos para graus
        // cout << degrees << endl;
        if (degrees < 45 && degrees > -45 ) {
            this->armTheta = 90 - theta * (180/M_PI);
        }
    } else {
        GLdouble theta = atan2(y - armY, armX - x);
        GLdouble degrees = theta * (180/M_PI); // Convertendo de radianos para graus
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
void RotatePointUtil(GLdouble x, GLdouble y, GLdouble height, GLdouble angle, GLdouble &xOut, GLdouble &yOut){
    yOut = y + height*sin (angle*M_PI/180);
    xOut = x + height*cos (angle*M_PI/180);
}

Shot* Character::Shoot() {
    if (this->ammo > 0) {
        // cout << ammo << endl;
        GLdouble tX = this->gX, tY = this->gY, tTheta = 90 + this->armTheta;
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

bool Character::CollidesDownWithAPlatform(Map* map) {
    if (GetCharacterGroundY() - 0.1 * this->GetTotalHeight() <= this->groundLimit) {
        return true;
    }
    int nPartitions = 72;
    for(double j = 0; j < nPartitions; j++) {
        float theta = 2.0f * M_PI * float(j) / float(nPartitions);//get the current angle
            GLdouble actualX = this->gX + (this->bodyWidth/2) * cosf(theta);
            GLdouble actualZ = this->gZ +(this->bodyWidth/2) * sinf(theta);
        if (map->ColidesWithAPlatform(actualX, this->gY - 0.6*totalHeight, actualZ) == true
            ||
            map->CollidesWithEnemy(actualX, this->gY - 0.6*totalHeight) == true
            ) {
            return true;
        }
    }
    return false;
}

bool Character::CollidesUpWithAPlatform(Map* map) {
    int nPartitions = 72;
    for(double j = 0; j < nPartitions; j++) {
        float theta = 2.0f * M_PI * float(j) / float(nPartitions);//get the current angle
            GLdouble actualX = this->gX + (this->bodyWidth/2) * cosf(theta);
            GLdouble actualZ = this->gZ +(this->bodyWidth/2) * sinf(theta);
        if (map->ColidesWithAPlatform(actualX, this->gY + 0.6*totalHeight, actualZ) == true
            ||
            map->CollidesWithEnemy(actualX, this->gY + 0.6*totalHeight) == true   
        ) {
            return true;
        }
    }

    return false;
}

bool Character::CollidesLeftWithAPlatform(Map* map) {
    int nPartitions = 72;
    for(double i = -0.48; i<= 0.41; i+=0.1) {
        for(double j = 0; j < nPartitions; j++) {
            float theta = 2.0f * M_PI * float(j) / float(nPartitions);//get the current angle
            GLdouble actualX = this->gX + (this->bodyWidth/2) * cosf(theta);
            GLdouble actualZ = this->gZ + (this->bodyWidth/2) * sinf(theta);
            if (map->ColidesWithAPlatform(actualX, this->gY + i*totalHeight, actualZ) == true
                // ||
                // map->CollidesWithEnemy(this->gX + this->bodyWidth/2 + inc, this->gY + i*totalHeight) == true
            ) {
                return true;
            }
        }
    }
    return false;
}

bool Character::CollidesRightWithAPlatform(Map* map) {
    int nPartitions = 72;
    for(double i = -0.48; i<= 0.41; i+=0.1) {
        for(double j = 0; j < nPartitions; j++) {
            float theta = (2.0f * M_PI) * float(j) / float(nPartitions);//get the current angle
            GLdouble actualX = this->gX + (this->bodyWidth/2) * cosf(theta);
            GLdouble actualZ = this->gZ + (this->bodyWidth/2) * sinf(theta);
            if (map->ColidesWithAPlatform(actualX, this->gY + i*totalHeight, actualZ) == true
                // ||
                // map->CollidesWithEnemy(this->gX + this->bodyWidth/2 + inc, this->gY + i*totalHeight) == true
            ) {
                return true;
            }
        }
    }
    return false;
}

bool Character::CollidesWithPoint(GLdouble x, GLdouble y) {
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

GLdouble Character::GetgX() {
    return this->gX;
}

GLdouble Character::GetgY() {
    return this->gY;
}

GLdouble Character::GetCharacterGroundY() {
    return this->gY - 0.51* this->totalHeight;
}

GLdouble Character::GetCharacterHighestY() {
    return this->gY + 0.51* this->totalHeight;
}

GLdouble Character::GetBodyWidth() {
    return this->bodyWidth;
}

GLdouble Character::GetTotalHeight() {
    return this->totalHeight;
}
