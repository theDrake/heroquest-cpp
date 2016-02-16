/******************************************************************************
   Filename: Character.cpp

     Author: David C. Drake (http://davidcdrake.com)

Description: Definition of a 'Character' class responsible for representing
             both player and non-player characters.
******************************************************************************/

#include "Character.h"

//-----------------------------------------------------------------------------
//      Method: Character
//
// Description: Constructs a Character object (via the 'initialize' method)
//              according to a given character type. Also provides the
//              character with a given pointer to a Quest object.
//
//      Inputs: type  - Integer representing the creature's type.
//              quest - Pointer to the current Quest object.
//
//     Outputs: None.
//-----------------------------------------------------------------------------
Character::Character(int type, Quest *quest) {
  initialize(type, quest);
}

//-----------------------------------------------------------------------------
//      Method: ~Character
//
// Description: Destructor.
//
//      Inputs: None.
//
//     Outputs: None.
//-----------------------------------------------------------------------------
Character::~Character() {}

//-----------------------------------------------------------------------------
//      Method: initialize
//
// Description: Initializes the Character object according to a given
//              character type. Also provides the character with a given
//              pointer to a Quest object.
//
//      Inputs: type  - Integer representing the creature's type.
//              quest - Pointer to the current Quest object.
//
//     Outputs: None.
//-----------------------------------------------------------------------------
void Character::initialize(int type, Quest *quest) {
  type_ = type;
  quest_ = quest;
  z_ = DEFAULT_Z_OFFSET;
  height_ = CELL_SIZE / 1.5;
  movementRate_ = 0.04;
  jumpRate_ = 0.05;
  vertAcceleration_ = 0.0;
  rotationRate_ = 2.0;
  collisionRadius_ = 0.25;

  if (isPlayer()) {
    red_ = 0.5;
    green_ = 0.0;
    blue_ = 0.0;
    x_ = quest_->getStartX() + CELL_SIZE / 2.0;
    y_ = CELL_SIZE / 2.0;
    rotation_ = 90.0;
  } else {  // NPC
    red_ = 0.0;
    green_ = 0.0;
    blue_ = 0.5;
    x_ = rand() % DEFAULT_MAZE_WIDTH + CELL_SIZE / 2.0;
    y_ = rand() % DEFAULT_MAZE_HEIGHT + CELL_SIZE / 2.0;
    rotation_ = rand() % 360;
  }
}

//-----------------------------------------------------------------------------
//      Method: getNextX
//
// Description: Returns the x-coordinate the character would have if it moved
//              forward one step.
//
//      Inputs: None.
//
//     Outputs: The character's next anticipated x-coordinate.
//-----------------------------------------------------------------------------
double Character::getNextX() const {
  return x_ + cos(rotation_ * PI / 180) * movementRate_;
}

//-----------------------------------------------------------------------------
//      Method: getNextY
//
// Description: Returns the y-coordinate the character would have if it moved
//              forward one step.
//
//      Inputs: None.
//
//     Outputs: The character's next anticipated y-coordinate.
//-----------------------------------------------------------------------------
double Character::getNextY() const {
  return y_ + sin(rotation_ * PI / 180) * movementRate_;
}

//-----------------------------------------------------------------------------
//      Method: getNextZ
//
// Description: Returns the z-coordinate the character would have if it moved
//              upward one step. (Not fully implemented right now. Currently,
//              this method returns the character's current z-coordinate.)
//
//      Inputs: None.
//
//     Outputs: The character's next anticipated z-coordinate. (Actually just
//              the current z-coordinate, for now.)
//-----------------------------------------------------------------------------
double Character::getNextZ() const {
  return z_;  // Temporary code.
}

//-----------------------------------------------------------------------------
//      Method: moveForward
//
// Description: Moves the character one step forward, if unobstructed.
//
//      Inputs: None.
//
//     Outputs: None.
//-----------------------------------------------------------------------------
void Character::moveForward() {
  double newX = getNextX();
  double newY = getNextY();

  if (quest_->isLegalPosition(newX, y_, collisionRadius_)) {
    x_ = newX;
  }
  if (quest_->isLegalPosition(x_, newY, collisionRadius_)) {
    y_ = newY;
  }
}

//-----------------------------------------------------------------------------
//      Method: moveBackward
//
// Description: Moves the character one step backward, if unobstructed.
//
//      Inputs: None.
//
//     Outputs: None.
//-----------------------------------------------------------------------------
void Character::moveBackward() {
  double newX = x_ - cos(rotation_ * PI / 180) * movementRate_;
  double newY = y_ - sin(rotation_ * PI / 180) * movementRate_;

  if (quest_->isLegalPosition(newX, y_, collisionRadius_)) {
    x_ = newX;
  }
  if (quest_->isLegalPosition(x_, newY, collisionRadius_)) {
    y_ = newY;
  }
}

//-----------------------------------------------------------------------------
//      Method: moveToward
//
// Description: Moves the character toward another given character.
//
//      Inputs: character - The target character.
//
//     Outputs: None.
//-----------------------------------------------------------------------------
void Character::moveToward(Character *character) {
  double opposite = character->getY() - y_;
  double adjacent = character->getX() - x_;
  rotation_ = atan(opposite / adjacent) * 180 / PI;
  if (x_ > character->getX()) {
    rotation_ += 180.0;
  }
  moveForward();
}

//-----------------------------------------------------------------------------
//      Method: strafeLeft
//
// Description: Moves the character one step to the left, if unobstructed.
//
//      Inputs: None.
//
//     Outputs: None.
//-----------------------------------------------------------------------------
void Character::strafeLeft() {
  double newX = x_ - sin(rotation_ * PI / 180) * movementRate_;
  double newY = y_ - cos(rotation_ * PI / 180) * movementRate_ * -1.0;

  if (quest_->isLegalPosition(newX, y_, collisionRadius_)) {
    x_ = newX;
  }
  if (quest_->isLegalPosition(x_, newY, collisionRadius_)) {
    y_ = newY;
  }
}

