#include "bad.h"

/*
* License: bad.h and bad.c are 
*
* Copyright (C) 2012-2013 beoran (beoran@beoran.net)
*
* Permission is hereby granted, free of charge, to any person obtaining a copy 
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights 
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is 
* furnished to do so, subject to the following conditions: 
*
* The above copyright notice and this permission notice shall be included in 
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
*/

int bad_mini(int one, int two) {
  return (one < two) ? one : two;  
}

double bad_mind(double one, double two) {
  return (one < two) ? one : two;  
}


int bad_maxi(int one, int two) {
  return (one > two) ? one : two;  
}

double bad_maxd(double one, double two) {
  return (one > two) ? one : two;
}


int bad_comparei(int one, int two) {
  if (one < two) return -1;
  if (one > two) return  1;
  return 0;
}

int bad_compared(double one, double two) {
  if (one < two) return -1;
  if (one > two) return  1;
  return 0;
}


/**
* Struct: BadList
*
* BadList is an intrusive doubly linked list.
* These lists do not contain any pointers to the data member that is part of 
* the list, rather, you use them by including them into the struct that 
* needs to be included in the intrusive linked list, hence the "intrusive".
* Use the macro bad_container to get the containing struct back.
*
* For example : struct Foo { int value ; BadList list } ...
* struct Foo foo, bar; badlist_init(&foo.list); badlist_init(&bar.list);
* badlist_add(&foo.list, &bar.list);
* 
*/

/**
* Function: badlist_initall
*
* Fully initializes a non-NULL intrusive linked list.
*
* Parameters:
*   self - BadList to initialize
*   next - Next BadList list node to link to or NULL
*   prev - Previous BadList list node to link to or NULL
*
* Returns: 
*   self
*/
BadList * badlist_initall(BadList * self, BadList * next, BadList * prev) {
  if (!self) return NULL;
  self->next = next;
  self->prev = prev;
  return self;
}

/**
* Function: badlist_init
*
* Initializes the intrusive linked list. Next and prev are set to NULL.
*
* Parameters:
*   self - BadList to initialize
*
* Returns: 
*   self
*/
BadList * badlist_init(BadList * self) {
  return badlist_initall(self, NULL, NULL);
}

/** 
* Function: badlist_unlink
*
* Unlinks self from the list it is part of. 
* Does NOT clean up any data asssociated with the container of the intrusive 
* list and also doesn't free self, since self should be part of the container 
* of the intrusive list. 
*
* Parameters:
*   self - BadList to remove from whole of list. May be NULL.
*
* Returns: 
*   self
*/
BadList * badlist_unlink(BadList * self) {  
  if(!self) return NULL;
  if(self->prev) { self->prev->next = self->next; }
  if(self->next) { self->next->prev = self->prev; }
  self->prev = NULL;
  self->next = NULL;
  return self;
}

/** 
* Function: badlist_add
*
* Appends other after self. 
*
* Parameters:
*   self - BadList to append to
*   other - BadList to append to self.
*
* Returns:
*   other if all went OK, NULL on error
*/
BadList * badlist_add(BadList * self, BadList * other) {  
  if(!self || !other) return NULL;
  self->next  = other;
  other->prev = self;
  return other;
}

/** 
* Function: badlist_next
*
* Returns the next element in the list
*
* Parameters:
*   self - BadList
*
* Returns: 
*   the next element in the list, or NULL if no next item. 
*/
BadList * badlist_next(BadList * self) {
  if(!self) return NULL;
  return self->next;
}

/** 
* Function: badlist_prev
*
* Returns the previous element in the list
*
* Parameters:
*   self - BadList
*
* Returns: 
*   the next element in the list, or NULL if no next item. 
*/
BadList * badlist_prev(BadList * self) {
  if(!self) return NULL;
  return self->prev;
}

/** 
* Function: badlist_first
*
* Returns the first element in the list, by dumb iteration.
*
* Parameters:
*   self - BadList
*
* Returns: 
*   the first link in the list, or NULL if self is NULL. 
*/
BadList * badlist_first(BadList * self) {
  BadList * aid = self; 
  if(!aid) return NULL;
  while (aid->prev) {
    aid = aid->prev;
  }
  return aid;  
}

