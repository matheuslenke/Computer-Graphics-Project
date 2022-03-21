#include "headers/shot.h"
#include <math.h>
#include <iostream>
#define DISTANCIA_MAX 500

void Shot::DrawCirc(GLdouble radius, GLfloat R, GLfloat G, GLfloat B)
{
    glColor3f(R, G, B);
    int numsegments = 20;

    glutSolidSphere(radius, 20, 10);

    // glBegin(GL_POLYGON);
    //   for(int i = 0; i < numsegments; i++)
    //   {
    //     float theta = 2.0f * 3.1415926f * float(i) / float(numsegments);//get the current angle

    //     float x = radius * cosf(theta);//calculate the x component
    //     float y = radius * sinf(theta);//calculate the y component

    //     glVertex2f(x, y);//output vertex
    //   }
    // glEnd();
}

void Shot::DrawShot(GLdouble x, GLdouble y, GLdouble z)
{
    glPushMatrix();
    glTranslatef(x, y, z);
    DrawCirc(this->shotRadius, this->color.x, this->color.y, this->color.z);
    glPopMatrix();
}

void Shot::Move(GLdouble timeDifference)
{
    GLdouble incx = this->gVel * timeDifference * this->directionVector.x;
    GLdouble incy = this->gVel * timeDifference * this->directionVector.y;
    GLdouble incz = this->gVel * timeDifference * this->directionVector.z;
    // cout << "Shot direction vector: " << incx << "," << incy << "," << incz << endl;
    gX += incx;
    gZ += incz;
    gY += incy;
}

bool Shot::Valid(Map* map)
{
    if (this->hitted == true) { return false; }
    GLboolean isValid = true;
    int numSegments = 20;
    for(int j = 0; j <= numSegments ; j+=1) {
        float theta = 2.0f * 3.1415926f * float(j) / float(numSegments);//get the current angle
        float x = this->shotRadius * cosf(theta);//calculate the x component
        float y = this->shotRadius * sinf(theta);//calculate the y component
        if(map->ColidesWithAPlatform(this->gX + x, this->gY + y, this->gZ) == true) {
            isValid = false;
            break;
        }
    }
    return isValid;
}

GLfloat Shot::GetRadius() {
    return this->shotRadius;
}