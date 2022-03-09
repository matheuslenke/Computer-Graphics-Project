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

typedef struct
{
    //Vertex coordinate
    double X;
    double Y;
    double Z;
    
    //Vertex normal 
    double nX;
    double nY;
    double nZ;
    
    //Vertex texture coordinate
    double U;
    double V;
} VERTICES;

typedef struct
{
    VERTICES * vtx;
    int numVtx;
    double radius;
} OBJ;

OBJ* objEarth;
OBJ* objSun;

//Identificadores de textura
GLuint textureEarth;
GLuint textureSun;
GLuint texturePlane;

//Cotroles de giro
double angleDay = 0;
double angleYear = 0;

//Camera controls
double camDist=50;
double camXYAngle=0;
double camXZAngle=0;
int toggleCam = 2;
int camAngle = 60;
int lastX = 0;
int lastY = 0;
int buttonDown=0;

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
            // enemies.push_back(enemy);
        }
    }
    for (Enemy* enemy : enemies) {
        map->AddEnemy(enemy);
    }
    for(Platform p : platforms) {
        map->AddPlatform(p);
    }
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

OBJ * CreateSphere (double R, double space) 
{
    OBJ *obj = new OBJ;
    
    obj->numVtx = (180 / space) * 
                  (2 + 360 / (2*space)) * 4;
    obj->vtx = new VERTICES[ obj->numVtx ];
    obj->radius = R;

    int n;
    double vR, lVR;
    double hR, lHR;
    double norm;
    n = 0;
    for( vR = 0; vR <= 180-space; vR+=space){
        for(hR = 0; hR <= 360+2*space; hR+=2*space)
        {
            lVR = vR;
            lHR = hR;
            obj->vtx[n].X = R * 
                    sin(lHR / 180 * M_PI) * 
                    sin(lVR / 180 * M_PI);
            obj->vtx[n].Y = R * 
                    cos(lHR / 180 * M_PI) * 
                    sin(lVR / 180 * M_PI);
            obj->vtx[n].Z = R * 
                    cos(lVR / 180 * M_PI);
            obj->vtx[n].V = lVR / 180;
            obj->vtx[n].U = lHR / 360;
            norm = sqrt(
                    obj->vtx[n].X*obj->vtx[n].X+
                    obj->vtx[n].Y*obj->vtx[n].Y+
                    obj->vtx[n].Z*obj->vtx[n].Z);
            obj->vtx[n].nX = obj->vtx[n].X/norm;
            obj->vtx[n].nY = obj->vtx[n].Y/norm;
            obj->vtx[n].nZ = obj->vtx[n].Z/norm;
            n++;

            lVR = vR + space;
            lHR = hR;
            obj->vtx[n].X = R * sin(lHR / 180 * M_PI) * sin(lVR / 180 * M_PI);
            obj->vtx[n].Y = R * cos(lHR / 180 * M_PI) * sin(lVR / 180 * M_PI);
            obj->vtx[n].Z = R * cos(lVR / 180 * M_PI);
            obj->vtx[n].V = lVR / 180;
            obj->vtx[n].U = lHR / 360;
            norm = sqrt(obj->vtx[n].X*obj->vtx[n].X+obj->vtx[n].Y*obj->vtx[n].Y+obj->vtx[n].Z*obj->vtx[n].Z);
            obj->vtx[n].nX = obj->vtx[n].X/norm;
            obj->vtx[n].nY = obj->vtx[n].Y/norm;
            obj->vtx[n].nZ = obj->vtx[n].Z/norm;
            n++;

            lVR = vR;
            lHR = hR + space;
            obj->vtx[n].X = R * sin(lHR / 180 * M_PI) * sin(lVR / 180 * M_PI);
            obj->vtx[n].Y = R * cos(lHR / 180 * M_PI) * sin(lVR / 180 * M_PI);
            obj->vtx[n].Z = R * cos(lVR / 180 * M_PI);
            obj->vtx[n].V = lVR / 180;
            obj->vtx[n].U = lHR / 360;
            norm = sqrt(obj->vtx[n].X*obj->vtx[n].X+obj->vtx[n].Y*obj->vtx[n].Y+obj->vtx[n].Z*obj->vtx[n].Z);
            obj->vtx[n].nX = obj->vtx[n].X/norm;
            obj->vtx[n].nY = obj->vtx[n].Y/norm;
            obj->vtx[n].nZ = obj->vtx[n].Z/norm;
            n++;

            lVR = vR + space;
            lHR = hR + space;
            obj->vtx[n].X = R * sin(lHR / 180 * M_PI) * sin(lVR / 180 * M_PI);
            obj->vtx[n].Y = R * cos(lHR / 180 * M_PI) * sin(lVR / 180 * M_PI);
            obj->vtx[n].Z = R * cos(lVR / 180 * M_PI);
            obj->vtx[n].V = lVR / 180;
            obj->vtx[n].U = lHR / 360;
            norm = sqrt(obj->vtx[n].X*obj->vtx[n].X+obj->vtx[n].Y*obj->vtx[n].Y+obj->vtx[n].Z*obj->vtx[n].Z);
            obj->vtx[n].nX = obj->vtx[n].X/norm;
            obj->vtx[n].nY = obj->vtx[n].Y/norm;
            obj->vtx[n].nZ = obj->vtx[n].Z/norm;
            n++;
        }
    }
    return obj;
}