/** 
* Function: badlist_last
*
* Returns the last element in the list, by dumb iteration.
*
* Parameters:
*   self - BadList
*
* Returns: 
*   the last link in the list, or NULL if self is NULL. 
*/
BadList * badlist_last(BadList * self) {
  BadList * aid = self; 
  if(!aid) return NULL;
  while (aid->next) {
    aid = aid->next;
  }
  return aid;  
}

/** 
* Function: badlist_push
*
* Appends other to the end of the list by dumb iteration.
*
* Parameters:
*   self - BadList
*
* Returns: 
*   other, or NULL if self or other is NULL. 
*/
BadList * badlist_push(BadList * self, BadList * other) {  
  BadList * aid;
  aid = badlist_last(self);
  return badlist_add(aid, other);
}


/** 
* Function: badlist_unshift 
*                
* Prepends other to the beginning of the list by dumb iteration.
*
* Parameters:
*   self - BadList
*
* Returns: 
*   other, or NULL if self or other is NULL. Other is now also the new 
*   beginning of the list.
*/
BadList * badlist_unshift(BadList * self, BadList * other) {  
  BadList * aid;
  aid = badlist_first(self);
  badlist_add(other, aid);
  return other;
}


/** 
* Function: badlist_shift
*
* Removes the first element from the list by dumb iteration.
*
* Parameters:
*   self - BadList
*
* Returns: 
*   list node that was removed, or NULL if self is NULL. 
*/
BadList * 
badlist_shift(BadList * self) {  
  BadList * aid;
  aid = badlist_first(self);
  return badlist_unlink(aid);
}


/** 
* Function: badlist_pop
*
* Removes the last element from the list by dumb iteration.
*
* Parameters:
*   self - BadList
*
* Returns: 
*   list node that was removed, or NULL if self is NULL. 
*/
BadList * 
badlist_pop(BadList * self) {  
  BadList * aid;
  aid = badlist_last(self);
  return badlist_unlink(aid);
}


/** 
* Function: badlist_data
* 
* Parameters:
* self - list node to get the data of.
* offset - Use offsetof to calculate this offset.
*
* Returns:  
*   the data for this node of the singly linked list. 
* 
*/
void * 
badlist_data(BadList * self, int offset) {
  if(!self) return NULL;
  return (void *)((char *)self - offset);
}



/** 
* Function: badlist_search
* 
* Parameters:
* self   - list node to search.
* offset - Comparer function, get passed eachelement of the BadList * and 
*          tofind, must return 0 if the item searched for is found.
*
* Returns:  
*   the BadList found, or NULL if not found.
* 
*/
BadList * 
badlist_search(BadList * self, BadListCompare * compare, BadList * tofind) {
  BadList * aid;
  for(aid = self; aid ; aid = aid->next) {
    int cmp = compare(aid, tofind);
    if(cmp == 0) return aid;
  }
  return NULL;
}


/*
* Function: badlist_unlink
*
* A shorthand for badlist_unlink(badlist_search(self, compare, toremove));
*/
BadList * 
badlist_remove(BadList * self, BadListCompare * compare, BadList * toremove) {  
  return badlist_unlink(badlist_search(self, compare, toremove));
}



BadBitree * 
badbitree_initall(BadBitree * self, BadBitree * left, BadBitree * right, 
                  BadBitree * up) {
  if(!self) return NULL;
  self->left  = left;
  self->right = right;
  self->up    = up;
  return self;
}

BadBitree *
badbitree_init(BadBitree * self) {
  return badbitree_initall(self, NULL, NULL, NULL);
}

BadBitree *
badbitree_left(BadBitree * self) {
  if(!self) return NULL;
  return self->left;
}

BadBitree *
badbitree_right(BadBitree * self) {
  if(!self) return NULL;
  return self->right;
}

BadBitree *
badbitree_up(BadBitree * self) {
  if(!self) return NULL;
  return self->right;
}


BadBitree *
badbitree_up_(BadBitree * self, BadBitree * newup) {
  if(!self) return NULL;
  return self->up = newup;
}

