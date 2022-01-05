#include "headers/platform.h"
#include <math.h>
#include <iostream>

void Platform::DrawPlatform(GLfloat x, GLfloat y)
{
    glPushMatrix();

    glTranslatef(x, y, 0);

    // Desenha o corpo do personagem
    DrawRectangle(this->sizeY, this->sizeX, 1, 0, 0);

    glPopMatrix();
}


void Platform::DrawRectangle(GLint height, GLint width, GLfloat R, GLfloat G, GLfloat B)
{
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