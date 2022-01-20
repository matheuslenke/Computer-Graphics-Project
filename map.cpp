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
        if (collides == true) { return true; }
        
    }

    return collides;
}

bool Map::CollidesWithEnemy(GLfloat x, GLfloat y) {
    for (Enemy* enemy: this->enemies) {
        if(x >= enemy->GetgX() - enemy->GetBodyWidth()/2 && x <= enemy->GetgX() + enemy->GetBodyWidth()/2 ) {
            if(y >= enemy->GetgY() - 0.5* enemy->GetTotalHeight() && y <= enemy->GetgY() + 0.5* enemy->GetTotalHeight()) {
                return true;
                break;
            }
        }
    }
    return false;
}

bool Map::ColidesWithRightEnd(GLfloat x) {
    if(x >= this->GetgX() + this->GetSizeX()) {
        return true;
    }
    return false;
}

void Map::ExecuteEnemiesActions(GLdouble timeDifference, Character* player) {
    Shot* aux;
    vec3 playerPosition = player->getPosition();
    for (Enemy* enemy : this->enemies) {
        enemy->AdjustArms(vec2(playerPosition.x, playerPosition.y));
        enemy->DoAction(timeDifference, this, player);
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
            Enemy* enemy = *index;
            vec2 shotPos = shot->GetPos();
            GLfloat shotRadius = shot->GetRadius();
            GLboolean collides = false;
            int numSegments = 10;
            for(int j = 0; j <= numSegments ; j+=1) {
                float theta = 2.0f * 3.1415926f * float(j) / float(numSegments);//get the current angle
                float x = shotRadius * cosf(theta);//calculate the x component
                float y = shotRadius * sinf(theta);//calculate the y component
                if(enemy->CollidesWithPoint(shotPos.x + x, shotPos.y + y)) {
                    collides = true;
                    shot->SetHittedEnemy();
                    break;
                }
            }
            if(collides) {
                this->enemies.erase(index);
            } else {
                index++;
            }
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

GLboolean Map::CheckIfPlayerIsHit(Character* player) {
    for(Enemy* enemy : this->enemies) {
        vector<Shot*>& enemyShots = enemy->GetShots();
        for(Shot* shot : enemyShots) {    
            vec2 shotPos = shot->GetPos();
            GLfloat shotRadius = shot->GetRadius();
            GLboolean collides = false;
            int numSegments = 10;
            for(int j = 0; j <= numSegments ; j+=1) {
                float theta = 2.0f * 3.1415926f * float(j) / float(numSegments);//get the current angle
                float x = shotRadius * cosf(theta);//calculate the x component
                float y = shotRadius * sinf(theta);//calculate the y component
                if(player->CollidesWithPoint(shotPos.x + x, shotPos.y + y)) {
                    collides = true;
                    shot->SetHittedEnemy();
                    return collides;
                }
            }
            return collides;
        }
    }
    return false;
}