#include "headers/map.h"
#include "headers/enemy.h"
#include <math.h>
#include <iostream>

using namespace std;

void Map::DrawMap(GLfloat x, GLfloat y)
{
    glPushMatrix();
    glTranslatef(x, y, 0);

    glPushMatrix();
    DrawOutline(); // Desenha contorno dos limites do mapa
    glPopMatrix();

    glPopMatrix();
    for (Platform platform : this->platforms) {
        platform.Draw();    
    }

    DrawEnemies();
}

void Map::DrawOutline() {
    glColor3f (0.1, 0.1, 0.7);

    glBegin(GL_POLYGON);
        glVertex3f (0, 0, 0.0);
        glVertex3f (this->sizeX, 0, 0.0);
        glVertex3f (this->sizeX , this->sizeY, 0.0);
        glVertex3f (0, this->sizeY, 0.0);
        glVertex3f (0, 0, 0.0);
    glEnd();
}

void Map::DrawRectangle(GLfloat height, GLfloat width, GLfloat R, GLfloat G, GLfloat B)
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

void Map::DrawEnemies() {
    for (Enemy* enemy : this->enemies) {
        enemy->DrawEnemy();
    }
}

void Map::DrawShots() {
    for (Enemy* enemy : this->enemies) {
        vector<Shot*> shots = enemy->GetShots();
        for (Shot* shot : shots) {
            shot->Draw();
        }
    }
}

bool Map::ColidesWithAPlatform(GLfloat x, GLfloat y) {
    bool collides = false;
    // Colisão com os limites horizontais do mapa
    if(x <= this->gX || x >= (this->gX + this->sizeX)) {
        return true;
    }
    // Colisão com os limites verticais do mapa
    if (y <= this->gY || y >= (this->gY + this->sizeY)) {
        return true;
    }
    // Colisão com alguma plataforma
    for (Platform platform : this->platforms) {
        collides = platform.isColiding(x, y);
        if (collides == true) { break; }
        
    }
    return collides;
}

void Map::ExecuteEnemiesActions(GLdouble timeDifference, vec2 playerPosition) {
    Shot* aux;
    playerPosition = vec2(playerPosition.x, playerPosition.y);
    for (Enemy* enemy : this->enemies) {
        enemy->AdjustArms(playerPosition);
        enemy->DoAction(timeDifference, this);
    }
}

void Map::ChangeEnemiesActions() {
    for (Enemy* enemy : this->enemies) {
        enemy->NextAction();
    }
}

void Map::RechargeEnemies() {
    for (Enemy* enemy : this->enemies) {
        enemy->RechargeShot();
    }
}

void Map::MoveShots(GLdouble timeDifference) {
    for(Enemy* enemy: this->enemies) {
        vector<Shot*>& enemyShots = enemy->GetShots();
        for (vector<Shot*>::iterator index = enemyShots.begin(); index != enemyShots.end(); ) {
            Shot* shot = *index;
            if (shot) {
                bool isShotValid = shot->Valid(this);
                if (!isShotValid){ 
                    enemyShots.erase(index);
                    enemy->RechargeShot();
                } else {
                    index++;
                    shot->Move(timeDifference);
                }
            }
        }

    }
}

void Map::CheckIfEnemyIsHit(vector<Shot*> playerShots) {
    for(Shot* shot : playerShots) {
        for(vector<Enemy*>::iterator index = enemies.begin(); index != enemies.end();) {

        }
    }
}

void Map::AddPlatform(Platform p) {
    this->platforms.push_back(p);
}

void Map::AddEnemy(Enemy* e) {
    this->enemies.push_back(e);
}

GLfloat Map::GetgX() {
    return this->gX;
}

GLfloat Map::GetgY() {
    return this->gY;
}

GLfloat Map::GetSizeX() {
    return this->sizeX;
}

GLfloat Map::GetSizeY() {
    return this->sizeY;
}
