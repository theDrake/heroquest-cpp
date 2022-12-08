/*******************************************************************************
   Filename: quest.h

     Author: David C. Drake (https://davidcdrake.com)

Description: Declaration of a 'Quest' class responsible for managing all
             aspects of a given quest, including its associated location, NPCs,
             traps, items, events, etc.
*******************************************************************************/

#ifndef QUEST_H_
#define QUEST_H_

#include <vector>
#include "main.h"
#include "character.h"
#include "cell.h"

using namespace std;

class Cell;
class Character;

enum Perspective {
  FIRST_PERSON,
  THIRD_PERSON,
  NUM_PERSPECTIVES
};

const int DEFAULT_QUEST_NO = 1;
const int DEFAULT_MAZE_WIDTH = 26;
const int DEFAULT_MAZE_HEIGHT = 19;
const int DEFAULT_NUM_NPCS = 30;
const int TEXTURE_OFFSET_PER_QUEST = 4;
const int DEFAULT_PERSPECTIVE = FIRST_PERSON;

class Quest {
 public:
  Quest(int questNo = DEFAULT_QUEST_NO,
        int width = DEFAULT_MAZE_WIDTH,
        int height = DEFAULT_MAZE_HEIGHT,
        int perspective = DEFAULT_PERSPECTIVE);
  ~Quest();
  void initialize(int questNo, int width, int height, int perspective);
  int initializeCells();
  int initializeCharacters();
  int assignNeighbors(int x, int y);
  int removeWalls(int x, int y);
  void setStartAndFinish();
  Character *setPlayer(Character *player);
  int setPerspective(int perspective);
  int getPerspective() const;
  int getWidth() const;
  int getHeight() const;
  int getStartX() const;
  int getFinishX() const;
  bool isLegalPosition(double x, double y, double radius) const;
  void draw();
 private:
  int questNo_,
      width_,
      height_,
      startX_,
      finishX_,
      perspective_;
  vector<Cell *> cells_;
  Character *player_;
  vector<Character *> characters_;
};

#endif  // QUEST_H_
