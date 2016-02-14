/******************************************************************************
   Filename: Character.h

     Author: David C. Drake (http://davidcdrake.com)

Description: Declaration of a 'Character' class responsible for representing
             both player and non-player characters.
******************************************************************************/

#ifndef CHARACTER_H_
#define CHARACTER_H_

#include <string>
#include "main.h"
#include "Quest.h"

using namespace std;

const double DEFAULT_Z_OFFSET = 0.001;
const double GRAVITY = 0.004;

class Quest;

const enum CharacterType {
  PLAYER_BARBARIAN,
  PLAYER_DWARF,
  PLAYER_ELF,
  PLAYER_WIZARD,
  GOBLIN,
  ORC,
  NUM_CHARACTER_TYPES
};

class Character {
 public:
  Character(int type, Quest *quest);
  ~Character();
  void initialize(int type, Quest *quest);
  int getType() const;
  double getX() const;
  double getY() const;
  double getZ() const;
  double getHeight() const;
  double getRotation() const;
  double getRotationRate() const;
  double getmovementRate() const;
  double getjumpRate() const;
  double getRed() const;
  double getGreen() const;
  double getBlue() const;
  double getNextX() const;
  double getNextY() const;
  double getNextZ() const;
  void act(Character *player);
  void moveForward();
  void moveBackward();
  void moveToward(Character *character);
  void strafeLeft();
  void strafeRight();
  void jump();
  void fall();
  void rotateLeft();
  void rotateRight();
  bool isPlayer() const;
  bool isOnGround();
  bool canSee(Character *character);
  void draw();
 protected:
  string name_;
  int type_,
    bodyPoints_,
    mindPoints_,
    attackDice_,
    defendDice_;
  double red_,
    green_,
    blue_,
    x_,
    y_,
    z_,
    height_,
    rotation_,
    rotationRate_,
    movementRate_,
    jumpRate_,
    vertAcceleration_,
    collisionRadius_;
  Quest *quest_;
};

#endif  // CHARACTER_H_
