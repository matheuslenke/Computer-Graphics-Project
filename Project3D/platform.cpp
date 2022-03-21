#include "headers/platform.h"
#include <math.h>
#include <iostream>

using namespace std;

void Platform::DrawPlatform(GLfloat x, GLfloat y, GLfloat z, GLuint texture)
{
    glPushMatrix();

    glTranslatef(x, y, z);

    glBindTexture(GL_TEXTURE_2D, texture);

    // Desenha a plataforma
    DrawRectangle(this->sizeY, this->sizeX, this->sizeZ, 0.8, 0.8, 0.8);

    glPopMatrix();
}


void Platform::DrawRectangle(GLfloat height, GLfloat width, GLfloat depth, GLfloat R, GLfloat G, GLfloat B)
{
    glColor3f (R, G, B);
    int slices = 16;

    float texSize = 5;

    float texH = height/texSize;
    float texW = width/texSize;
    float texD = depth/texSize;

    // face superior
    glNormal3f(0,1,0);
    for (int i = 0; i < slices; i++){
        for (int j = 0; j < slices; j++){
            glBegin(GL_QUADS);
                glTexCoord2f ((texW/slices)*i,(texD/slices)*j);
                    glVertex3f ((width/slices)*i, 0, (depth/slices)*j);
                glTexCoord2f ((texW/slices)*(i+1),(texD/slices)*j);
                    glVertex3f ((width/slices)*(i+1), 0,(depth/slices)*j);
                glTexCoord2f ((texW/slices)*(i+1), (texD/slices)*(j+1));
                    glVertex3f ((width/slices)*(i+1), 0, (depth/slices)*(j+1));
                glTexCoord2f ((texW/slices)*i, (texD/slices)*(j+1));
                    glVertex3f ((width/slices)*i, 0, (depth/slices)*(j+1));
            glEnd();
        
            // Face inferior
            glNormal3f(0,-1,0);
            glBegin(GL_QUADS);
                glTexCoord2f ((texW/slices)*i,(texD/slices)*j);
                    glVertex3f ((width/slices)*i, -height, (depth/slices)*j);
                glTexCoord2f ((texW/slices)*(i+1),(texD/slices)*j);
                    glVertex3f ((width/slices)*(i+1), -height,(depth/slices)*j);
                glTexCoord2f ((texW/slices)*(i+1), (texD/slices)*(j+1));
                    glVertex3f ((width/slices)*(i+1), -height, (depth/slices)*(j+1));
                glTexCoord2f ((texW/slices)*i, (texD/slices)*(j+1));
                    glVertex3f ((width/slices)*i, -height, (depth/slices)*(j+1));
            glEnd();

            // Faces laterais
            glNormal3f(-1,0,0);
            glBegin(GL_QUADS); // 1
                glTexCoord2f ((texH/slices)*i,(texD/slices)*j);
                    glVertex3f (0, (-height/slices)*i, (depth/slices)*j);
                glTexCoord2f ((texH/slices)*(i+1),(texD/slices)*j);
                    glVertex3f (0, (-height/slices)*(i+1), (depth/slices)*j);
                glTexCoord2f ((texH/slices)*(i+1), (texD/slices)*(j+1));
                    glVertex3f (0, (-height/slices)*(i+1), (depth/slices)*(j+1));
                glTexCoord2f ((texH/slices)*i, (texD/slices)*(j+1));
                    glVertex3f (0, (-height/slices)*i, (depth/slices)*(j+1));
            glEnd();

            glNormal3f(0,0,1);
            glBegin(GL_QUADS); // 2
                glTexCoord2f ((texW/slices)*i,(texH/slices)*j);
                    glVertex3f ((width/slices)*i, (-height/slices)*j, depth);
                glTexCoord2f ((texW/slices)*(i),(texH/slices)*(j+1));
                    glVertex3f ((width/slices)*i, (-height/slices)*(j+1), depth);
                glTexCoord2f ((texW/slices)*(i+1), (texH/slices)*(j+1));
                    glVertex3f ((width/slices)*(i+1), (-height/slices)*(j+1), depth);
                glTexCoord2f ((texW/slices)*(i+1), (texH/slices)*(j));
                    glVertex3f ((width/slices)*(i+1), (-height/slices)*j, depth);
            glEnd();

            glNormal3f(1,0,0);
            glBegin(GL_QUADS); // 3
                glTexCoord2f ((texH/slices)*i,(texD/slices)*j);
                    glVertex3f (width, (-height/slices)*i, (depth/slices)*j);
                glTexCoord2f ((texH/slices)*(i+1),(texD/slices)*j);
                    glVertex3f (width, (-height/slices)*(i+1), (depth/slices)*j);
                glTexCoord2f ((texH/slices)*(i+1), (texD/slices)*(j+1));
                    glVertex3f (width, (-height/slices)*(i+1), (depth/slices)*(j+1));
                glTexCoord2f ((texH/slices)*i, (texD/slices)*(j+1));
                    glVertex3f (width, (-height/slices)*i, (depth/slices)*(j+1));
            glEnd();

            glNormal3f(0,0, -1);
            glBegin(GL_QUADS); // 4
                glTexCoord2f ((texW/slices)*i,(texH/slices)*j);
                    glVertex3f ((width/slices)*i, (-height/slices)*j, 0);
                glTexCoord2f ((texW/slices)*(i+1),(texH/slices)*(j));
                    glVertex3f ((width/slices)*(i+1), (-height/slices)*j, 0);
                glTexCoord2f ((texW/slices)*(i+1), (texH/slices)*(j+1));
                    glVertex3f ((width/slices)*(i+1), (-height/slices)*(j+1), 0);
                glTexCoord2f ((texW/slices)*(i), (texH/slices)*(j+1));
                    glVertex3f ((width/slices)*i, (-height/slices)*(j+1), 0);
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