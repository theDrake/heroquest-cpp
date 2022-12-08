/*******************************************************************************
   Filename: character.h

     Author: David C. Drake (https://davidcdrake.com)

Description: Declaration of a 'Character' class responsible for representing
             both player and non-player characters.
*******************************************************************************/

#ifndef CHARACTER_H_
#define CHARACTER_H_

#include <string>
#include "main.h"
#include "quest.h"

using namespace std;

const double DEFAULT_Z_OFFSET = 0.001;
const double GRAVITY = 0.004;

class Quest;

enum CharacterType {
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
  int getType() const { return type_; }
  double getX() const { return x_; }
  double getY() const { return y_; }
  double getZ() const { return z_; }
  double getHeight() const { return height_; }
  double getRotation() const { return rotation_; }
  double getRotationRate() const { return rotationRate_; }
  double getmovementRate() const { return movementRate_; }
  double getjumpRate() const { return jumpRate_; }
  double getRed() const { return red_; }
  double getGreen() const { return green_; }
  double getBlue() const { return blue_; }
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
