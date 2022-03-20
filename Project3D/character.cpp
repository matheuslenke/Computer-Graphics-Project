#include "headers/character.h"
#include <math.h>
#include <iostream>
#include "headers/geom_draw.h"


double angleToRad(double angle){
  return (angle*M_PI)/180.0;
}

double _cos(double angle){
    return cos(angleToRad(angle));
}

double _sin(double angle){
    return sin(angleToRad(angle));
}

void pRotatef(double angle, double &xOut, double &yOut, double x0 = 0., double y0 = 0.){
  double tetha = angleToRad(angle);

  double _sin = sin(tetha);
  double _cos = cos(tetha);

  // Translate point to origin
  xOut -= x0;
  yOut -= y0;

  // rotate
  double xNew = xOut*_cos - yOut*_sin;
  double yNew = xOut*_sin + yOut*_cos;

  // translate back 
  xOut = xNew + x0;
  yOut = yNew + y0;
}


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
    drawRectangle(bodyHeight, bodyWidth, bodyWidth, bodyColor.x, bodyColor.y, bodyColor.z);
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

    // DEBUG: AIM POSITION
    // vec3 finalAim = GetFinalAimPosition();
    // vec3 initAim = GetInitialAimPosition();  
    // glPushMatrix();
    // glTranslatef(finalAim.x, finalAim.y, finalAim.z);
    // DrawCircle(armWidth/3, 1, 0, 0);
    // glPopMatrix();
    // glPushMatrix();
    // glTranslatef(initAim.x, initAim.y, initAim.z);
    // DrawCircle(armWidth/2, 1, 0, 0);
    // glPopMatrix();

    glPopMatrix();
}

void Character::DrawLegs() {
    // Desenhando a perna esquerda
    glPushMatrix();
    glTranslatef(-legWidth/2, 0, -bodyWidth/2 + legWidth/2);
    glRotatef(this->leg1Theta1, 0, 0, 1);
    drawRectangle(legHeight, legWidth, legWidth, bodyColor.x + 0.2, bodyColor.y - 0.2, bodyColor.z + 0.2);

    glRotatef(this->leg1Theta2, 0, 0, 1);
    glTranslatef(0, -legHeight, 0);
    drawRectangle(legHeight, legWidth, legWidth, bodyColor.x + 0.2, bodyColor.y - 0.2, bodyColor.z + 0.2);
    glPopMatrix();
    
    // Desenhando a perna direita
    glPushMatrix();
    glTranslatef(-legWidth/2, 0, bodyWidth/2 - legWidth/2);
    glRotatef(this->leg2Theta1, 0, 0, 1);
    drawRectangle(legHeight, legWidth, legWidth, bodyColor.x + 0.2, bodyColor.y - 0.2, bodyColor.z + 0.2);

    glRotatef(this->leg2Theta2, 0, 0, 1);
    glTranslatef(0, -legHeight, 0);
    drawRectangle(legHeight, legWidth, legWidth, bodyColor.x + 0.2, bodyColor.y - 0.2, bodyColor.z + 0.2);
    glPopMatrix();
}

