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
const GLdouble FPS = 10.0;

// Entidades do jogo
Character* player = nullptr; // Personagem principal
vector<Shot*> playerShots;
Map* map = nullptr; // Mapa do nível
GLboolean gameOver = false;
GLboolean gameWin = false;
string filename = "";

// Parametros de ação dos inimigos
GLfloat changeActionActualTime = 0;
const GLfloat timeToChangeAction = 2; // Tempo em segundos
const GLint playerAmmo = 3;
const GLint enemyAmmo = 1;
GLboolean enemyStopMoving = false;

// Impressao na tela
void * font = GLUT_BITMAP_9_BY_15;

void moveCamera() {
    glMatrixMode(GL_PROJECTION); // Select the projection matrix   
    glLoadIdentity(); 
    GLfloat xOff = player->GetgX() - (ViewingWidth / 2);
    glOrtho(xOff,     // X coordinate of left edge             
            xOff + ViewingWidth,     // X coordinate of right edge            
            cameraY,     // Y coordinate of bottom edge             
            cameraY + ViewingHeight,     // Y coordinate of top edge             
            -1.0,     // Z coordinate of the “near” plane            
            1.0);    // Z coordinate of the “far” plane
    glMatrixMode(GL_MODELVIEW); // Select the projection matrix    
    glLoadIdentity();
}

void PrintOnScreen(GLfloat x, GLfloat y, string text)
{
    glColor3f(1.0, 1.0, 1.0);
    glRasterPos2f(x, y);

    int i, len;
    len = text.length();
    for(i = 0; i < len ; i++) {
        glutBitmapCharacter(font, text[i]);
    }
}

