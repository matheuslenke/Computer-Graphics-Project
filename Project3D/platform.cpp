#include "headers/platform.h"
#include <math.h>
#include <iostream>

using namespace std;

void Platform::DrawPlatform(GLfloat x, GLfloat y, GLfloat z)
{
    glPushMatrix();

    glTranslatef(x, y, z);

    // Desenha a plataforma
    DrawRectangle(this->sizeY, this->sizeX, this->sizeZ, 0.8, 0.8, 0.8);

    glPopMatrix();
}


void Platform::DrawRectangle(GLfloat height, GLfloat width, GLfloat depth, GLfloat R, GLfloat G, GLfloat B)
{
    glColor3f (R, G, B);
    int slices = 16;
    // Face Superior
    glNormal3f(0,1,0);
    for (int i = 0; i < slices; i++){
        for (int j = 0; j < slices; j++){
            glBegin(GL_QUADS);
                glVertex3f ((width/slices)*i, 0, (depth/slices)*j);
                glVertex3f ((width/slices)*(i+1), 0,(depth/slices)*j);
                glVertex3f ((width/slices)*(i+1), 0, (depth/slices)*(j+1));
                glVertex3f ((width/slices)*i, 0, (depth/slices)*(j+1));
                glVertex3f ((width/slices)*i, 0, (depth/slices)*j);
            glEnd();
        
            // Face inferior
            glNormal3f(0,-1,0);
            glBegin(GL_QUADS);
                glVertex3f ((width/slices)*i, -height, (depth/slices)*j);
                glVertex3f ((width/slices)*(i+1), -height,(depth/slices)*j);
                glVertex3f ((width/slices)*(i+1), -height, (depth/slices)*(j+1));
                glVertex3f ((width/slices)*i, -height, (depth/slices)*(j+1));
                glVertex3f ((width/slices)*i, -height, (depth/slices)*j);
            glEnd();

            // Faces laterais
            glNormal3f(-1,0,0);
            glBegin(GL_QUADS); // 1
                glVertex3f (0, (-height/slices)*i, (depth/slices)*j);
                glVertex3f (0, (-height/slices)*(i+1), (depth/slices)*j);
                glVertex3f (0, (-height/slices)*(i+1), (depth/slices)*(j+1));
                glVertex3f (0, (-height/slices)*i, (depth/slices)*(j+1));
                glVertex3f (0, (-height/slices)*i, (depth/slices)*j);
            glEnd();

            glNormal3f(0,0,1);
            glBegin(GL_QUADS); // 2
                glVertex3f ((width/slices)*i, (-height/slices)*j, depth);
                glVertex3f ((width/slices)*i, (-height/slices)*(j+1), depth);
                glVertex3f ((width/slices)*(i+1), (-height/slices)*(j+1), depth);
                glVertex3f ((width/slices)*(i+1), (-height/slices)*j, depth);
                glVertex3f ((width/slices)*i, (-height/slices)*j, depth);
            glEnd();

            glNormal3f(1,0,0);
            glBegin(GL_QUADS); // 3
                glVertex3f (width, (-height/slices)*i, (depth/slices)*j);
                glVertex3f (width, (-height/slices)*(i+1), (depth/slices)*j);
                glVertex3f (width, (-height/slices)*(i+1), (depth/slices)*(j+1));
                glVertex3f (width, (-height/slices)*i, (depth/slices)*(j+1));
                glVertex3f (width, (-height/slices)*i, (depth/slices)*j);
            glEnd();

            glNormal3f(0,0, -1);
            glBegin(GL_QUADS); // 4
                glVertex3f ((width/slices)*i, (-height/slices)*j, 0);
                glVertex3f ((width/slices)*(i+1), (-height/slices)*j, 0);
                glVertex3f ((width/slices)*(i+1), (-height/slices)*(j+1), 0);
                glVertex3f ((width/slices)*i, (-height/slices)*(j+1), 0);
                glVertex3f ((width/slices)*i, (-height/slices)*j, 0);
            glEnd();
        }
    }
}

bool Platform::isColiding(GLfloat x, GLfloat y, GLfloat z) {
    if (x >= this->gX && x <= this->gX + this->sizeX) {
        if (y <= this->gY && y >= this->gY - this->sizeY) {
                return true;
        }
    }
    return false;
}