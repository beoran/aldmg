
#include "alum.h"


#include <stdlib.h>

/* Func: alumpoint
Creates a new AlumPoint.
*/

AlumPoint alumpoint(float x, float y) {
  AlumPoint result = { x, y };
  return result;
}

/** Makes a new bounds box struct. */
AlumBox alumbox_make(float x, float y, float w, float h) {
  AlumPoint p = alumpoint(x, y);
  AlumPoint s = alumpoint(w, h);
  AlumBox result = { p, s };
  return result;
}

/** Initializes a bounds pointer by copying data from a bounds struct.  */
AlumBox * alumbox_initbounds(AlumBox * self, AlumBox bounds) {
  if(!self) return NULL;
  (*self) = bounds;
  return self;
}

/** Initializes a bounds pointer from it's coordinates. */
AlumBox * alumbox_init(AlumBox * self, int x, int y, int w, int h) {
  return alumbox_initbounds(self, alumbox_make(x, y, w, h));
}

/** Returns a newly initialised rectangular box. */
AlumBox alumbox_new(float x, float y, float w, float h) {
  AlumBox result;
  /** Avoid negative widths and heights by adjusting x and y accordingly */
  if (w < 0) { x = x - w ; w = -w ; } 
  if (h < 0) { y = y - h ; w = -h ; } 
  result.at   = alumpoint(x, y);
  result.size = alumpoint(w, h);
  return result;
}


/** Return position of AlumBox top left corner. */
AlumPoint alumbox_at(AlumBox * self) {
  return self->at;
}

/** Sets position by individual components. */
AlumPoint alumbox_x_(AlumBox * self, float x) {
  self->at.x = x;
  return self->at;
}

/** Sets position by individual components. */
AlumPoint alumbox_y_(AlumBox * self, float y) {
  self->at.y = y;
  return self->at;
}


/** Sets position by individual components. */
AlumPoint alumbox_xy_(AlumBox * self, float x, float y) {
  self->at.x = x;
  self->at.y = y;
  return self->at;
}

/** Sets position. */
AlumPoint alumbox_at_(AlumBox * self, AlumPoint at) {
  self->at = at;
  return self->at;
}

/** Return x position of AlumBox top left corner. */
float alumbox_x(AlumBox * self) {
  return self->at.x;
}

/** Return y position of AlumBox top left corner. */
float alumbox_y(AlumBox * self) {
  return self->at.y;
}

/** Return width of AlumBox view. */
float alumbox_w(AlumBox * self) {
  return self->size.x;
}

/** Return height of AlumBox view. */
float alumbox_h(AlumBox * self) {
  return self->size.y;
}

/** Return x position of AlumBox bottom right corner. */
float alumbox_br_x(AlumBox * self) {
  return self->at.x + self->size.x;
}

/** Return y position of AlumBox bottom right corner. */
float alumbox_br_y(AlumBox * self) {
  return self->at.y + self->size.y;
}

/** Return x position of AlumBox view center. */
float alumbox_center_x(AlumBox * self) {
  return self->at.x + (self->size.x / 2);
}

/** Return y position of AlumBox bottom center */
float alumbox_center_y(AlumBox * self) {
  return self->at.y + (self->size.y / 2);;
}

/** Return position of AlumBox view center. */
AlumPoint alumbox_center(AlumBox * self) {
  return alumpoint(alumbox_center_x(self), alumbox_center_y(self));
}


/** Sets position of center of AlumBox to center. */
AlumPoint alumbox_center_(AlumBox * self, AlumPoint center) {
  AlumPoint at;
  at.x = center.x - (alumbox_w(self) / 2);
  at.y = center.y - (alumbox_h(self) / 2);
  return alumbox_at_(self, at);
}



/* Returns TRUE if box 2 if other is wholly inside self, FALSE if not. */
int alumbox_inside_p(AlumBox * self, AlumBox * other) {
  if (other->at.x < self->at.x) return FALSE;
  if (other->at.y < self->at.y) return FALSE;
  if (alumbox_br_x(other) > alumbox_br_x(self)) return FALSE;
  if (alumbox_br_y(other) > alumbox_br_y(self)) return FALSE;
  return TRUE;
}

/* Returns 0 if AlumBox other if other is wholly inside the AlumBox self,
otherwise the distance the box other has to move 
in the x direction to make it so. 
Only works if other is smaller than self. */
double alumbox_delta_x(AlumBox * self, AlumBox * other) {
  double dx = self->at.x - other->at.x;
  if (dx > 0) return dx;
  dx = alumbox_br_x(other) - alumbox_br_x(self);
  if (dx > 0) return -dx;
  return 0.0;
}

/* Returns 0 if box 2 if other is wholly inside self,
otherwise the distance the box other has to move 
in the y direction to make it so. 
Only works if other is smaller than self. */
double alumbox_delta_y(AlumBox * self, AlumBox * other) {
  double dy = self->at.y - other->at.y; 
  if (dy > 0) return dy; 
  dy = alumbox_br_y(other) - alumbox_br_y(self);
  if (dy > 0) return -dy;
  return 0.0;
}