BadBitree *
badbitree_left_(BadBitree * self, BadBitree * newleft) {
  if(!self) return NULL;
  badbitree_up_(newleft, self);
  return self->left = newleft;
}

BadBitree *
badbitree_right_(BadBitree * self, BadBitree * newright) {
  if(!self) return NULL;
  badbitree_up_(newright, self);
  return self->right = newright;
}

bool badbitree_isleaf(BadBitree * self) {
  if(!self) return FALSE;
  if(badbitree_right(self)) return FALSE;
  if(badbitree_left(self)) return FALSE;
  return TRUE;
}


BadAatree * 
badaatree_initall(BadAatree * self, BadAatree * left, BadAatree * right, 
                  BadAatree * up, int level) {
  if(!badbitree_initall(&self->tree, &left->tree, &right->tree, &up->tree)) 
    return NULL;
  self->level = level;
  return self;
}

BadAatree *
badaatree_init(BadAatree * self) {
  return badaatree_initall(self, NULL, NULL, NULL, 1);
}

#define bad_bi2aa(PTR) bad_container(PTR, BadAatree, tree)

BadAatree *
badaatree_left(BadAatree * self) {
  if(!self) return NULL;
  return bad_bi2aa(badbitree_left(&self->tree));
}

BadAatree *
badaatree_right(BadAatree * self) {
  if(!self) return NULL;
  return bad_bi2aa(badbitree_right(&self->tree));
}

BadAatree *
badaatree_up(BadAatree * self) {
  if(!self) return NULL;
  return bad_bi2aa(badbitree_up(&self->tree));
}

BadAatree *
badaatree_left_(BadAatree * self, BadAatree * other) {
  if(!self) return NULL;
  return bad_bi2aa(badbitree_left_(&self->tree, &other->tree));
}

BadAatree *
badaatree_right_(BadAatree * self, BadAatree * other) {
  if(!self) return NULL;
  return bad_bi2aa(badbitree_right_(&self->tree, &other->tree));
}

BadAatree *
badaatree_up_(BadAatree * self, BadAatree * other) {
  if(!self) return NULL;
  return bad_bi2aa(badbitree_up_(&self->tree, &other->tree));
}

int badaatree_level(BadAatree * self) { 
  if(!self) return -1;
  return self->level;
}

int badaatree_level_(BadAatree * self, int newlevel) { 
  if(!self) return -1;
  return self->level = newlevel;
}

bool badaatree_isleaf(BadAatree * self) {
  if(!self) return FALSE;
  return badbitree_isleaf(&self->tree);
}

BadAatree * badaatree_skew(BadAatree * self) { 
  BadAatree * left, * leftleft;
  if(!self) return NULL;
  left        = badaatree_left(self);
  if(!left) return self;
  leftleft    = badaatree_left(left);
  if(!leftleft) return self;
  if(badaatree_level(self) == badaatree_level(leftleft)) {
    BadAatree * left = badaatree_left(self);
    badaatree_left_(self, badaatree_right(left));
    badaatree_right_(left, self);
    return left;
  }
  return self;
}

BadAatree * badaatree_split(BadAatree * self) { 
  BadAatree * right, * rightright;
  if(!self) return NULL;
  right       = badaatree_right(self);
  if(!right) return self;
  rightright  = badaatree_right(right);
  if(!rightright)             return self;
  if (badaatree_level(self) == badaatree_level(rightright)) {
    badaatree_right_(self, badaatree_left(right));
    badaatree_left_(right, self);
    badaatree_level_(right, badaatree_level(right) + 1);
    return right;
  }
  return self;
}

BadAatree * badaatree_insert(BadAatree * self, BadAatree * node, 
                             BadAatreeCompare compare) { 
  int cmp;
  BadAatree * aid; 
  if(!self) { return node; } 
  cmp = compare(self, node);
  if(cmp < 0) {
    aid = badaatree_insert(badaatree_left(self), node, compare);
    badaatree_left_(self, aid);
  } else if (cmp > 0) {
    aid = badaatree_insert(badaatree_right(self), node, compare);
    badaatree_right_(self, aid);
  } else { /* Ignore duplicates for now,... */
  }
  self = badaatree_skew(self);
  self = badaatree_split(self);
  return self;
}