void Character::DrawArms() {
    if(!this->isFacingForward) {
        glPushMatrix();
        glTranslatef(0, 0.3 * totalHeight + armWidth/2, -bodyWidth/2 - armWidth/2);
        glRotatef(0, 0, 0, 1);
        drawRectangle(armHeight, armWidth, armWidth, bodyColor.x + 0.2, bodyColor.y - 0.2, bodyColor.z + 0.2,WIDTH_FROM_CENTER);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(0, 0.3 * totalHeight+armWidth/2, bodyWidth/2 + armWidth/2);
        glRotatef(90 -this->armThetaXZ, 0, 0, 1);
        glRotatef(this->armThetaXY, 1,0,0);
        drawRectangle(armHeight, armWidth, armWidth, bodyColor.x + 0.2, bodyColor.y - 0.2, bodyColor.z + 0.2,WIDTH_FROM_CENTER);
        glPopMatrix();
    } else {
        glPushMatrix();
        glTranslatef(0, 0.3 * totalHeight + armWidth/2, -bodyWidth/2 - armWidth/2);
        glRotatef(0, 0, 0, 1);
        drawRectangle(armHeight, armWidth, armWidth, bodyColor.x + 0.2, bodyColor.y - 0.2, bodyColor.z + 0.2, WIDTH_FROM_CENTER);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(0, 0.3 * totalHeight+armWidth/2, bodyWidth/2 + armWidth/2);
        glRotatef(90 + this->armThetaXZ, 0, 0, 1);
        glRotatef(this->armThetaXY, 1,0,0);
        drawRectangle(armHeight, armWidth, armWidth, bodyColor.x + 0.2, bodyColor.y - 0.2, bodyColor.z + 0.2, WIDTH_FROM_CENTER);
        glPopMatrix();
    }
    glPopMatrix();
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
void Character::StartMoving(bool isToForward) {
    if (isJumping) {
        // if (isToForward) {
        //     this->isFacingForward = true;
        //     this->leg1Theta1 = 40;
        //     this->leg1Theta2 = -100;
        //     this->leg2Theta1 = 45;
        //     this->leg2Theta2 = -100;
        // } else {
        //     this->isFacingForward = false;
        //     this->leg1Theta1 = -40;
        //     this->leg1Theta2 = 100;
        //     this->leg2Theta1 = -45;
        //     this->leg2Theta2 = 100;
        // }
    }
    else if (isToForward) {
        leg1Theta1 = 30;
        leg2Theta1 = -30;
        da = -legMovingSpeed;
        da2 = legMovingSpeed;
    } else {
        leg1Theta1 = -30;
        leg2Theta1 = 30;
        da = legMovingSpeed;
        da2 = -legMovingSpeed;
    }
}

void Character::StartJumping() {
    this->isJumping = true;
    this->jumpingGround = GetCharacterGroundY();
    if(isFacingForward) {
        // this->leg1Theta1 = 40;
        // this->leg1Theta2 = -100;
        // this->leg2Theta1 = 45;
        // this->leg2Theta2 = -100;
    } else {
        // this->leg1Theta1 = -40;
        // this->leg1Theta2 = 100;
        // this->leg2Theta1 = -45;
        // this->leg2Theta2 = 100;
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
        GLdouble incx = this->speed * timeDiff * this->directionVector.x;
        GLdouble incz = -this->speed * timeDiff * this->directionVector.z;
        this->gX += incx;
        this->gZ += incz;
        if (CollidesHorizontallyWithAPlatform(map) == true) {
            this->gX -= incx;
            this->gZ -= incz;
            return;
        }
        if (this->isJumping) { return; }
        this->AnimateLegs();

    } else {
        GLdouble incx = this->speed * timeDiff * -this->directionVector.x;
        GLdouble incz = -this->speed * timeDiff * -this->directionVector.z;
        this->gX += incx;
        this->gZ += incz;

        if (CollidesHorizontallyWithAPlatform(map) == true) {
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
            this->gY += inc;
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

void Character::MoveArmsAngle(GLdouble x, GLdouble y, GLdouble z) {
    GLdouble armX = this->gX;
    GLdouble armY = this->gY + 0.3 * totalHeight;
    // cout << this->gX << " : " << x << endl;
    GLdouble thetaXZ = this->armThetaXZ + (y * (-0.2));
    GLdouble thetaXY = this->armThetaXY + (x * (-0.2));
    if (thetaXZ < 45 && thetaXZ > -45 ) {
        this->armThetaXZ += (y * (-0.2));
    }
    if(thetaXY < 45 && thetaXY > -45) {
        // cout << "Arms angle: " << this->armThetaXY << endl;
        this->armThetaXY += (x * (-0.2));
    }
}

void Character::TurnRight(GLdouble timeDiff) {
    if (this->isJumping == true) {
        return;
    }
    if (this->lookingAngle <= 0) {
        this->lookingAngle = 360;
    }

    this->lookingAngle -= timeDiff * turningSpeed;
    this->CalculateDirectionVector();
    // cout << "Direction Vector: " << this->directionVector.x << " , " << this->directionVector.z << endl;
    // cout << "Looking Angle: " << this->lookingAngle << endl;
}

void Character::CalculateDirectionVector() {
    this->directionVector.x = _cos(this->lookingAngle);
    this->directionVector.z = _sin(this->lookingAngle);
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
    this->CalculateDirectionVector();
    // cout << "Direction Vector: " << this->directionVector.x << " , " << this->directionVector.z << endl;
    // cout << "Looking Angle: " << this->lookingAngle << endl;
}

// Funcao auxiliar de rotacao para posicionar o tiro
void RotatePointUtil(GLdouble x, GLdouble y, GLdouble height, GLdouble angleXZ, GLdouble &xOut, GLdouble &yOut){
    yOut = y + height*sin(angleXZ*M_PI/180);
    xOut = x + height*cos(angleXZ*M_PI/180);
}
// Funcao auxiliar de rotacao para posicionar o tiro
void RotatePointUtil3D(GLdouble x, GLdouble y, GLdouble z, GLdouble height, GLdouble angleXZ, GLdouble angleXY, GLdouble &xOut, GLdouble &yOut, GLdouble &zOut){
    yOut = y + height*sin(angleXZ*M_PI/180)+ height*sin((90-angleXY)*M_PI/180);
    xOut = x + height*cos(angleXZ*M_PI/180)+height*cos(angleXY*M_PI/180);
    zOut = z + height*sin(angleXY*M_PI/180)+height*cos(90-angleXY)*M_PI/180;
}

//Funcao auxiliar para normalizar um vetor a/|a|
void normalize(vec3 &a)
{
    double norm = sqrt(a.x*a.x+a.y*a.y+a.z*a.z); 
    a.x /= norm;
    a.y /= norm;
    a.z /= norm;
}

Shot* Character::Shoot(vec3* target) {
    if (this->ammo > 0) {
        this->ammo -= 1;

        vec3 inicAim = GetInitialShootPosition();
        vec3 finalAim = GetFinalShootPosition();

        vec3 shotDirection = target ? (*target)-finalAim : finalAim-inicAim;

        normalize(shotDirection);
        // cout << "Shot Direction: " << shotDirection.x << "," << shotDirection.y << "," << shotDirection.z << endl;
        Shot* shot = new Shot(finalAim.x, finalAim.y, finalAim.z, shotDirection, armWidth * 0.6, 2*this->speed, this->shootColor);
        return shot;
    }
    return nullptr;
}

vec3 Character::GetInitialShootPosition() {
    GLdouble inicX = gX;
    GLdouble inicY = gY + 0.3 * totalHeight - armWidth/2;
    GLdouble inicZ = gZ;

    // Move  a mira pro lado (eixo z do boneco)
    inicX += (bodyWidth/2+armWidth/2)*_sin(lookingAngle);
    inicZ += (bodyWidth/2+armWidth/2)*_cos(lookingAngle);
    
    return vec3(inicX, inicY, inicZ);
}

vec3 Character::GetFinalShootPosition() {
    vec3 inic = GetInitialShootPosition();
    GLdouble tX, tY, tZ;

    tX = inic.x + armHeight * _cos(lookingAngle+armThetaXY)    * _cos(armThetaXZ); 
    tZ = inic.z + armHeight * _cos(90+lookingAngle+armThetaXY) * _sin(90+armThetaXZ);
    tY = inic.y + armHeight * _sin(armThetaXZ);

    return vec3(tX, tY, tZ);
}

vec3 Character::GetInitialAimPosition() {
    vec3 inic = GetInitialShootPosition();
    GLdouble tX, tY, tZ;

    tX = inic.x + armWidth * _cos(lookingAngle+armThetaXY)    * _cos(armThetaXZ+90); 
    tZ = inic.z + armWidth * _cos(90+lookingAngle+armThetaXY) * _sin(90+armThetaXZ+90);
    tY = inic.y + armWidth * _sin(armThetaXZ+90);

    return vec3(tX, tY, tZ);
}

vec3 Character::GetFinalAimPosition() {
    vec3 inic = GetFinalShootPosition();
    GLdouble tX, tY, tZ;

    tX = inic.x + armWidth * _cos(lookingAngle+armThetaXY)    * _cos(armThetaXZ+90); 
    tZ = inic.z + armWidth * _cos(90+lookingAngle+armThetaXY) * _sin(90+armThetaXZ+90);
    tY = inic.y + armWidth * _sin(armThetaXZ+90);

    return vec3(tX, tY, tZ);
}

void Character::RechargeShot() {
    if(this->ammo <= totalAmmo) {
        this->ammo += 1;
    }
}

bool Character::CollidesDownWithAPlatform(Map* map) {
    if (GetCharacterGroundY() - 0.05 * this->GetTotalHeight() <= this->groundLimit) {
        return true;
    }
    int nPartitions = 72;
    for(double j = 0; j < nPartitions; j++) {
        float theta = 2.0f * M_PI * float(j) / float(nPartitions);//get the current angle
            GLdouble actualX = this->gX + (this->bodyWidth/2) * cosf(theta);
            GLdouble actualZ = this->gZ +(this->bodyWidth/2) * sinf(theta);
        if (map->ColidesWithAPlatform(actualX, this->gY - 0.51*totalHeight, actualZ) == true
            ||
            map->CollidesWithEnemy(actualX, this->gY - 0.51*totalHeight, actualZ) == true
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
            map->CollidesWithEnemy(actualX, this->gY + 0.6*totalHeight, actualZ) == true   
        ) {
            return true;
        }
    }

    return false;
}

bool Character::CollidesHorizontallyWithAPlatform(Map* map) {
    int nPartitions = 72;
    for(double i = -0.48; i<= 0.41; i+=0.1) {
        for(double j = 0; j < nPartitions; j++) {
            float theta = (2.0f * M_PI) * float(j) / float(nPartitions);//get the current angle
            GLdouble actualX = this->gX + (this->bodyWidth/2) * cosf(theta);
            GLdouble actualZ = this->gZ + (this->bodyWidth/2) * sinf(theta);
            if (map->ColidesWithAPlatform(actualX, this->gY + i*totalHeight, actualZ) == true
                ||
                map->CollidesWithEnemy(actualX, this->gY + i*totalHeight, actualZ) == true
            ) {
                return true;
            }
        }
    }
    return false;
}

bool Character::CollidesWithPoint(GLdouble x, GLdouble y, GLdouble z) {
    if(x > this->gX - bodyWidth/2 && x < this->gX + bodyWidth/2) {
        if(y > this->gY - 0.5 * totalHeight && y < this->gY + 0.5 * totalHeight) {
            if (z > this->gZ - bodyWidth/2 && z < this->gZ + bodyWidth/2) {
                return true;
            }
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
    return this->isFacingForward;
}

bool Character::getIsJumping() {
    return this->isJumping;
}

vec3 Character::getPosition() {
    return vec3(this->gX, this->gY, this->gZ);
}

GLdouble Character::GetgX() {
    return this->gX;
}

GLdouble Character::GetgY() {
    return this->gY;
}

GLdouble Character::GetCharacterGroundY() {
    return this->gY - 0.5* this->totalHeight;
}

GLdouble Character::GetCharacterHighestY() {
    return this->gY + 0.5* this->totalHeight;
}

GLdouble Character::GetBodyWidth() {
    return this->bodyWidth;
}

GLdouble Character::GetArmWidth() {
    return this->armWidth;
}

GLdouble Character::GetTotalHeight() {
    return this->totalHeight;
}
