#include "headers/enemy.h"

using namespace std;

void Enemy::MoveInX(GLdouble timeDiff, Map* map, Character* player) {
  GLfloat inc = this->speed * timeDiff;
  this->gX += inc * this->directionVector.x;
  if(foundGround && isOnPlatform) {
    if(this->gX <= this->minWalkingX) {
        this->gX -= inc * this->directionVector.x;
        this->lookingAngle += 180;
        if(this->lookingAngle >= 360) {
            this->lookingAngle = 0;
        }
        this->CalculateDirectionVector();
    } else if (this->gX >= this->maxWalkingX) {
        this->gX -= inc * this->directionVector.x;
        this->lookingAngle += 180;
        if(this->lookingAngle >= 360) {
            this->lookingAngle = 0;
        }
        this->CalculateDirectionVector();
        return;
    }
  }

    if(CollidesRightWithCharacter(player) == true ){
        this->gX -= inc * this->directionVector.x;
        Character::StartStanding();
        return;
    }
    else if (Enemy::CollidesHorizontallyWithAPlatform(map) == true ) {
        this->gX -= inc * this->directionVector.x;
        this->lookingAngle += 180;
        if(this->lookingAngle >= 360) {
            this->lookingAngle = 0;
        }
        this->CalculateDirectionVector();
        Character::StartMoving(false);
        return;
    }
    if (this->isJumping) { return; }
    Character::AnimateLegs();

    if(CollidesUpWithCharacter(player) || CollidesDownWithCharacter(player)) {
        Character::StartStanding();
        return;
    }
}

void Enemy::MoveInY(GLdouble timeDiff, Map* map, Character* player) {
    if (foundGround) {
        return;
    }
    GLfloat inc = this->jumpingSpeed * timeDiff;
    this->gY -= inc;

    if (CollidesDownWithAPlatform(map)) {
        foundGround = true;
        this->groundLimit = Character::GetCharacterGroundY() + inc;
        vec2* positionX = map->GetPlatformLimitsAtPoint(this->gX - this->bodyWidth/2, this->gY - 0.55*totalHeight - inc);
        if (positionX != nullptr) {
            this->minWalkingX = positionX->x;
            this->maxWalkingX = positionX->y;
            // cout << "Em plataforma com limites: " << minWalkingX << "," << maxWalkingX  << endl;
            isOnPlatform = true;
        }
        isJumping = false;
        return;
    }
}

Shot* Enemy::DoAction(GLfloat timeDiff, Map* map, Character* player, EnemyStatus enemyStatus) {
    Shot* aux = nullptr;
    int action = this->actingPattern[actualAction];
    this->MoveInY(timeDiff, map, player);
    if (action == 2 || enemyStatus == STOP) {
        this->StartStanding();
    }
    else if (action == 0 && (enemyStatus == SHOOTING_AND_MOVING || enemyStatus == MOVING)) {
        this->MoveInX(timeDiff, map, player);
    } else if (action == 1 && (enemyStatus == SHOOTING || enemyStatus == SHOOTING_AND_MOVING)) {
        if(canShoot){
            vec3 target = player->getPosition();
            aux = this->Shoot(&target);
        }
        if (aux) {
            vec3 shotPos = aux->GetPos();
        }
    }
    return aux;
}

GLboolean Enemy::CollidesUpWithCharacter(Character* character) {
    if (character->CollidesWithPoint(this->gX - this->bodyWidth/2, this->gY + 0.6*totalHeight, this->gZ) == true) {
        return true;
    }
    if (character->CollidesWithPoint(this->gX + this->bodyWidth/2, this->gY + 0.6*totalHeight, this->gZ) == true) {
        return true;
    }
    return false;
}

GLboolean Enemy::CollidesDownWithCharacter(Character* character) {
    if (character->CollidesWithPoint(this->gX - this->bodyWidth/2, this->gY - 0.6*totalHeight, this->gZ) == true) {
        return true;
    }
    if (character->CollidesWithPoint(this->gX + this->bodyWidth/2, this->gY - 0.6*totalHeight, this->gZ) == true) {
        return true;
    }
    return false;
}

GLboolean Enemy::CollidesHorizontallyWithAPlatform(Map* map) {
    int nPartitions = 72;
    for(double i = -0.48; i<= 0.41; i+=0.1) {
        for(double j = 0; j < nPartitions; j++) {
            float theta = (2.0f * M_PI) * float(j) / float(nPartitions);//get the current angle
            GLdouble actualX = this->gX + (this->bodyWidth/2) * cosf(theta);
            GLdouble actualZ = this->gZ + (this->bodyWidth/2) * sinf(theta);
            if (map->ColidesWithAPlatform(actualX, this->gY + i*totalHeight, actualZ) == true) {
                return true;
            }
        }
    }
    return false;
}

GLboolean Enemy::CollidesRightWithCharacter(Character* character) {
    int nPartitions = 72;
    for(double i = -0.48; i<= 0.41; i+=0.1) {
        for(double j = 0; j < nPartitions; j++) {
            float theta = (2.0f * M_PI) * float(j) / float(nPartitions);//get the current angle
            GLdouble actualX = this->gX + (this->bodyWidth/2) * cosf(theta);
            GLdouble actualZ = this->gZ + (this->bodyWidth/2) * sinf(theta);
            if (character->CollidesWithPoint(actualX, this->gY + i*totalHeight, actualZ) == true) {
                return true;
            }
        }
    }
    return false;
}

GLboolean Enemy::CollidesLeftWithCharacter(Character* character) {
    int nPartitions = 72;
    for(double i = -0.48; i<= 0.41; i+=0.1) {
        for(double j = 0; j < nPartitions; j++) {
            float theta = (2.0f * M_PI) * float(j) / float(nPartitions);//get the current angle
            GLdouble actualX = this->gX + (this->bodyWidth/2) * cosf(theta);
            GLdouble actualZ = this->gZ + (this->bodyWidth/2) * sinf(theta);
            if (character->CollidesWithPoint(actualX, this->gY + i*totalHeight, actualZ) == true) {
                return true;
            }
        }
    }
    return false;
}

void Enemy::AdjustArms(vec3 playerPosition) {
    GLdouble angleXZ = atan2(abs((this->gY-0.5*totalHeight)-(playerPosition.y)), abs(this->gX-playerPosition.x))*180/M_PI;
    GLdouble angleXY = atan2(this->gZ-(playerPosition.z + bodyWidth/2), this->gX-playerPosition.x)*180/M_PI;
    if(this->lookingAngle >= 0 && this->lookingAngle < 180 ) {
        if (angleXY > 0) {
            angleXY = angleXY - 180;
        } else {
            angleXY = angleXY + 180;
        }
        if (angleXY > -45 && angleXY < 45) {
            this->armThetaXY = -angleXY;
            canShoot = true;
        }else {canShoot = false;}
    } else {
        if (angleXY > -45 && angleXY < 45) {
            this->armThetaXY = -angleXY;
            canShoot = true;
        }else {canShoot = false;}
    }
        if (angleXZ > -45 && angleXZ < 45) {
            this->armThetaXZ = angleXZ;
            canShoot = canShoot && true;
        }else {canShoot = false;}
}

void Enemy::NextAction() {
    if (actualAction >= actingPattern.size()) {
        actualAction = 0;
    } else {
        actualAction++;
    }
}

void  Enemy::InsertAction(int action) {
    actingPattern.push_back(action);
}