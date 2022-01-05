#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <iostream>
#include "headers/character.h"
#include "headers/platform.h"
#include "headers/map.h"
#include "headers/camera.h"

Character character = Character(0, 30, 60); // Personagem principal
Map map = Map(0,0, 1000, 500); // Mapa do nível
Camera camera = Camera(vec3(0, 250, 0)); // Objeto para controlar camera

// Window dimensions
const GLint Width = 700;
const GLint Height = 700;

// Viewing dimensions
const GLint ViewingWidth = 500;
const GLint ViewingHeight = 500;

int keyStatus[256];
int cursorPressStatus = 1;

void renderScene(void)
{
    // Clear the screen.
    glClear(GL_COLOR_BUFFER_BIT);

    camera.Activate();

    map.Draw();
    character.Draw();

    glutSwapBuffers(); // Desenha the new frame of the game.
}


void ResetKeyStatus()
{
    int i;
    //Initialize keyStatus
    for(i = 0; i < 256; i++)
       keyStatus[i] = 0; 
}


void init (void) 
{
   ResetKeyStatus();
    // The color the windows will redraw. Its done to erase the previous frame.
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black, no opacity(alpha).
 
    glMatrixMode(GL_PROJECTION); // Select the projection matrix    
    glOrtho(-(ViewingWidth/2),     // X coordinate of left edge             
            (ViewingWidth/2),     // X coordinate of right edge            
            -(ViewingHeight/2),     // Y coordinate of bottom edge             
            (ViewingHeight/2),     // Y coordinate of top edge             
            -100,     // Z coordinate of the “near” plane            
            100);    // Z coordinate of the “far” plane
    glMatrixMode(GL_MODELVIEW); // Select the projection matrix    
    glLoadIdentity();
}

 /* Callback de tecla pressionada */
void keyPress(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'a':
        case 'A':
             if (keyStatus[(int)('a')] == 0 && character.getIsDirectionToRight()) 
             {character.StartMoving(false);}
             keyStatus[(int)('a')] = 1; //Using keyStatus trick
             break;
        case 'd':
        case 'D':
             if (keyStatus[(int)('d')] == 0 && !character.getIsDirectionToRight()) 
             {character.StartMoving(true);}
             keyStatus[(int)('d')] = 1; //Using keyStatus trick
             break;
        case 'j':
            keyStatus[(int)('j')] = 1;
            break;  
        case 'k':
            keyStatus[(int)('k')] = 1;
            break;
        case 'l':
            keyStatus[(int)('l')] = 1;
            break;
        case 'i':
            keyStatus[(int)('i')] = 1;
            break;
        case 27 :
             exit(0);
    }
    glutPostRedisplay();
}

void keyUp(unsigned char key, int x, int y)
{
    keyStatus[(int)(key)] = 0;
    glutPostRedisplay();
}

void idle (void) {

    static GLdouble previousTime = glutGet(GLUT_ELAPSED_TIME);
    GLdouble currentTime, timeDifference;
    //Pega o tempo que passou do inicio da aplicacao
    currentTime = glutGet(GLUT_ELAPSED_TIME);
    // Calcula o tempo decorrido desde de a ultima frame.
    timeDifference = currentTime - previousTime;
    //Atualiza o tempo do ultimo frame ocorrido
    previousTime = currentTime;

    if(keyStatus[(int)('a')])
    {
        character.MoveInX(false, timeDifference);
        camera.Left(timeDifference);
    }
    if(keyStatus[(int)('d')])
    {
        camera.Right(timeDifference);
        character.MoveInX(true, timeDifference);
    }
    if (cursorPressStatus == 0) {
        character.MoveInY(timeDifference, true);
    } else {
        character.MoveInY(timeDifference, false);
    }

    // if(keyStatus[(int)('j')]) {
    //     camera.Left();
    // }
    // if(keyStatus[(int)('k')]) {
    //     camera.Back();
    // }
    // if(keyStatus[(int)('l')]) {
    //     camera.Right();
    // }
    // if(keyStatus[(int)('i')]) {
    //     camera.Forward();
    // }

   glutPostRedisplay();
}

void mouseClick(int button, int state, int x, int y) {
    if(button == 0) {
        cursorPressStatus = state;
        if(!character.getIsJumping()) {
            character.StartJumping();
        }
    }
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);

    // Create the Window
    glutInitWindowSize(Width, Height);
    glutInitWindowPosition (150, 50);
    glutCreateWindow ("Trabalho 1 - Matheus Lenke");
    glutDisplayFunc(renderScene); 

    // Registrar Funções
    glutKeyboardFunc(keyPress);
    glutKeyboardUpFunc(keyUp);
    glutIdleFunc(idle);
    glutMouseFunc(mouseClick);

    init ();

    // Criação do mapa do jogo
    map.CreateMapFromSVG();

    glutMainLoop();

   return 0;
}
