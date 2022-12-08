/*******************************************************************************
   Filename: cell.cc

     Author: David C. Drake (https://davidcdrake.com)

Description: Definition of a 'Cell' class, corresponding to a single cube of
             space within a 3D environment.
*******************************************************************************/

#include "cell.h"

//------------------------------------------------------------------------------
//      Method: Cell
//
// Description: Constructs a Cell object with walls on all sides, no neighbors,
//              no textures, and in a "not visited" state.
//
//      Inputs: None.
//
//     Outputs: None.
//------------------------------------------------------------------------------
Cell::Cell() {
  for (int i = 0; i < NUM_SIDES; ++i) {
    walls_[i] = true;
    neighbors_[i] = NULL;
    textures_[i] = 0;
  }
  visited_ = false;
}

//------------------------------------------------------------------------------
//      Method: ~Cell
//
// Description: Destructor.
//
//      Inputs: None.
//
//     Outputs: None.
//------------------------------------------------------------------------------$
Cell::~Cell() {}

//------------------------------------------------------------------------------
//      Method: setVisited
//
// Description: Sets the Cell's "visited" state to a given boolean value.
//
//      Inputs: visited - The boolean value to assign to the Cell's "visited"
//                        state.
//
//     Outputs: The newly assigned value of the Cell's "visited" state.
//------------------------------------------------------------------------------
bool Cell::setVisited(bool visited) {
  return visited_ = visited;
}

//------------------------------------------------------------------------------
//      Method: setTexture
//
// Description: Assigns a given texture to a given side of the Cell.
//
//      Inputs: side      - Integer representing the side of interest (NORTH,
//                          SOUTH, EAST, WEST, TOP, or BOTTOM).
//              textureNo - Integer representing the desired texture.
//
//     Outputs: The integer value of the newly assigned texture.
//------------------------------------------------------------------------------
int Cell::setTexture(int side, int textureNo) {
  return textures_[side] = textureNo;
}

//------------------------------------------------------------------------------
//      Method: setNeighbor
//
// Description: Assigns the Cell a given neighbor on a given side.
//
//      Inputs: side - Integer representing the side of interest (NORTH, SOUTH,
//                     EAST, WEST, TOP, or BOTTOM).
//              cell - Pointer to the Cell that is to be assigned as a
//                     neighbor.
//
//     Outputs: A pointer to the newly assigned neighbor.
//------------------------------------------------------------------------------
Cell *Cell::setNeighbor(int side, Cell *cell) {
  return neighbors_[side] = cell;
}

//------------------------------------------------------------------------------
//      Method: removeWall
//
// Description: Removes a wall along a given side of the Cell. Also removes the
//              corresponding wall of the Cell's neighbor on that side (i.e.,
//              "the other side of the wall") if a neighbor exists there.
//
//      Inputs: side - Integer representing the side of interest (NORTH, SOUTH,
//                     EAST, WEST, TOP, or BOTTOM).
//
//     Outputs: None.
//------------------------------------------------------------------------------
void Cell::removeWall(int side) {
  walls_[side] = false;

  int opposite = oppositeSide(side);
  if (neighbors_[side] && neighbors_[side]->hasWallAt(opposite)) {
    neighbors_[side]->removeWall(opposite);
  }
}

//------------------------------------------------------------------------------
//      Method: removeRandomWall
//
// Description: Removes a wall along one of the four cardinal directions,
//              randomly selected, unless no valid removal can occur in any of
//              those directions (the Cell must have both a wall and an
//              unvisited neighbor in a given direction for it to be valid).
//
//      Inputs: None.
//
//     Outputs: An integer representing the side where a wall was removed
//              (NORTH, SOUTH, EAST, or WEST), or -1 if no wall was removed.
//------------------------------------------------------------------------------
int Cell::removeRandomWall() {
  int availablePaths = 4;

  for (int i = 0; i < 4; ++i) {
    if (!walls_[i] || !neighbors_[i] || neighbors_[i]->hasBeenVisited()) {
      --availablePaths;
    }
  }

  while (availablePaths > 0) {
    int side = rand() % 4;

    if (walls_[side] && neighbors_[side] &&
        !neighbors_[side]->hasBeenVisited()) {
      removeWall(side);

      return side;
    }
  }

  return -1;
}

