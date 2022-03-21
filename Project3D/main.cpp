#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <list>
#include <vector>

#include "imageloader.h"
#include "headers/character.h"
#include "headers/enemy.h"
#include "headers/platform.h"
#include "headers/map.h"
#include "headers/camera.h"
#include "headers/shot.h"
#include "tinyxml2.h"


using namespace std;
using namespace tinyxml2;

#define SPACEBAR 32
// #define SHOOTING_AND_MOVING 0
// #define SHOOTING 1
// #define MOVING 2
// #define STOP 3

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
int lastAimY = 0;
int lastAimX = 0;
GLboolean dark = false;

// Parametros de ação dos inimigos
GLfloat changeActionActualTime = 0;
const GLfloat timeToChangeAction = 2; // Tempo em segundos
const GLint playerAmmo = 100;
const GLint enemyAmmo = 1;

EnemyStatus enemyStatus = SHOOTING_AND_MOVING;

// Impressao na tela
void * font = GLUT_BITMAP_9_BY_15;

//Identificadores de textura
GLuint texturePlane;


//Camera controls
double camDist=50;
double camXYAngle=0;
double camXZAngle=0;
int toggleCam = 2;
int camAngle = 60;
int lastX = 0;
int lastY = 0;
int leftButtonDown=0;
int lastCamera = 0;

GLuint LoadTextureRAW( const char * filename );

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
            Platform p = Platform( aux->DoubleAttribute("x"), - aux->FloatAttribute("y") + (map->GetSizeY() + 2 * map->GetgY()), aux->DoubleAttribute("width"), aux->DoubleAttribute("height"), map->GetSizeZ());
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
    mapDocument.Clear();
}

