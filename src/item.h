/*******************************************************************************
   Filename: item.h

     Author: David C. Drake (https://davidcdrake.com)

Description: Declaration of an 'Item' class.
*******************************************************************************/

#ifndef ITEM_H_
#define ITEM_H_

class Item {
 public:
  Item(int type);
  ~Item();
  int getType();
  void draw();
 private:
  int type_;
};

#endif  // ITEM_H_
