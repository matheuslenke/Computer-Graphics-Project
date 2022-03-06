#include "headers/platform.h"
#include <math.h>
#include <iostream>

using namespace std;

void Platform::DrawPlatform(GLfloat x, GLfloat y, GLfloat z)
{
    glPushMatrix();

    glTranslatef(x, y, z);

    // Desenha a plataforma
    DrawRectangle(this->sizeY, this->sizeX, this->sizeZ, 0, 0, 0);

    glPopMatrix();
}


void Platform::DrawRectangle(GLfloat height, GLfloat width, GLfloat depth, GLfloat R, GLfloat G, GLfloat B)
{
    glColor3f (R, G, B);
    // Face Superior
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
    glBegin(GL_QUADS);
        glNormal3f(1,0,0);
        glVertex3f (0, 0, -depth/2);
        glNormal3f(1,0,0);
        glVertex3f (0, -height, -depth/2);
        glNormal3f(1,0,0);
        glVertex3f (0, -height, depth/2);
        glNormal3f(1,0,0);
        glVertex3f (0, 0, depth/2);
        glNormal3f(1,0,0);
        glVertex3f (0, 0, -depth/2);
    glEnd();
    glBegin(GL_QUADS);
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
    glBegin(GL_QUADS);
        glNormal3f(0,1,0);
        glVertex3f (width, 0, -depth/2);
        glNormal3f(0,1,0);
        glVertex3f (width, -height, -depth/2);
        glNormal3f(0,1,0);
        glVertex3f (width, -height, depth/2);
        glNormal3f(0,1,0);
        glVertex3f (width, 0, depth/2);
        glNormal3f(0,1,0);
        glVertex3f (width, 0, -depth/2);
    glEnd();
    glBegin(GL_QUADS);
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
}

bool Platform::isColiding(GLfloat x, GLfloat y) {
    if (x >= this->gX && x <= this->gX + this->sizeX) {
        if (y <= this->gY && y >= this->gY - this->sizeY) {
            return true;
        }
    }
    return false;
}