void RasterChars(GLfloat x, GLfloat y, GLfloat z, const char * text, double r, double g, double b)
{
    //Push to recover original attributes
    glPushAttrib(GL_ENABLE_BIT);
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        //Draw text in the x, y, z position
        glColor3f(r,g,b);
        glRasterPos3f(x, y, z);
        const char* tmpStr;
        tmpStr = text;
        while( *tmpStr ){
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *tmpStr);
            tmpStr++;
        }
    glPopAttrib();
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
void PrintText(GLfloat x, GLfloat y, const char * text, double r, double g, double b)
{
    //Draw text considering a 2D space (disable all 3d features)
    glMatrixMode (GL_PROJECTION);
    //Push to recover original PROJECTION MATRIX
    glPushMatrix();
        glLoadIdentity ();
        glOrtho (0, 1, 0, 1, -1, 1);
        RasterChars(x, y, 0, text, r, g, b);    
    glPopMatrix();
    glMatrixMode (GL_MODELVIEW);
}
void ResetKeyStatus()
{
    int i;
    //Initialize keyStatus
    for(i = 0; i < 256; i++)
       keyStatus[i] = 0; 
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

void DrawAxes()
{
    GLfloat color_r[] = { 1.0, 0.0, 0.0, 1.0 };
    GLfloat color_g[] = { 0.0, 1.0, 0.0, 1.0 };
    GLfloat color_b[] = { 0.0, 0.0, 1.0, 1.0 };

    glPushAttrib(GL_ENABLE_BIT);
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
 
        //x axis
        glPushMatrix();
            glColor3fv(color_r);
            glScalef(5, 0.3, 0.3);
            glTranslatef(0.5, 0, 0); // put in one end
            glutSolidCube(1.0);
        glPopMatrix();

        //y axis
        glPushMatrix();
            glColor3fv(color_g);
            glRotatef(90,0,0,1);
            glScalef(5, 0.3, 0.3);
            glTranslatef(0.5, 0, 0); // put in one end
            glutSolidCube(1.0);
        glPopMatrix();

        //z axis
        glPushMatrix();
            glColor3fv(color_b);
            glRotatef(-90,0,1,0);
            glScalef(5, 0.3, 0.3);
            glTranslatef(0.5, 0, 0); // put in one end
            glutSolidCube(1.0);
        glPopMatrix();
    glPopAttrib();
    
}

void crossProduct(
        double uX, double uY, double uZ, 
        double vX, double vY, double vZ, 
        double &oX, double &oY, double &oZ)
{
    oX = uY*vZ - uZ*vY;
    oY = uZ*vX - uX*vZ;
    oZ = uX*vY - uY*vX;
    double norm = sqrt(oX*oX+oY*oY+oZ*oZ);
    oX /= norm;
    oY /= norm;
    oZ /= norm;
}

//Funcao auxiliar para normalizar um vetor a/|a|
void normalize(float a[3])
{
    double norm = sqrt(a[0]*a[0]+a[1]*a[1]+a[2]*a[2]); 
    a[0] /= norm;
    a[1] /= norm;
    a[2] /= norm;
}

void changeCamera(int angle, int w, int h, double znear = 1)
{
    glMatrixMode (GL_PROJECTION);

    glLoadIdentity();

    gluPerspective (angle, 
            (GLfloat)w / (GLfloat)h, znear, map->GetSizeX());

    glMatrixMode (GL_MODELVIEW);
}

void reshape (int w, int h) {

    ViewingWidth = w;
    ViewingHeight = h;
    glViewport (0, 0, (GLsizei)w, (GLsizei)h);

    changeCamera(camAngle, w, h);
}

void eyeCamera() {
    vec3 direction = player->GetDirectionVector();
    double dir[3] = {player->GetgX() + direction.x, player->GetgY() + player->GetTotalHeight() * 0.45 + direction.y, player->GetgZ() - direction.z};
    PrintText(0.1, 0.1, "Eye Camera", 0,1,0);
    changeCamera(camAngle+15, ViewingWidth, ViewingHeight);
    gluLookAt(player->GetgX(), player->GetgY() + player->GetTotalHeight() * 0.45, player->GetgZ(),
    dir[0], dir[1], dir[2], 0, 1, 0);
}

void aimingCamera() {
    vec3 direction = player->GetDirectionVector();
    vec3 aimFinalPosition = player->GetFinalAimPosition();
    vec3 aimPosition = player->GetInitialAimPosition();
    // aimPosition = (aimFinalPosition+aimPosition)/2;
    PrintText(0.1, 0.1, "Aiming Camera", 0,1,0);
    changeCamera(camAngle-15, ViewingWidth, ViewingHeight, 0.1);
    gluLookAt(aimPosition.x, aimPosition.y, aimPosition.z,
    aimFinalPosition.x, aimFinalPosition.y, aimFinalPosition.z, 0, 1, 0);
}


//Funcao auxiliar para normalizar um vetor a/|a|
void normalizeVec(vec3 &a)
{
    double norm = sqrt(a.x*a.x+a.y*a.y+a.z*a.z); 
    a.x /= norm;
    a.y /= norm;
    a.z /= norm;
}

static const GLfloat light_diffuse[] = {1,1,1,1.5};
static const GLfloat light_ambient[] = {0.2,0.2,0.,1};
static const GLfloat light_defaut_attenuation[] = {1,1,1};
static const GLfloat light_specular[] = {1,1,1,1};


void drawLights() {
        GLfloat light_position[4] = { (GLfloat) (player->GetgX() - player->GetTotalHeight()), (GLfloat) (player->GetgY() + (GLfloat)1.5 * player->GetTotalHeight()), map->GetSizeZ(), 1.0 };
        if(!dark){
            glDisable(GL_LIGHT1);
            glLightfv(GL_LIGHT0, GL_POSITION, light_position);
            glEnable(GL_LIGHT0);
        } else {
            glDisable(GL_LIGHT0);

            vec3 flashlightPos = player->GetFinalShootPosition();
            vec3 flashlightDir = flashlightPos - player->GetInitialShootPosition();
            normalizeVec(flashlightDir);
            light_position[0] = (GLfloat) flashlightPos.x;
            light_position[1] = (GLfloat) flashlightPos.y;
            light_position[2] = (GLfloat) flashlightPos.z;
            
            GLfloat light_diretion[] = { (GLfloat) flashlightDir.x, (GLfloat) flashlightDir.y, (GLfloat) flashlightDir.z};

            glLightfv(GL_LIGHT1, GL_POSITION, light_position);
            glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light_diretion);

            glEnable(GL_LIGHT1);

        }

        cout << light_position[0] << ", " << light_position[1] << ", " <<  light_position[2] << endl;

        glDisable (GL_LIGHTING);
            glPushMatrix();
                glColor3f (0.0, 1.0, 1.0);
                glTranslatef(light_position[0], light_position[1], light_position[2]);
                glutWireCube (1);
            glPopMatrix();
        glEnable (GL_LIGHTING);
}

