#include "headers/enemy.h"

using namespace std;

void Enemy::MoveInX(GLdouble timeDiff, Map* map, Character* player) {
  GLfloat inc = this->speed * timeDiff;
  if(foundGround && isOnPlatform) {
    if(this->gX <= this->minWalkingX) {
        this->isFacingRight = true;
    } else if (this->gX >= this->maxWalkingX) {
        this->isFacingRight = false;
    }
  }
  if(CollidesUpWithCharacter(player, inc) || CollidesDownWithCharacter(player, inc)) {
      Character::StartStanding();
      return;
  }
  if (CollidesRightWithAPlatform(map)) {
      this->isFacingRight = false;
  } else if (CollidesLeftWithAPlatform(map)) {
      this->isFacingRight = true;
  }
  if (isFacingRight) {
        if(CollidesRightWithCharacter(player, inc) == true ){
            Character::StartStanding();
            return;
        }
        if (
            CollidesRightWithAPlatform(map) == true ) {
            this->isFacingRight = false;
            Character::StartMoving(false);
            return;
        }
        this->gX += inc;
        if (this->isJumping) { return; }
        leg1Theta2 = -10;
        leg2Theta2 = -10;

        if(this->leg1Theta1 > 30) { da = -legMovingSpeed; }
        if(this->leg1Theta1 < -30) { da = legMovingSpeed;}

        if(this->leg2Theta1 > 30) { da2 = -legMovingSpeed; }
        if(this->leg2Theta1 < -30) { da2 = legMovingSpeed; }

        leg1Theta1 += da;
        leg2Theta1 += da2;

    } else {
        if(CollidesLeftWithCharacter(player, inc) == true ){
            Character::StartStanding();
            return;
        }
        if (CollidesLeftWithAPlatform(map) == true) {
            this->isFacingRight = true;
            Character::StartMoving(true);
            return;
            return;
        }
        this->gX -= inc;
        if (this->isJumping) { return; }
        leg1Theta2 = 10;
        leg2Theta2 = 10;

        if(this->leg1Theta1 > 30) { da = -legMovingSpeed; }
        if(this->leg1Theta1 < -30) { da = legMovingSpeed;}

        if(this->leg2Theta1 > 30) { da2 = -legMovingSpeed; }
        if(this->leg2Theta1 < -30) { da2 = legMovingSpeed; }

        leg1Theta1 += da;
        leg2Theta1 += da2;
    }
}

void Enemy::MoveInY(GLdouble timeDiff, Map* map, Character* player) {
    if (foundGround) {
        return;
    }
    GLfloat inc = this->jumpingSpeed * timeDiff;

    if (CollidesDownWithAPlatform(map)) {
        foundGround = true;
        this->groundLimit = Character::GetCharacterGroundY();
        vec2* positionX = map->GetPlatformLimitsAtPoint(this->gX - this->bodyWidth/2, this->gY - 0.5*totalHeight - inc);
        if (positionX != nullptr) {
            this->minWalkingX = positionX->x;
            this->maxWalkingX = positionX->y;
            isOnPlatform = true;
        }
        isJumping = false;
        return;
    } else {
        this->gY -= inc;
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
            vec2 shotPos = aux->GetPos();
        }

    } else if (action == 2) {
        this->StartStanding();
    }

    return aux;
}

GLboolean Enemy::CollidesUpWithCharacter(Character* character, GLfloat inc) {
    if (character->CollidesWithPoint(this->gX - this->bodyWidth/2, this->gY + 0.5*totalHeight + inc) == true) {
        return true;
    }
    if (character->CollidesWithPoint(this->gX + this->bodyWidth/2, this->gY + 0.5*totalHeight + inc) == true) {
        return true;
    }
    return false;
}

GLboolean Enemy::CollidesDownWithCharacter(Character* character, GLfloat inc) {
    if (character->CollidesWithPoint(this->gX - this->bodyWidth/2, this->gY - 0.5*totalHeight - inc) == true) {
        return true;
    }
    if (character->CollidesWithPoint(this->gX + this->bodyWidth/2, this->gY - 0.5*totalHeight - inc) == true) {
        return true;
    }
    return false;
}

GLboolean Enemy::CollidesRightWithCharacter(Character* character, GLfloat inc) {
    for(double i = -0.49; i<= 0.41; i+=0.1) { 
        if (character->CollidesWithPoint(this->gX + this->bodyWidth/2 + inc, this->gY +i *totalHeight) == true) {
            return true;
        }
    }
    return false;
}

GLboolean Enemy::CollidesLeftWithCharacter(Character* character, GLfloat inc) {
    for(double i = -0.49; i<= 0.41; i+=0.1) { 
        if (character->CollidesWithPoint(this->gX - this->bodyWidth/2 - inc, this->gY +i *totalHeight) == true) {
            return true;
        }
    }
    return false;
}

void Enemy::AdjustArms(vec2 playerPosition) {
    this->MoveArmsAngle(playerPosition.x, playerPosition.y);
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