//Funcao auxiliar para normalizar um vetor a/|a|
void normalize(float a[3])
{
    double norm = sqrt(a[0]*a[0]+a[1]*a[1]+a[2]*a[2]); 
    a[0] /= norm;
    a[1] /= norm;
    a[2] /= norm;
}

void eyeCamera() {
    GLfloat increment = player->GetTotalHeight();
    // if (player->getIsDirectionToRight() == false) {
    //     increment = -increment;
    // } 
    double dir[3] = {player->GetgX() + increment, player->GetgY() + player->GetTotalHeight() * 0.45, player->GetgZ()};
    PrintText(0.1, 0.1, "Eye Camera", 0,1,0);
    gluLookAt(player->GetgX() + player->GetBodyWidth(), player->GetgY() + player->GetTotalHeight() * 0.45, player->GetgZ(),
    dir[0], dir[1], dir[2], 0, 1, 0);
}

void display (void) {
    glClearColor (0.2,0.2,0.2,1.0);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

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
        if (toggleCam == 0){
            eyeCamera();
        } else if (toggleCam == 1){
            PrintText(0.1, 0.1, "Static Camera at a Distance", 0,1,0);
            gluLookAt(map->GetgX(),map->GetgY(),50, player->GetgX(),player->GetgY(),0, 0,1,0);
        } else if (toggleCam == 2){
            PrintText(0.1, 0.1, "Third Person Camera", 0,1,0);
            glTranslatef(0, 0, -camDist);
            glRotatef(camXZAngle,1,0,0);
            glRotatef(camXYAngle,0,1,0);
            glTranslatef(-player->GetgX(), -player->GetgY(), -player->GetgZ());
        }

        GLfloat light_position[] = { player->GetgX() - player->GetTotalHeight(), player->GetgY() + (GLfloat)1.5 * player->GetTotalHeight(), map->GetSizeZ(), 1.0 };

        // cout << "Ligth position:" << light_position[0] << " , " << light_position[1]<<" , " << player->GetgY() << endl;
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
        glDisable (GL_LIGHTING);
        glColor3f (0.0, 1.0, 1.0);
        glPushMatrix();
        glTranslatef(light_position[0], light_position[1], light_position[2]);
        glutWireCube (0.1);
        glEnable (GL_LIGHTING);
        glPopMatrix();
        map->Draw();
        player->Draw();

        // for (Shot* shot : playerShots ) {
        //     shot->Draw();
        // }
        // map->DrawShots(); // Tiros dos inimigos
    }

    glutSwapBuffers();
}