void DisplayPlane (GLuint texture)
{
    glPushAttrib(GL_LIGHTING_BIT);

    GLfloat materialEmission[] = { 1.0, 1.0, 1.0, 1};
    GLfloat materialColorA[] = { 0.2, 0.2, 0.2, 1};
    GLfloat materialColorD[] = { 1.0, 1.0, 1.0, 1};
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1};
    GLfloat mat_shininess[] = { 100.0 };
    glColor3f(1,1,1);

    glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialColorA);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColorD);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT  );//X
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );//Y

    glBindTexture(GL_TEXTURE_2D, texture);
    double textureS = 1; // Bigger than 1, repeat
    glBegin (GL_QUADS);
        glNormal3f(0,1,0);
        glTexCoord2f (0, 0);
        glVertex3f (-1, 0, -1);
        glNormal3f(0,1,0);
        glTexCoord2f (0, textureS);
        glVertex3f (-1, 0, +1);
        glNormal3f(0,1,0);
        glTexCoord2f (textureS, textureS);
        glVertex3f (+1, 0, +1);
        glNormal3f(0,1,0);
        glTexCoord2f (textureS, 0);
        glVertex3f (+1, 0, -1);
    glEnd();

    glPopAttrib();

}

void display (void) {
    glClearColor (0.2,0.2,0.2,1.0);
    // glClearColor (0.,0.,0.,1.0);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();


    glBindTexture(GL_TEXTURE_2D, 0);

    if(gameOver == true) {
        GLfloat xOff = player->GetgX() - (ViewingWidth / 2);
        PrintText(0.4, 0.5, "Game Over!", 0,1,0);
        PrintText(0.32, 0.4, "Press R to restart!!", 0,1,0);
    } else if (gameWin == true) {
        GLfloat xOff = player->GetgX() - (ViewingWidth / 2);
         PrintText(0.35, 0.55, "Congratulations!", 0,1,0);
         PrintText(0.30, 0.5, "Press r to play again!!", 0,1,0);
    } else {
        switch (enemyStatus) {
            case MOVING:
                PrintText(0.7, 0.1, "Anda", 0,1,0);
                break;
            case SHOOTING_AND_MOVING:
                PrintText(0.7, 0.1, "Atira e Anda", 0,1,0);
                break;
            case SHOOTING:
                PrintText(0.7, 0.1, "Atira", 0,1,0);
                break;
            case STOP:
                PrintText(0.7, 0.1, "Stop", 0,1,0);
                break;
        }
        if (toggleCam == 0){
            eyeCamera();
        } else if (toggleCam == 1){
            aimingCamera();
        } else if (toggleCam == 2){
            PrintText(0.1, 0.1, "Third Person Camera", 0,1,0);
            glTranslatef(0, 0, -camDist);
            glRotatef(camXZAngle,1,0,0);
            glRotatef(camXYAngle,0,1,0);
            glTranslatef(-player->GetgX(), -player->GetgY(), -player->GetgZ());
        }
        
        drawLights();

        map->Draw();

        for (Shot* shot : playerShots ) {
            shot->Draw();
        }
        map->DrawShots(); // Tiros dos inimigos

        player->Draw();
    }

    //  glPushMatrix();
    //     glScalef(70,70,1);
    //     glTranslatef(0,0,-10);
    //     glRotatef(90,1,0,0);
    //     DisplayPlane(texturePlane);
    // glPopMatrix();


    glutSwapBuffers();
}

