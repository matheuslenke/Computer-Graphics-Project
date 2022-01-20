#include "headers/enemy.h"

using namespace std;

void Enemy::MoveInX(GLdouble timeDiff, Map* map, Character* player) {
  GLfloat inc = this->speed * timeDiff;
  if(!CollidesDownWithAPlatform(map)) {
    this->isFacingRight = !this->isFacingRight;
    if (isFacingRight) {
      this->gX += inc;
    } else {
      this->gX -= inc;
    }
  }
  if(CollidesUpWithCharacter(player, inc) || CollidesDownWithCharacter(player, inc)) {
      Character::StartStanding();
      return;
  }
  if (isFacingRight) {
        if(CollidesRightWithCharacter(player, inc) == true ){
            Character::StartStanding();
            return;
        }
        if (
            CollidesRightWithAPlatform(inc, map) == true ) {
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
        if (CollidesLeftWithAPlatform(inc, map) == true) {
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

void Enemy::DoAction(GLfloat timeDiff, Map* map, Character* player) {

    switch(actingPattern[actualAction]) {
        case 0: // Andar
            this->MoveInX(timeDiff, map, player);
            break;
        case 1: // Atirar
            this->Shoot();
            break;
        case 2: // Ficar parado
            this->StartStanding();
            break;
    }
}

GLboolean Enemy::CollidesUpWithCharacter(Character* character, GLfloat inc) {
    if (character->CollidesWithPoint(this->gX - this->bodyWidth/2, this->gY + 0.6*totalHeight) == true) {
        return true;
    }
    if (character->CollidesWithPoint(this->gX + this->bodyWidth/2, this->gY + 0.6*totalHeight) == true) {
        return true;
    }
    return false;
}

GLboolean Enemy::CollidesDownWithCharacter(Character* character, GLfloat inc) {
    if (character->CollidesWithPoint(this->gX - this->bodyWidth/2, this->gY - 0.6*totalHeight) == true) {
        return true;
    }
    if (character->CollidesWithPoint(this->gX + this->bodyWidth/2, this->gY - 0.6*totalHeight) == true) {
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