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
#include <list>
#include <vector>

#include "headers/character.h"
#include "headers/enemy.h"
#include "headers/platform.h"
#include "headers/map.h"
#include "headers/camera.h"
#include "headers/shot.h"
#include "tinyxml2.h"


using namespace std;
using namespace tinyxml2;

// Window dimensions
const GLint Width = 500;
const GLint Height = 500;

// Viewing dimensions
GLint ViewingWidth = 500;
GLint ViewingHeight = 500;

GLfloat cameraX = 0;
GLfloat cameraY = 0;

int keyStatus[256];
int cursorPressStatus = 1;

// FPS
const float FPS = 60;

// Entidades do jogo
GLfloat playerHeight = 60;
Character* player = nullptr; // Personagem principal
Map* map = nullptr; // Mapa do nível
vector<Shot*> playerShots;

// Parametros de ação dos inimigos
GLfloat changeActionActualTime = 0;
const GLfloat timeToChangeAction = 2; // Tempo em segundos

void moveCamera() {
    glMatrixMode(GL_PROJECTION); // Select the projection matrix   
    glLoadIdentity(); 
    GLfloat xOff = player->GetgX() - (ViewingWidth / 2);
    // cout << "Ortho: ( " << player->GetgX() << " , " << player->GetgX() + ViewingWidth/2 << " , " << cameraY << " , " << cameraY + ViewingHeight << ")" << endl;
    // cout << "Player xOff: " << xOff << endl;
    glOrtho(xOff,     // X coordinate of left edge             
            xOff + ViewingWidth,     // X coordinate of right edge            
            cameraY,     // Y coordinate of bottom edge             
            cameraY + ViewingHeight,     // Y coordinate of top edge             
            -1.0,     // Z coordinate of the “near” plane            
            1.0);    // Z coordinate of the “far” plane
    glMatrixMode(GL_MODELVIEW); // Select the projection matrix    
    glLoadIdentity();
}

void renderScene(void)
{
    // Clear the screen.
    glClear(GL_COLOR_BUFFER_BIT);

    moveCamera();
    map->Draw();
    player->Draw();

    for(Shot* shot : playerShots) {
        shot->Draw();
    }

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

    moveCamera(); 
}

 /* Callback de tecla pressionada */
void keyPress(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'a':
        case 'A':
             if (keyStatus[(int)('a')] == 0 && player->getIsDirectionToRight()) 
             {player->StartMoving(false);}
             keyStatus[(int)('a')] = 1; //Using keyStatus trick
             break;
        case 'd':
        case 'D':
             if (keyStatus[(int)('d')] == 0 && !player->getIsDirectionToRight()) 
             {player->StartMoving(true);}
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
    GLdouble currentTime, timeDifference;
    do {
        static GLdouble previousTime =  glutGet(GLUT_ELAPSED_TIME);
        //Pega o tempo que passou do inicio da aplicacao
        currentTime = glutGet(GLUT_ELAPSED_TIME);
        // Calcula o tempo decorrido desde de a ultima frame.
        timeDifference = currentTime - previousTime;
        //Atualiza o tempo do ultimo frame ocorrido
        previousTime = currentTime;
    } while (timeDifference < 1.0 / FPS);

    // <--- Movimento do Player --->
    if(keyStatus[(int)('a')])
    {
        player->MoveInX(false, timeDifference, map);
    }
    if(keyStatus[(int)('d')])
    {
        player->MoveInX(true, timeDifference, map);
    }
    if (cursorPressStatus == 0) {
        player->MoveInY(timeDifference, true, map);
    } else {
        player->MoveInY(timeDifference, false, map);
    }

    // Movimento dos tiros
    for (vector<Shot*>::iterator index = playerShots.begin(); index != playerShots.end(); index++) {
        Shot* shot = *index;
        if (shot) {
            // cout << "Vai avaliar" << endl;
            bool isShotValid = shot->Valid(map);
            // cout << "avaliou" << endl;
            if (!isShotValid){ 
                // cout << "Tiro invalido!" << endl;
                // cout << shot << endl;
                // playerShots.erase(index);
                // player->RechargeShot();
                // delete shot;
            } else {
                shot->Move(timeDifference);
            }
        }
    }

    // <--- Movimento dos inimigos --->
    GLdouble enemyTimeDiff = currentTime - changeActionActualTime;
    if(enemyTimeDiff >= 1 * 1000) { // Muda a cada 1 segundo
        map->ChangeEnemiesActions();
        changeActionActualTime = glutGet(GLUT_ELAPSED_TIME);
    }
    map->ExecuteEnemiesActions(timeDifference);

   glutPostRedisplay();
}

void mouseClick(int button, int state, int x, int y) {
    if(button == 0) {
        cursorPressStatus = state;
        if(!player->getIsJumping()) {
            player->StartJumping();
        }
    } else if(button == 2) {
        Shot* shot= player->Shoot();
        playerShots.push_back(shot);
    }
}

void mouseMove(int x, int y) {
    y = (Height - y);
    GLfloat normalizedY = (float)y /(float) Height;
    GLfloat normalizedX = (float)x /(float) Width;

    GLfloat transformedX = ( ViewingWidth - (player->GetgX() - (ViewingWidth/2))) * normalizedX;
    // cout << "x: " << transformedX << "," << normalizedY << endl;
    player->MoveArmsAngle(x * 2, y * 2);

    glutPostRedisplay();
}

void readXMLFile(string filename) {
    XMLDocument mapDocument;
    mapDocument.LoadFile(filename.c_str());

    XMLElement* svg = mapDocument.FirstChildElement("svg");
    XMLElement* aux;

    for(aux = svg->FirstChildElement("rect"); aux != nullptr; aux = aux->NextSiblingElement()) {
        if(strcmp(aux->Attribute("fill"),"blue") == 0) {
            ViewingHeight = aux->DoubleAttribute("height");
            ViewingWidth = aux->DoubleAttribute("height");
            map = new Map(aux->DoubleAttribute("x"), aux->DoubleAttribute("y"), aux->DoubleAttribute("width"), aux->DoubleAttribute("height") );
            cameraY = aux->DoubleAttribute("y"); 
        }
        else if(strcmp(aux->Attribute("fill"), "green") == 0) {
            player = new Character(aux->DoubleAttribute("cx"), - aux->FloatAttribute("cy") + (map->GetSizeY() + 2 * map->GetgY()), 2 * aux->DoubleAttribute("r")  , map->GetgY(), vec3(0, 1, 0));
        } else if (strcmp(aux->Attribute("fill"), "black") == 0) {
            Platform p = Platform( aux->DoubleAttribute("x"), - aux->FloatAttribute("y") + (map->GetSizeY() + 2 * map->GetgY())  , aux->DoubleAttribute("width"), aux->DoubleAttribute("height"));
            map->AddPlatform(p);
        } else if(strcmp(aux->Attribute("fill"), "red") == 0) {
            Enemy* enemy = new Enemy(aux->DoubleAttribute("cx"), - aux->FloatAttribute("cy") + (map->GetSizeY() + 2 * map->GetgY()), 2 * aux->DoubleAttribute("r")  , map->GetgY(), vec3(1, 0, 0));
            
            // Gerando e inserindo padrão aleatório de ações
            int random;
            for(int i = 0; i < 20; i++) {
                random = rand() % 3;
                enemy->InsertAction(random);
            }
            map->AddEnemy(enemy);

            // enemies.push_back(enemy);
        }
    }
}


int main(int argc, char** argv)
{
    readXMLFile("arena_teste.svg");

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
    glutPassiveMotionFunc(mouseMove);

    init();

    glutMainLoop();

   return 0;
}