//-----------------------------------------------------------------------------
//      Method: strafeRight
//
// Description: Moves the character one step to the right, if unobstructed.
//
//      Inputs: None.
//
//     Outputs: None.
//-----------------------------------------------------------------------------
void Character::strafeRight() {
  double newX = x_ - sin(rotation_ * PI / 180) * movementRate_ * -1.0;
  double newY = y_ - cos(rotation_ * PI / 180) * movementRate_;

  if (quest_->isLegalPosition(newX, y_, collisionRadius_)) {
    x_ = newX;
  }
  if (quest_->isLegalPosition(x_, newY, collisionRadius_)) {
    y_ = newY;
  }
}

//-----------------------------------------------------------------------------
//      Method: jump
//
// Description: Causes the character to accelerate upwards, if currently on the
//              ground.
//
//      Inputs: None.
//
//     Outputs: None.
//-----------------------------------------------------------------------------
void Character::jump() {
  if (z_ <= DEFAULT_Z_OFFSET) {
    vertAcceleration_ += jumpRate_;
    z_ += vertAcceleration_;
  }
}

//-----------------------------------------------------------------------------
//      Method: fall
//
// Description: Applies gravity to the character's vertical acceleration, if
//              currently in the air. If not in the air, ensures all relevant
//              variables are set to their default values.
//
//      Inputs: None.
//
//     Outputs: None.
//-----------------------------------------------------------------------------
void Character::fall() {
  if (z_ > DEFAULT_Z_OFFSET) {
    vertAcceleration_ -= GRAVITY;
    z_ += vertAcceleration_;
  } else {
    z_ = DEFAULT_Z_OFFSET;
    vertAcceleration_ = 0.0;
  }
}

//-----------------------------------------------------------------------------
//      Method: rotateLeft
//
// Description: Rotates the character to the left.
//
//      Inputs: None.
//
//     Outputs: None.
//-----------------------------------------------------------------------------
void Character::rotateLeft() {
  rotation_ += rotationRate_;
}

//-----------------------------------------------------------------------------
//      Method: rotateRight
//
// Description: Rotates the character to the right.
//
//      Inputs: None.
//
//     Outputs: None.
//-----------------------------------------------------------------------------
void Character::rotateRight() {
  rotation_ -= rotationRate_;
}

//-----------------------------------------------------------------------------
//      Method: isPlayer
//
// Description: Determines whether the character is the player character.
//
//      Inputs: None.
//
//     Outputs: Returns 'true' if the character's type corresponds to a player
//              character type, 'false' otherwise.
//-----------------------------------------------------------------------------
bool Character::isPlayer() const {
  return type_ == PLAYER_BARBARIAN || type_ == PLAYER_WIZARD ||
         type_ == PLAYER_ELF || type_ == PLAYER_DWARF;
}

//-----------------------------------------------------------------------------
//      Method: isOnGround
//
// Description: Determines whether the character is touching the ground.
//
//      Inputs: None.
//
//     Outputs: Returns 'true' if the character's feet are touching the ground,
//              'false' otherwise.
//-----------------------------------------------------------------------------
bool Character::isOnGround() {
  if (z_ <= DEFAULT_Z_OFFSET) {
    z_ = DEFAULT_Z_OFFSET;
  }

  return z_ == DEFAULT_Z_OFFSET;
}

//-----------------------------------------------------------------------------
//      Method: canSee
//
// Description: Determines whether the character can see another given
//              character.
//
//      Inputs: character - The character of interest.
//
//     Outputs: Returns 'true' if the character can see the other given
//              character, 'false' otherwise.
//-----------------------------------------------------------------------------
bool Character::canSee(Character *character) {
  return false;
}

//-----------------------------------------------------------------------------
//      Method: act
//
// Description: Executes AI behavior for the character. (Simplified for now.)
//
//      Inputs: None.
//
//     Outputs: None.
//-----------------------------------------------------------------------------
void Character::act(Character *player) {
  if (isPlayer()) {
    return;
  } else if (canSee(player)) {
    moveToward(player);
  } else {
    if (quest_->isLegalPosition(getNextX(), y_, collisionRadius_) &&
        quest_->isLegalPosition(x_, getNextY(), collisionRadius_)) {
      moveForward();
    } else {
      rotateRight();
    }
  }
}

//-----------------------------------------------------------------------------
//      Method: draw
//
// Description: Draws the character.
//
//      Inputs: None.
//
//     Outputs: None.
//-----------------------------------------------------------------------------
void Character::draw() {
  // Draw the character's body:
  glColor3d(red_, green_, blue_);
  glPushMatrix();
  glTranslated(x_, y_, z_);
  glRotated(rotation_, 0.0, 0.0, 1.0);

  glBegin(GL_QUADS);
  glVertex3d(0.0, collisionRadius_, 0.0);
  glVertex3d(0.0, collisionRadius_, height_);
  glVertex3d(0.0, -collisionRadius_, height_);
  glVertex3d(0.0, -collisionRadius_, 0.0);
  glEnd();

  /*// Draw the character's limbs:
  glPushMatrix();
  glTranslated(0, -0.025, 0);
  glrotated(mLegrotation_, 0, 0, 1);
  glBegin(GL_LINES);
  glVertex2d(0, 0.025);
  glVertex2d(0, 0.1);
  glEnd();
  glPopMatrix();*/

  glPopMatrix();
}