BadAatree * badaatree_search(BadAatree * self, BadAatree * node,
                             BadAatreeCompare compare) { 
  int cmp;
  BadAatree * aid; 
  if(!self) { return NULL; } 
  cmp = compare(self, node);
  if(cmp < 0) {
    return badaatree_search(badaatree_left(self), node, compare);
  } else if (cmp > 0) {
    return badaatree_search(badaatree_right(self), node, compare);
  } else { /* Found the item! */
    return self;
  }
}


BadAatree * badaatree_leveldownall(BadAatree * self) {
  BadAatree * right = badaatree_right(self);
  int lowest = bad_mini(badaatree_level(self), badaatree_level(right)) + 1;
  if (lowest < badaatree_level(self)) {
    badaatree_level_(self, lowest);
    if (lowest < badaatree_level(right)) {
      badaatree_level_(right, lowest);
    }
  }
  return self;
}

BadAatree * badaatree_successor(BadAatree * self) {
  BadAatree * aid, *next;
  if(!self) return NULL;
  aid  = badaatree_right(self); 
  if(!aid) return NULL;
  next = badaatree_left(aid);
  while (next) {
    aid  = next;
    next = badaatree_left(aid); 
  };
  return aid;
}

BadAatree * badaatree_predecessor(BadAatree * self) {
  BadAatree * aid, *next;
  if(!self) return NULL;
  aid  = badaatree_left(self); 
  if(!aid) return NULL;
  next = badaatree_right(aid);
  while (next) {
    aid  = next;
    next = badaatree_right(aid); 
  };
  return aid;
}

BadAatree * badaatree_delete(BadAatree * self, BadAatree * node, 
                             BadAatreeCompare * compare, 
                             BadAatreeSetValue * set) {
  BadAatree * aid;
  int cmp;
  if(!self) { return self; } 
  cmp = compare(self, node);
  if(cmp < 0) {
    aid = badaatree_delete(badaatree_left(self), node, compare, set);
    badaatree_left_(self, aid);
  } else if (cmp > 0) {
    aid = badaatree_delete(badaatree_right(self), node, compare, set);
    badaatree_right_(self, aid);
  } else { /* Found the value ! */
    if(badaatree_isleaf(self)) {
      badaatree_init(self); /* call init to unlink the tree's up pointer. */
      return NULL;
    } else if (!badaatree_left(self)) {
      BadAatree * 
      left  = badaatree_successor(self);
      aid   = badaatree_delete(left, badaatree_right(self), compare, set);
      badaatree_right_(self, aid);
      set(self, left);
    } else {
      BadAatree * 
      right = badaatree_predecessor(self);
      aid   = badaatree_delete(right, badaatree_left(self), compare, set);
      badaatree_left_(self, aid);
      set(self, right);
    }
  }
  /* Rebalance */
  badaatree_leveldownall(self);
  self  = badaatree_skew(self);
  aid   = badaatree_right(self);
  aid   = badaatree_skew(aid);
  badaatree_right_(self, aid);
  aid   = badaatree_right(badaatree_right(self));
  aid   = badaatree_skew(aid);
  badaatree_right_(badaatree_right(self), aid);
  self  = badaatree_split(self);
  aid   = badaatree_right(self);
  aid   = badaatree_skew(aid);
  badaatree_right_(self, aid);
  return self;
}

void badaatree_printgraph(BadAatree * self, int level) {
  if(level == 0) {
    printf("digraph { \n");
  }
  level++;
  if(badaatree_left(self)) { 
    printf("t%p -> t%p [color=red];\n", self, badaatree_left(self));
    badaatree_printgraph(badaatree_left(self), level);
  }
  if(badaatree_right(self)) {
    printf("t%p -> t%p [color=green];\n", self, badaatree_right(self));
    badaatree_printgraph(badaatree_right(self), level);
  }
  level--;
  if(level == 0) {
    printf("} \n");
  }
}