void init (void) {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);

    glDepthFunc(GL_LEQUAL);   

    texturePlane = LoadTextureRAW( "stars1.bmp" );

    // Set light intensity and color for each component
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);

    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, (GLfloat) 60.0);
    glLightf(GL_LIGHT1,GL_SPOT_EXPONENT, 60);

    glBindTexture( GL_TEXTURE_2D, 0);
    // glEnable( GL_NORMALIZE );
    glEnable(GL_LIGHT0);
    // glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}


void mouse_callback(int button, int state, int x, int y) 
{
    if(gameOver || gameWin) { return; }
    if(button == GLUT_LEFT_BUTTON) {
        if(state == 1) {
            Shot* newShot = player->Shoot();
            if(newShot) {
                playerShots.push_back(newShot);
            }
        }
    }
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        // lastX = x;
        // lastY = y;
        leftButtonDown = 1;
        lastCamera = toggleCam;
        toggleCam = 1;
    } 
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
        leftButtonDown = 0;
        toggleCam = lastCamera;
    }
}

void mouse_motion(int x, int y)
{
    if(gameOver || gameWin) { return; }
    // Movimento do braço
    int incrementY = (y - lastAimY);
    int incrementX = (x - lastAimX);
    if(incrementY != 0) {
        player->MoveArmsAngle(0, incrementY, 0);
        lastAimY = y;
    }
    // TODO: MIRA EM AMBAS DIRECOES N TÁ 100% BOM AINDA
    // if (incrementX != 0) {
    //     player->MoveArmsAngle(incrementX, 0, 0);
    //     lastAimX = x;
    // }

    // Movimento da Camera
    if (!keyStatus[(int)('x')])
        return;

    GLdouble newCamXZAngle = camXZAngle + y - lastY;
    newCamXZAngle = (int) newCamXZAngle % 360;
    if(newCamXZAngle >= -60 && newCamXZAngle <= 60 ) {
        camXZAngle = newCamXZAngle;
    }
    camXYAngle += x - lastX;
    camXYAngle = (int)camXYAngle % 360;
    lastX = x;
    lastY = y;
    

    y = (Height - y);
}