void renderScene(void)
{
    // Clear the screen.
    glClear(GL_COLOR_BUFFER_BIT);
    if(gameOver == true) {
        string text {"Game Over! press r to restart"};
        GLfloat xOff = player->GetgX() - (ViewingWidth / 2);
        PrintOnScreen(xOff + ViewingWidth/4, cameraY + ViewingHeight/2, text);
    } else if (gameWin == true) {
        string text {"You win! press r to restart"};

        GLfloat xOff = player->GetgX() - (ViewingWidth / 2);
        PrintOnScreen(xOff + ViewingWidth/3, cameraY + ViewingHeight/2, "Congratulations!");
        PrintOnScreen(xOff + ViewingWidth/4, cameraY + ViewingHeight/3, text);
    } else {
        moveCamera();
        map->Draw();
        player->Draw();

        for (Shot* shot : playerShots ) {
            shot->Draw();
        }
        map->DrawShots(); // Tiros dos inimigos
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

void readXMLFile(string filename) {
    XMLDocument mapDocument;
    mapDocument.LoadFile(filename.c_str());

    XMLElement* svg = mapDocument.FirstChildElement("svg");
    XMLElement* aux;
    vector<Enemy*> enemies;
    vector<Platform> platforms;
    for(aux = svg->FirstChildElement("rect"); aux != nullptr; aux = aux->NextSiblingElement()) {
        if(strcmp(aux->Attribute("fill"),"blue") == 0) {
            ViewingHeight = aux->DoubleAttribute("height");
            ViewingWidth = aux->DoubleAttribute("height");
            map = new Map(aux->DoubleAttribute("x"), aux->DoubleAttribute("y"), aux->DoubleAttribute("width"), aux->DoubleAttribute("height") );
            cameraY = aux->DoubleAttribute("y"); 
        }
        else if(strcmp(aux->Attribute("fill"), "green") == 0) {
            player = new Character(aux->DoubleAttribute("cx"), - aux->FloatAttribute("cy") + (map->GetSizeY() + 2 * map->GetgY()), 2 * aux->DoubleAttribute("r")  , map->GetgY(), vec3(0, 1, 0), vec3(1, 1, 1), playerAmmo);
        } else if (strcmp(aux->Attribute("fill"), "black") == 0) {
            Platform p = Platform( aux->DoubleAttribute("x"), - aux->FloatAttribute("y") + (map->GetSizeY() + 2 * map->GetgY())  , aux->DoubleAttribute("width"), aux->DoubleAttribute("height"));
            platforms.push_back(p);
        } else if(strcmp(aux->Attribute("fill"), "red") == 0) {
            Enemy* enemy = new Enemy(aux->DoubleAttribute("cx"), - aux->FloatAttribute("cy") + (map->GetSizeY() + 2 * map->GetgY()), 2 * aux->DoubleAttribute("r")  , map->GetgY(), vec3(1, 0, 0), vec3(0.7, 0.3, 0.3), enemyAmmo);
            // Gerando e inserindo padrão aleatório de ações
            int random;
            for(int i = 0; i < 20; i++) {
                random = rand() % 2;
                enemy->InsertAction(random);
            }
            enemies.push_back(enemy);
        }
    }
    for (Enemy* enemy : enemies) {
        map->AddEnemy(enemy);
    }
    for(Platform p : platforms) {
        map->AddPlatform(p);
    }
}

void restartGame() {
    gameOver = false;
    gameWin = false;
    ResetKeyStatus();
    delete player;
    player = nullptr;
    delete map;
    map = nullptr;
    readXMLFile(filename);
}

 /* Callback de tecla pressionada */
void keyPress(unsigned char key, int x, int y)
{
    if (gameOver || gameWin) { 
        if(key == 'R' || key == 'r') {
            restartGame();
        }
        return;
     }
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
        case 'p':
            enemyStopMoving = !enemyStopMoving;
            break;
        case 27 :
             exit(0);
    }
    glutPostRedisplay();
}

void keyUp(unsigned char key, int x, int y)
{
    if(gameOver || gameWin ) { return; }
    keyStatus[(int)(key)] = 0;
    glutPostRedisplay();
}

void idle (void) {
    GLdouble currentTime, timeDifference;
    static GLdouble previousTime =  glutGet(GLUT_ELAPSED_TIME);
    //Pega o tempo que passou do inicio da aplicacao
    currentTime = glutGet(GLUT_ELAPSED_TIME);
    // Calcula o tempo decorrido desde de a ultima frame.
    timeDifference = currentTime - previousTime;
    //Atualiza o tempo do ultimo frame ocorrido
    previousTime = currentTime;
    
    if (gameOver || gameWin) { return; }

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

    // <--- Checando se o usuário ganhou o jogo --->
    if(player->CollidesWithEndOfMap(map)) {
        gameWin = true;
    }

    // Lógica de movimentação dos tiros do player
    for (vector<Shot*>::const_iterator index = playerShots.begin(); index != playerShots.end(); ) {
        Shot* shot = *index;
        if (shot) {
            bool isShotValid = shot->Valid(map);
            if (!isShotValid){ 
                playerShots.erase(index);
                delete shot;
                player->RechargeShot();
            } else {
                index++;
                shot->Move(timeDifference);
            }
        }
    }

    // Movimento dos tiros dos inimigos
    map->MoveShots(timeDifference);

    // <--- Movimento dos inimigos --->
    map->ExecuteEnemiesActions(timeDifference, player, enemyStopMoving);
    GLdouble enemyTimeDiff = currentTime - changeActionActualTime;
    if(enemyTimeDiff >= 1 * 1000) { // Muda a cada 1 segundo
        map->ChangeEnemiesActions();
        changeActionActualTime = glutGet(GLUT_ELAPSED_TIME);
        map->RechargeEnemies();
    }

    // Checagem de colisão
    // Colisão dos inimigos com tiros do player
    map->CheckIfEnemyIsHit(playerShots);

    // Colisão do player com tiros dos inimigos
    if (map->CheckIfPlayerIsHit(player)) {
        gameOver = true;
        ResetKeyStatus();
        glutPostRedisplay();
        return;
    }

   glutPostRedisplay();
}

void mouseClick(int button, int state, int x, int y) {
    if(gameOver || gameWin) { return; }
    if(button == 0) {
        cursorPressStatus = state;
        if(!player->getIsJumping()) {
            player->StartJumping();
        }
    } else if(button == 2) {
        if(state == 1) {
            Shot* newShot = player->Shoot();
            if(newShot) {
                playerShots.push_back(newShot);
            }
        }
    }
}

void mouseMove(int x, int y) {
    if(gameOver || gameWin) { return; }
    y = (Height - y);
    GLfloat normalizedY = (float)y /(float) Height;
    GLfloat normalizedX = (float)x /(float) Width;
    // Transformações para o x e y do mouse coincidirem com o do mapa
    GLfloat xOff = player->GetgX() - (ViewingWidth / 2);
    GLfloat transformedX = (player->GetgX() - ViewingWidth/2 ) + ViewingWidth * normalizedX;
    GLfloat transformedY = ( ViewingHeight * normalizedY) + cameraY;
    player->MoveArmsAngle(transformedX, transformedY);

    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    if(argc != 2) {
        cout << "Por favor informe o nome do arquivo SVG" << endl;
        return 0;
    }
    cout << "Lendo arena: " << argv[1] << endl;
    filename = argv[1];
    readXMLFile(filename);

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
    glutMotionFunc(mouseMove);

    init();

    glutMainLoop();

   return 0;
}
