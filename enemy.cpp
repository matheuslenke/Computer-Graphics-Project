#include "headers/enemy.h"

using namespace std;

void Enemy::MoveInX(GLdouble timeDiff, Map* map) {
  GLfloat inc = this->speed * timeDiff;
  if(!CollidesDownWithAPlatform(map)) {
    this->isFacingRight = !this->isFacingRight;
    if (isFacingRight) {
      this->gX += inc;
    } else {
      this->gX -= inc;
    }
  }
  if (isFacingRight) {
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
        GLfloat inc = this->speed * timeDiff;
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

void Enemy::DoAction(GLfloat timeDiff, Map* map) {
    switch(actingPattern[actualAction]) {
        case 0: // Andar
            this->MoveInX(timeDiff, map);
            break;
        case 1: // Atirar
            // cout << "Atirando" << endl;
            break;
        case 2: // Ficar parado
            this->StartStanding();
            break;
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