//------------------------------------------------------------------------------
//      Method: hasBeenVisited
//
// Description: Determines whether the Cell has been visited.
//
//      Inputs: None.
//
//     Outputs: Returns 'true' if the cell has been visited, 'false' otherwise.
//------------------------------------------------------------------------------
bool Cell::hasBeenVisited() const {
  return visited_;
}

//------------------------------------------------------------------------------
//      Method: hasWallAt
//
// Description: Determines whether the Cell has a wall along a given side.
//
//      Inputs: side - Integer representing the side of interest (NORTH, SOUTH,
//                     EAST, WEST, TOP, or BOTTOM).
//
//     Outputs: Returns 'true' if a wall exists along the given side, 'false'
//              otherwise.
//------------------------------------------------------------------------------
bool Cell::hasWallAt(int side) const {
  return walls_[side];
}

//------------------------------------------------------------------------------
//      Method: hasNeighborAt
//
// Description: Determines whether the Cell has a neighbor (i.e., an adjoining
//              Cell object) on a given side.
//
//      Inputs: side - Integer representing the side of interest (NORTH, SOUTH,
//                     EAST, WEST, TOP, or BOTTOM).
//
//     Outputs: Returns 'true' if a neighbor exists on the given side, 'false'
//              otherwise.
//------------------------------------------------------------------------------
bool Cell::hasNeighborAt(int side) const {
  return neighbors_[side] != NULL;
}

