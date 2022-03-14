#include "headers/enemy.h"

using namespace std;

void Enemy::MoveInX(GLdouble timeDiff, Map* map, Character* player) {
  GLfloat inc = this->speed * timeDiff;
  if(foundGround && isOnPlatform) {
    if(this->gX <= this->minWalkingX) {
        this->lookingAngle = 180;
        this->CalculateDirectionVector();
    } else if (this->gX >= this->maxWalkingX) {
        this->lookingAngle = 0;
        this->CalculateDirectionVector();
    }
  }

    this->gX += inc * this->directionVector.x;
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
        cout << "Achou chão!" << endl;
        foundGround = true;
        this->groundLimit = Character::GetCharacterGroundY();
        vec2* positionX = map->GetPlatformLimitsAtPoint(this->gX - this->bodyWidth/2, this->gY - 0.51*totalHeight - inc);
        if (positionX != nullptr) {
            this->minWalkingX = positionX->x;
            this->maxWalkingX = positionX->y;
            isOnPlatform = true;
        }
        isJumping = false;
        this->gY += inc;
        return;
    }
}

Shot* Enemy::DoAction(GLfloat timeDiff, Map* map, Character* player, GLboolean stopMoving) {
    Shot* aux = nullptr;
    int action = this->actingPattern[actualAction];
    this->MoveInY(timeDiff, map, player);
    if (action == 0 && !stopMoving) {
        this->MoveInX(timeDiff, map, player);
    } else if (action == 1) {
        aux = this->Shoot();
        if (aux) {
            vec3 shotPos = aux->GetPos();
        }

    } else if (action == 2) {
        this->StartStanding();
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
            if (character->CollidesWithPoint(actualX, this->gY + i*totalHeight, actualZ) == true
                // ||
                // map->CollidesWithEnemy(this->gX + this->bodyWidth/2 + inc, this->gY + i*totalHeight) == true
            ) {
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
            if (character->CollidesWithPoint(actualX, this->gY + i*totalHeight, actualZ) == true
                // ||
                // map->CollidesWithEnemy(this->gX + this->bodyWidth/2 + inc, this->gY + i*totalHeight) == true
            ) {
                return true;
            }
        }
    }
    return false;
}

void Enemy::AdjustArms(vec3 playerPosition) {
    int increment = 1;
    if(this->gZ < playerPosition.z) {
        this->MoveArmsAngle(0,0,increment);
    } else {  
        this->MoveArmsAngle(0,0,increment);
    }
    if (this->gY < playerPosition.y) {
        this->MoveArmsAngle(0, increment, 0);
    } else {
        this->MoveArmsAngle(0, increment, 0);
    }
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