void init (void) {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glShadeModel (GL_SMOOTH);

    glDepthFunc(GL_LEQUAL);
    glEnable(GL_LIGHT0);
}

void changeCamera(int angle, int w, int h)
{
    glMatrixMode (GL_PROJECTION);

    glLoadIdentity ();

    gluPerspective (angle, 
            (GLfloat)w / (GLfloat)h, 1, map->GetSizeX());

    glMatrixMode (GL_MODELVIEW);
}

void reshape (int w, int h) {

    glViewport (0, 0, (GLsizei)w, (GLsizei)h);

    changeCamera(camAngle, w, h);
}

void mouse_callback(int button, int state, int x, int y) 
{
    if(gameOver || gameWin) { return; }
    if(button == GLUT_LEFT_BUTTON) {

    } 
    if(button == GLUT_RIGHT_BUTTON) {
        if(state == 1) {
            Shot* newShot = player->Shoot();
            if(newShot) {
                playerShots.push_back(newShot);
            }
        }
    }
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        lastX = x;
        lastY = y;
        buttonDown = 1;
    } 
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
        buttonDown = 0;
    }
}

void mouse_motion(int x, int y)
{
    if (!buttonDown)
        return;
    
    camXYAngle += x - lastX;
    camXZAngle += y - lastY;
    
    camXYAngle = (int)camXYAngle % 360;
    camXZAngle = (int)camXZAngle % 360;
    
    lastX = x;
    lastY = y;

    if(gameOver || gameWin) { return; }
    y = (Height - y);
    GLfloat normalizedY = (float)y /(float) Height;
    GLfloat normalizedX = (float)x /(float) Width;
    // Transformações para o x e y do mouse coincidirem com o do mapa
    GLfloat xOff = player->GetgX() - (ViewingWidth / 2);
    GLfloat transformedX = (player->GetgX() - ViewingWidth/2 ) + ViewingWidth * normalizedX;
    GLfloat transformedY = ( ViewingHeight * normalizedY) + cameraY;
    player->MoveArmsAngle(transformedX, transformedY);
}

void idle()
{
    angleDay+=0.05;
    angleYear+=0.01;
    
    if (angleDay > 360) angleDay = 0;
    else if (angleDay < 0) angleDay = 360;

    if (angleYear > 360) angleYear = 0;
    else if (angleYear < 0) angleYear = 360;

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

void keyboard(unsigned char key, int x, int y)
{
    if (gameOver || gameWin) { 
        if(key == 'R' || key == 'r') {
            restartGame();
        }
        return;
     }
    static bool textureEnebled = true;
    static bool lightingEnebled = true;
    static bool smoothEnebled = true;
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
            if ( textureEnebled ){
                glDisable( GL_TEXTURE_2D );
            }else{
                glEnable( GL_TEXTURE_2D );
            }
            textureEnebled = !textureEnebled; 
            break;
        case '8':
            if ( lightingEnebled ){
                glDisable( GL_LIGHTING );
            }else{
                glEnable( GL_LIGHTING );
            }
            lightingEnebled = !lightingEnebled; 
            break;
        case '9':
            if ( smoothEnebled ){
                glShadeModel (GL_FLAT);
            }else{
                glShadeModel (GL_SMOOTH);
            }
            smoothEnebled = !smoothEnebled; 
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
        // case 'l':
        //     keyStatus[(int)('l')] = 1;
        //     break;
        case 'i':
            keyStatus[(int)('i')] = 1;
            break;
        case SPACEBAR:
            keyStatus[SPACEBAR] = 1;
            if(!player->getIsJumping()) {
                player->StartJumping();
            }
            break;
        case 27:
            exit(0);
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
//    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_REPLACE );
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