//------------------------------------------------------------------------------
//      Method: draw
//
// Description: Draws each of the Cell's walls (if any) according to given
//              coordinates.
//
//      Inputs: i, j        - The Cell's coordinates within the 3D environment,
//                            measured in cells.
//              perspective - Integer representing the current perspective mode
//                            (for determining whether to display the ceiling).
//
//     Outputs: None.
//------------------------------------------------------------------------------
void Cell::draw(int i, int j, int perspective) {
  double r = i / (double) DEFAULT_MAZE_WIDTH;
  double g = j / (double) DEFAULT_MAZE_HEIGHT;
  double b = 0.5;

  glColor3d(r, g, b);
  if (walls_[NORTH]) {
    /*if (textures_[NORTH] && mMiniTextures[NORTH])
    {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textures_[NORTH]);
    glBegin(GL_QUADS);
    glTexCoord2f(-1, -1); glVertex3d(i, j + 1, 0);
    glTexCoord2f(2, -1); glVertex3d(i + 1, j + 1, 0);
    glTexCoord2f(2, 2); glVertex3d(i + 1, j + 1, 1);
    glTexCoord2f(-1, 2); glVertex3d(i, j + 1, 1);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    }
    else*/
    if (textures_[NORTH] > 0) {
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, textures_[NORTH]);
      glBegin(GL_QUADS);
      glTexCoord2f(0, 0); glVertex3d(i, j + 1, 0);
      glTexCoord2f(1, 0); glVertex3d(i + 1, j + 1, 0);
      glTexCoord2f(1, 1); glVertex3d(i + 1, j + 1, 1);
      glTexCoord2f(0, 1); glVertex3d(i, j + 1, 1);
      glEnd();
      glDisable(GL_TEXTURE_2D);
    } else {
      glBegin(GL_QUADS);
      glVertex3d(i, j + 1, 0);
      glVertex3d(i + 1, j + 1, 0);
      glVertex3d(i + 1, j + 1, 1);
      glVertex3d(i, j + 1, 1);
      glEnd();
    }
  }
  if (walls_[SOUTH] && j == 0) {
    if (textures_[SOUTH]) {
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, textures_[SOUTH]);
      glBegin(GL_QUADS);
      glTexCoord2f(0, 0); glVertex3d(i, j, 0);
      glTexCoord2f(1, 0); glVertex3d(i + 1, j, 0);
      glTexCoord2f(1, 1); glVertex3d(i + 1, j, 1);
      glTexCoord2f(0, 1); glVertex3d(i, j, 1);
      glEnd();
      glDisable(GL_TEXTURE_2D);
    } else {
      glBegin(GL_QUADS);
      glVertex3d(i, j, 0);
      glVertex3d(i + 1, j, 0);
      glVertex3d(i + 1, j, 1);
      glVertex3d(i, j, 1);
      glEnd();
    }
  }
  if (walls_[WEST] && i == 0) {
    if (textures_[WEST]) {
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, textures_[WEST]);
      glBegin(GL_QUADS);
      glTexCoord2f(0, 0); glVertex3d(i, j, 0);
      glTexCoord2f(1, 0); glVertex3d(i, j + 1, 0);
      glTexCoord2f(1, 1); glVertex3d(i, j + 1, 1);
      glTexCoord2f(0, 1); glVertex3d(i, j, 1);
      glEnd();
      glDisable(GL_TEXTURE_2D);
    } else {
      glBegin(GL_QUADS);
      glVertex3d(i, j, 0);
      glVertex3d(i, j + 1, 0);
      glVertex3d(i, j + 1, 1);
      glVertex3d(i, j, 1);
      glEnd();
    }
  }
  if (walls_[EAST]) {
    if (textures_[EAST]) {
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, textures_[EAST]);
      glBegin(GL_QUADS);
      glTexCoord2f(0, 0); glVertex3d(i + 1, j, 0);
      glTexCoord2f(1, 0); glVertex3d(i + 1, j + 1, 0);
      glTexCoord2f(1, 1); glVertex3d(i + 1, j + 1, 1);
      glTexCoord2f(0, 1); glVertex3d(i + 1, j, 1);
      glEnd();
      glDisable(GL_TEXTURE_2D);
    } else {
      glBegin(GL_QUADS);
      glVertex3d(i + 1, j, 0);
      glVertex3d(i + 1, j + 1, 0);
      glVertex3d(i + 1, j + 1, 1);
      glVertex3d(i + 1, j, 1);
      glEnd();
    }
  }
  if (walls_[TOP] && perspective == FIRST_PERSON) {
    if (textures_[TOP]) {
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, textures_[TOP]);
      glBegin(GL_QUADS);
      glTexCoord2f(0, 0); glVertex3d(i, j, 1);
      glTexCoord2f(1, 0); glVertex3d(i + 1, j, 1);
      glTexCoord2f(1, 1); glVertex3d(i + 1, j + 1, 1);
      glTexCoord2f(0, 1); glVertex3d(i, j + 1, 1);
      glEnd();
      glDisable(GL_TEXTURE_2D);
    } else {
      glBegin(GL_QUADS);
      glVertex3d(i, j, 1);
      glVertex3d(i + 1, j, 1);
      glVertex3d(i + 1, j + 1, 1);
      glVertex3d(i, j + 1, 1);
      glEnd();
    }
  }
  if (walls_[BOTTOM]) {
    if (textures_[BOTTOM]) {
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, textures_[BOTTOM]);
      glBegin(GL_QUADS);
      glTexCoord2f(0, 0); glVertex3d(i, j, 0);
      glTexCoord2f(1, 0); glVertex3d(i + 1, j, 0);
      glTexCoord2f(1, 1); glVertex3d(i + 1, j + 1, 0);
      glTexCoord2f(0, 1); glVertex3d(i, j + 1, 0);
      glEnd();
      glDisable(GL_TEXTURE_2D);
    } else {
      glBegin(GL_QUADS);
      glVertex3d(i, j, 0);
      glVertex3d(i + 1, j, 0);
      glVertex3d(i + 1, j + 1, 0);
      glVertex3d(i, j + 1, 0);
      glEnd();
    }
  }
}

//------------------------------------------------------------------------------
//      Method: oppositeSide
//
// Description: A private method that returns an integer representing the
//              opposite of a given side value (e.g., if NORTH is passed in,
//              SOUTH will be returned).
//
//      Inputs: side - Integer representing the side of interest (NORTH, SOUTH,
//                     EAST, WEST, TOP, or BOTTOM).
//
//     Outputs: An integer representing the side opposite the given side value,
//              or -1 if the given value was invalid.
//------------------------------------------------------------------------------
int Cell::oppositeSide(int side) {
  switch (side) {
    case NORTH:
      return SOUTH;
      break;
    case SOUTH:
      return NORTH;
      break;
    case WEST:
      return EAST;
      break;
    case EAST:
      return WEST;
      break;
    case TOP:
      return BOTTOM;
      break;
    case BOTTOM:
      return TOP;
      break;
    default:
      break;
  }

  return -1;
}
