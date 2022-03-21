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
    glEnd();

    // Faces laterais

    glBegin(GL_QUADS); // 3
        glNormal3f(1,0,0);
        glVertex3f (widthEnd, 0, -depth/2);
        glNormal3f(1,0,0);
        glVertex3f (widthEnd, -height, -depth/2);
        glNormal3f(1,0,0);
        glVertex3f (widthEnd, -height, depth/2);
        glNormal3f(1,0,0);
        glVertex3f (widthEnd, 0, depth/2);
    glEnd();
    glBegin(GL_QUADS); // 1
        glNormal3f(-1,0,0);
        glVertex3f (0, 0, -depth/2);
        glNormal3f(-1,0,0);
        glVertex3f (0, -height, -depth/2);
        glNormal3f(-1,0,0);
        glVertex3f (0, -height, depth/2);
        glNormal3f(-1,0,0);
        glVertex3f (0, 0, depth/2);
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
    glEnd();
}


void drawRectangleTextured(GLdouble height, GLdouble width, GLdouble depth, GLfloat R, GLfloat G, GLfloat B, GLuint texture, float texSizeFactor, int slices ) {
    glBindTexture(GL_TEXTURE_2D, texture);

    glColor3f (R, G, B);
    float texH = height/texSizeFactor;
    float texW = width/texSizeFactor;
    float texD = depth/texSizeFactor;

    // face superior
    glNormal3f(0,1,0);
    for (int i = 0; i < slices; i++){
        for (int j = 0; j < slices; j++){
            glBegin(GL_QUADS);
                glNormal3f(0,1,0);
                glTexCoord2f ((texW/slices)*i,(texD/slices)*j);
                    glVertex3f ((width/slices)*i, 0, (depth/slices)*j);
                glNormal3f(0,1,0);
                glTexCoord2f ((texW/slices)*(i+1),(texD/slices)*j);
                    glVertex3f ((width/slices)*(i+1), 0,(depth/slices)*j);
                glNormal3f(0,1,0);
                glTexCoord2f ((texW/slices)*(i+1), (texD/slices)*(j+1));
                    glVertex3f ((width/slices)*(i+1), 0, (depth/slices)*(j+1));
                glNormal3f(0,1,0);
                glTexCoord2f ((texW/slices)*i, (texD/slices)*(j+1));
                    glVertex3f ((width/slices)*i, 0, (depth/slices)*(j+1));
            glEnd();
        
            // Face inferior
            glNormal3f(0,-1,0);
            glBegin(GL_QUADS);
                glNormal3f(0,-1,0);
                glTexCoord2f ((texW/slices)*i,(texD/slices)*j);
                    glVertex3f ((width/slices)*i, -height, (depth/slices)*j);
                glNormal3f(0,-1,0);
                glTexCoord2f ((texW/slices)*(i+1),(texD/slices)*j);
                    glVertex3f ((width/slices)*(i+1), -height,(depth/slices)*j);
                glNormal3f(0,-1,0);
                glTexCoord2f ((texW/slices)*(i+1), (texD/slices)*(j+1));
                    glVertex3f ((width/slices)*(i+1), -height, (depth/slices)*(j+1));
                glNormal3f(0,-1,0);
                glTexCoord2f ((texW/slices)*i, (texD/slices)*(j+1));
                    glVertex3f ((width/slices)*i, -height, (depth/slices)*(j+1));
            glEnd();

            // Faces laterais
            glNormal3f(-1,0,0);
            glBegin(GL_QUADS); // 1
                glNormal3f(-1,0,0);
                glTexCoord2f ((texH/slices)*i,(texD/slices)*j);
                glNormal3f(-1,0,0);
                    glVertex3f (0, (-height/slices)*i, (depth/slices)*j);
                glTexCoord2f ((texH/slices)*(i+1),(texD/slices)*j);
                glNormal3f(-1,0,0);
                    glVertex3f (0, (-height/slices)*(i+1), (depth/slices)*j);
                glTexCoord2f ((texH/slices)*(i+1), (texD/slices)*(j+1));
                glNormal3f(-1,0,0);
                    glVertex3f (0, (-height/slices)*(i+1), (depth/slices)*(j+1));
                glTexCoord2f ((texH/slices)*i, (texD/slices)*(j+1));
                    glVertex3f (0, (-height/slices)*i, (depth/slices)*(j+1));
            glEnd();

            glNormal3f(0,0,1);
            glBegin(GL_QUADS); // 2
                glNormal3f(0,0,1);
                glTexCoord2f ((texW/slices)*i,(texH/slices)*j);
                    glVertex3f ((width/slices)*i, (-height/slices)*j, depth);
                glNormal3f(0,0,1);
                glTexCoord2f ((texW/slices)*(i),(texH/slices)*(j+1));
                    glVertex3f ((width/slices)*i, (-height/slices)*(j+1), depth);
                glNormal3f(0,0,1);
                glTexCoord2f ((texW/slices)*(i+1), (texH/slices)*(j+1));
                    glVertex3f ((width/slices)*(i+1), (-height/slices)*(j+1), depth);
                glNormal3f(0,0,1);
                glTexCoord2f ((texW/slices)*(i+1), (texH/slices)*(j));
                    glVertex3f ((width/slices)*(i+1), (-height/slices)*j, depth);
            glEnd();

            glNormal3f(1,0,0);
            glBegin(GL_QUADS); // 3
                glNormal3f(1,0,0);
                glTexCoord2f ((texH/slices)*i,(texD/slices)*j);
                    glVertex3f (width, (-height/slices)*i, (depth/slices)*j);
                glNormal3f(1,0,0);
                glTexCoord2f ((texH/slices)*(i+1),(texD/slices)*j);
                    glVertex3f (width, (-height/slices)*(i+1), (depth/slices)*j);
                glNormal3f(1,0,0);
                glTexCoord2f ((texH/slices)*(i+1), (texD/slices)*(j+1));
                    glVertex3f (width, (-height/slices)*(i+1), (depth/slices)*(j+1));
                glNormal3f(1,0,0);
                glTexCoord2f ((texH/slices)*i, (texD/slices)*(j+1));
                    glVertex3f (width, (-height/slices)*i, (depth/slices)*(j+1));
            glEnd();

            glNormal3f(0,0, -1);
            glBegin(GL_QUADS); // 4
                glNormal3f(0,0, -1);
                glTexCoord2f ((texW/slices)*i,(texH/slices)*j);
                    glVertex3f ((width/slices)*i, (-height/slices)*j, 0);
                glNormal3f(0,0, -1);
                glTexCoord2f ((texW/slices)*(i+1),(texH/slices)*(j));
                    glVertex3f ((width/slices)*(i+1), (-height/slices)*j, 0);
                glNormal3f(0,0, -1);
                glTexCoord2f ((texW/slices)*(i+1), (texH/slices)*(j+1));
                    glVertex3f ((width/slices)*(i+1), (-height/slices)*(j+1), 0);
                glNormal3f(0,0, -1);
                glTexCoord2f ((texW/slices)*(i), (texH/slices)*(j+1));
                    glVertex3f ((width/slices)*i, (-height/slices)*(j+1), 0);
            glEnd();
        }
    }
}