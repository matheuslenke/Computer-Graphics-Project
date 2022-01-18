#include "headers/platform.h"
#include <math.h>
#include <iostream>

using namespace std;

void Platform::DrawPlatform(GLfloat x, GLfloat y)
{
    glPushMatrix();

    glTranslatef(x, y, 0);
    // cout << "Desenhando plataforma: (" << x << " , " << y << ")" << endl;

    // Desenha a plataforma
    DrawRectangle(this->sizeY, this->sizeX, 0, 0, 0);

    glPopMatrix();
}


void Platform::DrawRectangle(GLfloat height, GLfloat width, GLfloat R, GLfloat G, GLfloat B)
{
    glColor3f (R, G, B);

    glBegin(GL_POLYGON);
        glVertex3f (0, 0, 0.0);
        glVertex3f (width, 0, 0.0);
        glVertex3f (width , -height, 0.0);
        glVertex3f (0, -height, 0.0);
        glVertex3f (0, 0, 0.0);
    glEnd();
}

bool Platform::isColiding(GLfloat x, GLfloat y) {
    if (x > this->gX && x < this->gX + this->sizeX) {
        if (y < this->gY && y > this->gY - this->sizeY) {
            return true;
        }
    }
    return false;
}