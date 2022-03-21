#include "headers/map.h"
#include "headers/enemy.h"
#include <math.h>
#include <iostream>

using namespace std;

void Map::DrawMap(GLfloat x, GLfloat y, GLfloat z)
{
    // DrawOutline();
    
    for (Platform platform : this->platforms) {
        platform.Draw(platformsTexture);    
    }
    DrawEnemies();
}

void Map::DrawOutline() {
    glPushMatrix();

    glTranslatef(gX, gY, 0);

    glColor3f (0.6, 0.6, 0.6);
    int slicesX = 16;
    int slicesY = 32;

    glBindTexture(GL_TEXTURE_2D, floorTexture);

    float texSize = 32;

    float textureX = sizeX/texSize;
    float textureY = sizeY/texSize;

    glNormal3f(0,1,0);

    for (int i = 0; i < slicesX; i++){
        for (int j = 0; j < slicesY; j++){
            glBegin(GL_QUADS);
                glTexCoord2f ((textureX/slicesX)*i,(textureY/slicesY)*j);
                glVertex3f ((this->sizeX/slicesX)*i, 0, (this->sizeZ/slicesY)*j);        
                glTexCoord2f ((textureX/slicesX)*(i+1),(textureY/slicesY)*j);
                glVertex3f ((this->sizeX/slicesX)*(i+1), 0, (this->sizeZ/slicesY)*j);
                glTexCoord2f ((textureX/slicesX)*(i+1), (textureY/slicesY)*(j+1));
                glVertex3f ((this->sizeX/slicesX)*(i+1) , 0, (this->sizeZ/slicesY)*(j+1));
                glTexCoord2f ((textureX/slicesX)*i, (textureY/slicesY)*(j+1));
                glVertex3f ((this->sizeX/slicesX)*i, 0, (this->sizeZ/slicesY)*(j+1));
            glEnd();
        }
    }

    glPopMatrix();

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
    for(Shot* shot : enemyShots) {
        shot->Draw();
    }
}

bool Map::ColidesWithAPlatform(GLfloat x, GLfloat y, GLfloat z) {
    bool collides = false;
    // Colisão com os limites horizontais do mapa
    if(x <= this->gX || x >= (this->gX + this->sizeX)) {
        return true;
    }
    // Colisão com os limites verticais do mapa
    if (y <= this->gY || y >= (this->gY + this->sizeY)) {
        return true;
    }
    // Colisão com os limites em Z do mapa
    if (z <= this->gZ + 1 || z >= (this->gZ + this->sizeZ)) {
        // cout << "Colidiu em Z: " << z << " , " << this->gZ << endl;
        return true;
    }
    // Colisão com alguma plataforma
    for (Platform platform : this->platforms) {
        collides = platform.isColiding(x, y, z);
        if (collides == true) { return true; }
    }

    return collides;
}

vec2 *Map::GetPlatformLimitsAtPoint(GLfloat x, GLfloat y) {
    // Colisão com alguma plataforma
    for (Platform platform : this->platforms) {
        if(platform.isColiding(x, y, 0)) {
            return platform.ReturnPlatformXLimits();
        }
    }
    return nullptr;
}

bool Map::CollidesWithEnemy(GLfloat x, GLfloat y, GLfloat z) {
    for (Enemy* enemy: this->enemies) {
        if(x >= enemy->GetgX() - enemy->GetBodyWidth()/2 && x <= enemy->GetgX() + enemy->GetBodyWidth()/2 ) {
            if(y >= enemy->GetgY() - 0.5* enemy->GetTotalHeight() && y <= enemy->GetgY() + 0.5* enemy->GetTotalHeight()) {
                if(z >= enemy->GetgZ() - enemy->GetBodyWidth()/2 && z <= enemy->GetgZ() + enemy->GetBodyWidth()/2) {
                    return true;
                    break;
                }
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

void Map::ExecuteEnemiesActions(GLdouble timeDifference, Character* player, EnemyStatus enemyStatus) {
    vec3 playerPosition = player->getPosition();
    for (Enemy* enemy : this->enemies) {
        Shot* aux;
        enemy->AdjustArms(vec3(playerPosition.x, playerPosition.y, playerPosition.z));
        aux = enemy->DoAction(timeDifference, this, player, enemyStatus);
        if (aux != nullptr) {
            enemyShots.push_back(aux);
        }
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

    for(vector<Shot*>::iterator index = enemyShots.begin(); index != enemyShots.end(); ) {
        Shot* shot = *index;
        if (shot) {
            bool isShotValid = shot->Valid(this);
            if(!isShotValid) {
                enemyShots.erase(index);
                delete shot;
            } else {
                index++;
                shot->Move(timeDifference);
            }
        }
    }
}


// <--- Colisões --->
void Map::CheckIfEnemyIsHit(vector<Shot*> playerShots) {
    for(Shot* shot : playerShots) {
        for(vector<Enemy*>::iterator index = enemies.begin(); index != enemies.end();) {
            Enemy* enemy = *index;
            vec3 shotPos = shot->GetPos();
            GLfloat shotRadius = shot->GetRadius();
            if(enemy->CollidesWithPoint(shotPos.x, shotPos.y, shotPos.z)) {
                shot->SetHitted();
                this->enemies.erase(index);
            } else {
                index++;
            }
        }
    }
}

GLboolean Map::CheckIfPlayerIsHit(Character* player) {
    for(vector<Shot*>::iterator index = this->enemyShots.begin(); index != this->enemyShots.end();) {   
        Shot* shot = *index;
        vec3 shotPos = shot->GetPos();
        GLfloat shotRadius = shot->GetRadius();
        vec3 playerPosition = player->getPosition();
        if(player->CollidesWithPoint(shotPos.x, shotPos.y, shotPos.z) == true) {
            shot->SetHitted();
            return true;
        } else {
            index++;
        }
    }
    
    return false;
}

// <--- Getters e Setters --->
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

Map::~Map() {
    for (Enemy *enemy : this->enemies) {
        delete enemy;
    }
    for (Shot* shot: this->enemyShots) {
        delete shot;
    }
}

void Map::SetTextures(GLuint floorTexture, GLuint platformsTexture, GLuint enemiesTexture){
    this->floorTexture = floorTexture;
    this->platformsTexture = platformsTexture;
    for (Enemy* enemy : this->enemies) {
        enemy->SetTexture(enemiesTexture);
    }
}
