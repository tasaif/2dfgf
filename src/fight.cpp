#include "application.h"
#include "fight.h"

Fight::Fight(Player* _p, Player* _opponent){
  p = _p;
  c = p->getCharacter();
  opponent = _opponent;
  if (p->getPnum() == 0){
    p->position.x = -1 * Application::SCREEN_WIDTH / 4;
  } else {
    p->position.x = Application::SCREEN_WIDTH / 4;
  }
  p->position.y = 0;
  currentSprite = c->getBaseAnim(psNEUTRAL)->getSprite();
}

Fight::~Fight(){
}

unsigned abs_unsigned(int n){
  return n < 0 ? n * -1 : n;
}

int Fight::limitCheck(int dx){
  int max_distance = Application::SCREEN_WIDTH;
  int min_value = Stage::LEFT_MOST;
  int max_value = Stage::RIGHT_MOST;
  int newx = p->position.x + dx;
  int new_distance = abs_unsigned(p->position.x + dx - opponent->position.x);
  if (new_distance > max_distance){
    return 0;
  }
  if (newx > max_value){
    return max_value - p->position.x;
  }
  if (newx < min_value){
    return min_value - p->position.x;
  }
  return dx;
}

void Fight::run(){
  Animation* tmpAnim;
  away = p->position.x < opponent->position.x ? bLEFT : bRIGHT;
  towards = away == bLEFT ? bRIGHT : bLEFT;
  dir = p->joystick->getDirection();
  distance = abs_unsigned(opponent->position.x - p->position.x);

  switch(state){
    case psMATCHSTARTING:
      state = psNEUTRAL;
      break;
    case psNEUTRAL:
      currentSprite = c->getBaseAnim(psNEUTRAL)->getSprite();
      if (p->joystick->Pressed(bLP)){
        c->getBaseAnim(psWALKF)->animation_speed--;
        cout << c->getBaseAnim(psWALKF)->animation_speed << endl;
      } else if (p->joystick->Pressed(bMP)){
        c->getBaseAnim(psWALKF)->animation_speed++;
        cout << c->getBaseAnim(psWALKF)->animation_speed << endl;
      } else if (p->joystick->Pressed(bHP)){
        c->walkf_speed++;
        cout << c->walkf_speed;
      } else if (p->joystick->Pressed(bHK)){
        c->walkf_speed--;
        cout << c->walkf_speed;
      }
      switch(dir){
        case bUP:
          dy = dy_reset;
          state = psJUMPU;
          break;
        case bLEFT:
          dx = -1 * (away == bLEFT ? c->walkb_speed : c->walkf_speed);
          state = away == bLEFT ? psWALKB : psWALKF;
          currentSprite = c->getBaseAnim(state)->primeSprite();
          break;
        case bRIGHT:
          dx = (away == bRIGHT ? c->walkb_speed : c->walkf_speed);
          state = away == bRIGHT ? psWALKB : psWALKF;
          currentSprite = c->getBaseAnim(state)->primeSprite();
          break;
        case bUPRIGHT:
          dy = dy_reset;
          dx = (away == bRIGHT ? c->jumpb_speed : c->jumpf_speed);
          state = away == bRIGHT ? psJUMPB : psJUMPF;
          break;
        case bUPLEFT:
          dy = dy_reset;
          dx = -1 * (away == bLEFT ? c->jumpb_speed : c->jumpf_speed);
          state = away == bLEFT ? psJUMPB : psJUMPF;
          break;
        case bDOWNLEFT:
        case bDOWNRIGHT:
        case bDOWN:
          state = psPRECROUCH;
          c->getBaseAnim(psPRECROUCH)->reset();
          break;
        default:
          break;
      };
      break;
    case psWALKB:
      currentSprite = c->getBaseAnim(psWALKB)->getSprite();
      p->position.x += limitCheck(dx);
      if (dir != away){
        state = psNEUTRAL;
        currentSprite = c->getBaseAnim(psNEUTRAL)->primeSprite();
      }
      break;
    case psWALKF:
      currentSprite = c->getBaseAnim(psWALKF)->getSprite();
      p->position.x += limitCheck(dx);
      if (dir != towards){
        state = psNEUTRAL;
        currentSprite = c->getBaseAnim(psNEUTRAL)->primeSprite();
      }
      break;
    case psJUMPU:
      dy += c->jump_acc;
      p->position.y += dy;
      if (p->position.y <= 0){
        p->position.y = 0;
        state = psNEUTRAL;
        currentSprite = c->getBaseAnim(psNEUTRAL)->primeSprite();
      }
      break;
    case psJUMPF:
      p->position.x += limitCheck(dx);
      dy += c->jump_acc;
      p->position.y += dy;
      if (p->position.y <= 0){
        p->position.y = 0;
        state = psNEUTRAL;
        currentSprite = c->getBaseAnim(psNEUTRAL)->primeSprite();
      }
      break;
    case psJUMPB:
      p->position.x += limitCheck(dx);
      dy += c->jump_acc;
      p->position.y += dy;
      if (p->position.y <= 0){
        p->position.y = 0;
        state = psNEUTRAL;
        currentSprite = c->getBaseAnim(psNEUTRAL)->primeSprite();
      }
      break;
    case psPRECROUCH:
      tmpAnim = c->getBaseAnim(psPRECROUCH);
      currentSprite = tmpAnim->getSprite();
      if (dir != bDOWN && dir != bDOWNLEFT && dir != bDOWNRIGHT){
        state = psPOSTCROUCH;
        currentSprite = c->getBaseAnim(psPOSTCROUCH)->primeSprite();
      } else if (tmpAnim->loopComplete()){
        state = psCROUCH;
        currentSprite = c->getBaseAnim(psCROUCH)->primeSprite();
      }
      break;
    case psCROUCH:
      currentSprite = c->getBaseAnim(psCROUCH)->getSprite();
      if (dir != bDOWN && dir != bDOWNLEFT && dir != bDOWNRIGHT){
        state = psPOSTCROUCH;
        currentSprite = c->getBaseAnim(psPOSTCROUCH)->primeSprite();
      }
      break;
    case psPOSTCROUCH:
      tmpAnim = c->getBaseAnim(psPOSTCROUCH);
      currentSprite = tmpAnim->getSprite();
      if (tmpAnim->loopComplete()){
        state = psNEUTRAL;
        currentSprite = c->getBaseAnim(psNEUTRAL)->primeSprite();
      }
      break;
    default:
      break;
  };
}

Sprite* Fight::getSprite(){
  return currentSprite;
}

