/******************************************************************************
   Filename: Cell.h

     Author: David C. Drake (http://davidcdrake.com)

Description: Declaration of a 'Cell' class, corresponding to a single cube of
             space within a 3D environment.
******************************************************************************/

#ifndef CELL_H_
#define CELL_H_

#include "Quest.h"

const double CELL_SIZE = 1.0;

const enum Side {
  NORTH,
  SOUTH,
  EAST,
  WEST,
  TOP,
  BOTTOM,
  NUM_SIDES
};

class Cell {
 public:
  Cell();
  ~Cell();
  bool setVisited(bool visited);
  int setTexture(int side, int textureNo);
  Cell *setNeighbor(int side, Cell *cell);
  void removeWall(int side);
  int removeRandomWall();
  bool hasBeenVisited() const;
  bool hasWallAt(int side) const;
  bool hasNeighborAt(int side) const;
  void draw(int x, int y, int perspective);
 private:
   bool walls_[NUM_SIDES],
     visited_;
   Cell *neighbors_[NUM_SIDES];
   int textures_[NUM_SIDES];

   int oppositeSide(int side);
};

#endif  // CELL_H_
