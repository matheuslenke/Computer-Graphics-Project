#include "headers/map.h"
#include <math.h>
#include <iostream>

void Map::DrawMap(GLfloat x, GLfloat y)
{
    glPushMatrix();

    glTranslatef(x, y, 0);

    DrawOutline(); // Desenha contorno dos limites do mapa

    for (Platform platform : this->platforms) {
        platform.Draw();    
    }

    glPopMatrix();
}

void Map::DrawOutline() {
    glColor3f (1, 1, 1);

    glBegin(GL_LINE_LOOP);
        glVertex3f (0, 0, 0.0);
        glVertex3f (this->sizeX, 0, 0.0);
        glVertex3f (this->sizeX , this->sizeY, 0.0);
        glVertex3f (0, this->sizeY, 0.0);
        glVertex3f (0, 0, 0.0);
    glEnd();
}


void Map::DrawRectangle(GLint height, GLint width, GLfloat R, GLfloat G, GLfloat B)
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

void Map::CreateMapFromSVG() {
    Platform p1 = Platform(0, 0, 50, 20);
    this->platforms.push_back(p1);
}