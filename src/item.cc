/*******************************************************************************
   Filename: item.cc

     Author: David C. Drake (https://davidcdrake.com)

Description: Definition of an 'Item' class.
*******************************************************************************/

#include "item.h"

Item::Item(int type) {
  type_ = type;
}

Item::~Item() {}

int Item::getType() {
  return type_;
}

void Item::draw() {}