void idle()
{

    GLdouble currentTime, timeDifference;
    static GLdouble previousTime =  glutGet(GLUT_ELAPSED_TIME);
    //Pega o tempo que passou do inicio da aplicacao
    currentTime = glutGet(GLUT_ELAPSED_TIME);
    // Calcula o tempo decorrido desde de a ultima frame.
    timeDifference = currentTime - previousTime;
    //Atualiza o tempo do ultimo frame ocorrido
    previousTime = currentTime;

    if (timeDifference > 16) {
        timeDifference = 16;
    }
    
    if (gameOver || gameWin) { return; }

    // <--- Movimento do Player --->
    if(keyStatus[(int)('s')])
    {
        player->MoveInXZ(false, timeDifference, map);
    }
    if(keyStatus[(int)('w')])
    {
        player->MoveInXZ(true, timeDifference, map);
    }
    if(keyStatus[(int)('d')])
    {
        player->TurnRight(timeDifference);
    }
    if(keyStatus[(int)('a')])
    {
        player->TurnLeft(timeDifference);
    }
    if (keyStatus[(int)('x')]) {

    }
    if (keyStatus[SPACEBAR]) {
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
    map->ExecuteEnemiesActions(timeDifference, player, enemyStatus);
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

void keyboard(unsigned char key, int x, int y)
{
    if(key == 27){
        exit(0);
    }
    if (gameOver || gameWin) { 
        if(key == 'R' || key == 'r') {
            restartGame();
        }
        return;
     }
    static bool textureEnabled = true;
    static bool lightingEnabled = true;
    static bool smoothEnabled = true;
    switch (key) {
        case '1':
            toggleCam = 0;
            break;
        case '2':
            toggleCam = 1;
            break;
        case '3':
            toggleCam = 2;
            break;
        case '7':
            if ( textureEnabled ){
                glDisable( GL_TEXTURE_2D );
            }else{
                glEnable( GL_TEXTURE_2D );
            }
            textureEnabled = !textureEnabled; 
            break;
        case '8':
            if ( lightingEnabled ){
                glDisable( GL_LIGHTING );
            }else{
                glEnable( GL_LIGHTING );
            }
            lightingEnabled = !lightingEnabled; 
            break;
        case '9':
            if ( smoothEnabled ){
                glShadeModel (GL_FLAT);
            }else{
                glShadeModel (GL_SMOOTH);
            }
            smoothEnabled = !smoothEnabled; 
            break;
        case '+':
        {
            int inc = camAngle >= 180 ? 0 : 1;
            camAngle += inc;
            changeCamera(camAngle, 
                    glutGet(GLUT_WINDOW_WIDTH), 
                    glutGet(GLUT_WINDOW_HEIGHT));
            break;
        }
        case '-':
        {
            int inc = camAngle <= 5 ? 0 : 1;
            camAngle -= inc;
            changeCamera(camAngle, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
            break;
        }
        case 's':
        case 'S':
             if (keyStatus[(int)('s')] == 0 && player->getIsDirectionToRight()) 
             {player->StartMoving(false);}
             keyStatus[(int)('s')] = 1; //Using keyStatus trick
             break;
        case 'w':
        case 'W':
             if (keyStatus[(int)('w')] == 0 && !player->getIsDirectionToRight()) 
             {player->StartMoving(true);}
             keyStatus[(int)('w')] = 1; //Using keyStatus trick
             break;
        case 'A':
        case 'a':
             keyStatus[(int)('a')] = 1; //Using keyStatus trick
             break;
        case 'D':
        case 'd':
             keyStatus[(int)('d')] = 1; //Using keyStatus trick
             break;
        case 'j':
            keyStatus[(int)('j')] = 1;
            break;  
        case 'k':
            keyStatus[(int)('k')] = 1;
            break;
        case 'l':
            dark = !dark;
            break;
        case 'i':
            keyStatus[(int)('i')] = 1;
            break;
        case 'x':
            keyStatus[(int)('x')] = 1;
            lastX = x;
            lastY = y;
            break;
        case 'p':
            enemyStatus = STOP;
            break;
        case '4':
            switch (enemyStatus) {
                case STOP:
                    enemyStatus = MOVING;
                    break;
                case MOVING:
                    enemyStatus = SHOOTING;
                    break;
                case SHOOTING:
                    enemyStatus = SHOOTING_AND_MOVING;
                    break;
                case SHOOTING_AND_MOVING:
                    enemyStatus = STOP;
                    break;
            }
            break;
        case '5':
            enemyStatus = MOVING;
            break;
        case '6':
            enemyStatus = SHOOTING_AND_MOVING;
            break;
        case SPACEBAR:
            keyStatus[SPACEBAR] = 1;
            if(!player->getIsJumping()) {
                player->StartJumping();
            }
            break;
    }
}

void keyUp(unsigned char key, int x, int y)
{
    if(gameOver || gameWin ) { return; }
    keyStatus[(int)(key)] = 0;
    glutPostRedisplay();
}

int main (int argc, char **argv) {
    if(argc != 2) {
        cout << "Por favor informe o nome do arquivo SVG" << endl;
        return 0;
    }
    cout << "Lendo arena: " << argv[1] << endl;
    filename = argv[1];
    readXMLFile(filename);
    glutInit (&argc, argv);

    glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize (500, 500);
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("Trabalho 2 - Matheus Lenke e Gabriel Cipriano");

    init();
    glutDisplayFunc (idle);
    glutIdleFunc (display);
    glutReshapeFunc (reshape);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyUp);
    glutMotionFunc(mouse_motion);
    glutPassiveMotionFunc(mouse_motion);
    glutMouseFunc(mouse_callback);

    glutMainLoop ();

    return 0;
}

GLuint LoadTextureRAW( const char * filename )
{

    GLuint texture;
    
    Image* image = loadBMP(filename);

    glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_2D, texture );
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE );
    // glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_REPLACE );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );
    glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
                             0,                            //0 for now
                             GL_RGB,                       //Format OpenGL uses for image
                             image->width, image->height,  //Width and height
                             0,                            //The border of the image
                             GL_RGB, //GL_RGB, because pixels are stored in RGB format
                             GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
                                               //as unsigned numbers
                             image->pixels);               //The actual pixel data
    delete image;

    return texture;
}

