#include "headers/geom_draw.h"

#define WIDTH_FROM_BOTTOM -1
#define WIDTH_FROM_CENTER 0

void drawRectangle(GLdouble height, GLdouble width, GLdouble depth, GLfloat R, GLfloat G, GLfloat B, int widthDrawStart) {
    /* Define cor dos vértices com os valores R, G e B variando de 0.0 a 1.0 */
    glColor3f (R, G, B);

    GLdouble widthStart, widthEnd;

    if(widthDrawStart == WIDTH_FROM_BOTTOM){
      widthStart = 0;
      widthEnd = width;
    } else if (widthDrawStart == WIDTH_FROM_CENTER) {
      widthStart = -width/2;
      widthEnd = width/2;
    }



    glBegin(GL_QUADS);
        glNormal3f(0,1,0);
        glVertex3f (widthStart, 0, -depth/2);
        glNormal3f(0,1,0);
        glVertex3f (widthEnd, 0,-depth/2);
        glNormal3f(0,1,0);
        glVertex3f (widthEnd, 0, depth/2);
        glNormal3f(0,1,0);
        glVertex3f (widthStart, 0, depth/2);
        glNormal3f(0,1,0);
        glVertex3f (widthStart, 0, -depth/2);
    glEnd();

    // Face inferior
    glBegin(GL_QUADS);
        glNormal3f(0,-1,0);
        glVertex3f (widthStart, -height, -depth/2);
        glNormal3f(0,-1,0);
        glVertex3f (widthEnd, -height,-depth/2);
        glNormal3f(0,-1,0);
        glVertex3f (widthEnd , -height, depth/2);
        glNormal3f(0,-1,0);
        glVertex3f (widthStart, -height, depth/2);
        glNormal3f(0,-1,0);
        glVertex3f (widthStart, -height, -depth/2);
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
        glVertex3f (widthStart, 0, depth/2);
        glNormal3f(0,0,1);
        glVertex3f (widthStart, -height, depth/2);
        glNormal3f(0,0,1);
        glVertex3f (widthEnd, -height, depth/2);
        glNormal3f(0,0,1);
        glVertex3f (widthEnd, 0, depth/2);
        glNormal3f(0,0,1);
        glVertex3f (widthStart, 0, depth/2);
    glEnd();
    glBegin(GL_QUADS); // 3
        glNormal3f(1,0,0);
        glVertex3f (widthEnd, 0, -depth/2);
        glNormal3f(1,0,0);
        glVertex3f (widthEnd, -height, -depth/2);
        glNormal3f(1,0,0);
        glVertex3f (widthEnd, -height, depth/2);
        glNormal3f(1,0,0);
        glVertex3f (widthEnd, 0, depth/2);
        glNormal3f(1,0,0);
        glVertex3f (widthEnd, 0, -depth/2);
    glEnd();
    glBegin(GL_QUADS); // 4
        glNormal3f(0,0, -1);
        glVertex3f (widthStart, 0, -depth/2);
        glNormal3f(0,0, -1);
        glVertex3f (widthEnd, 0, -depth/2);
        glNormal3f(0,0, -1);
        glVertex3f (widthEnd, -height, -depth/2);
        glNormal3f(0,0, -1);
        glVertex3f (widthStart, -height, -depth/2);
        glNormal3f(0,0, -1);
        glVertex3f (widthStart, 0, -depth/2);
    glEnd();
}