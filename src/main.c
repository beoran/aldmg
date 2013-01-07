
#include "alum.h"
#include <stdlib.h>

#define SCREEN_W 640
#define SCREEN_H 480

int main_loop(ALLEGRO_DISPLAY * display, ALLEGRO_EVENT_QUEUE   * queue) {
  int busy = 1;
  ALLEGRO_EVENT event;
  while (busy) {
    while(al_get_next_event(queue, &event)) {
      if(event.type == ALLEGRO_EVENT_KEY_DOWN) {
        switch(((ALLEGRO_KEYBOARD_EVENT*)&event)->keycode) {
          case ALLEGRO_KEY_ESCAPE:
             busy = 0;
          default:
            break;
        }
      }
    }
    al_flip_display();
  }
  return busy;
}

struct TrySender_ {
  AlumSender sender;
};

struct TryListener_ {
  int state;
  AlumListener listener;
};

typedef struct TryListener_ TryListener;

/* A test listener function. */
int trylistener_onstate(void * sender, void * listener, 
                         int message, void * data) {
  struct TryListener_ *container;
  container         = bad_container(listener, TryListener, listener);
  container->state  = *((int *)(data));
  printf("Broadcast %p %d %d\n", listener, message, container->state);
  return ALUM_REPLY_OK;
}

typedef struct TryAatree_ TryAatree;

struct TryAatree_ {
  int value;
  BadAatree tree;
}; 

void tryaatree_print(TryAatree * self) {
  printf("TryAatree: %p %d \n", self, self->value);
}

int tryaatree_compare(BadAatree * one, BadAatree * two) {
  struct TryAatree_ * t1;
  struct TryAatree_ * t2;
  t1 = bad_container(one, struct TryAatree_, tree);
  t2 = bad_container(two, TryAatree, tree);
  // printf("Compare : %p %p %d %d\n", t1, t2, t1->value, t2->value);
  // printf("Compare: %p %p %d %d\n", t1, t2, t1->value, t2->value);
  return bad_comparei(t1->value, t2->value);
}


int tryaatree_setvalue(BadAatree * one, BadAatree * two) {
  struct TryAatree_ * t1;
  struct TryAatree_ * t2;
  t1 = bad_container(one, struct TryAatree_, tree);
  t2 = bad_container(two, TryAatree, tree);
  return t1->value = t2->value;
}


TryAatree * tryaatree_init(TryAatree * self, int newvalue) {
  if(!self) return NULL;
  badaatree_init(&(self->tree));
  self->value = newvalue;
  return self;
}



int main(void) {
  struct TrySender_       sender;
  struct TryListener_     listener1  = {0}, listener2 = {0};
  struct TryAatree_       aatree[64];
  TryAatree               key;
  BadAatree             * root, * newroot;
  BadAatree             * aataid;
  TryAatree             * taid;
  int                     index;
  int                     argi = 7;
  ALLEGRO_DISPLAY       * display;
  ALLEGRO_EVENT_QUEUE   * queue;
  srand(time(NULL));
  alumsender_init(&sender.sender);
  alumsender_link(&sender.sender, &listener1.listener, trylistener_onstate);
  alumsender_link(&sender.sender, &listener2.listener, trylistener_onstate);
  assert(listener1.state == 0);
  assert(listener2.state == 0);
  printf("Broadcast\n");
  alum_broadcast(&sender.sender, 123, &argi);
  printf("States: %d, %d", listener1.state, listener2.state);
  root = NULL;
  for( index = 0; index < 64; index ++) {
    tryaatree_init(&(aatree[index]), index);
    newroot = badaatree_insert(root, 
                        &(aatree[index].tree), tryaatree_compare);
    // printf("Roots : %p %p", root, newroot);
    root = newroot;
  }
  
  assert(tryaatree_init(&key, 28));
  key.value = 28;
  aataid = badaatree_search(root, &(key.tree), tryaatree_compare);
  assert(aataid);
  taid = bad_container(aataid, TryAatree, tree);
  assert(taid->value == 28);
  root = badaatree_delete(root, &(key.tree), 
                            tryaatree_compare, tryaatree_setvalue);
  assert(root);
  taid = bad_container(root, TryAatree, tree);
  tryaatree_print(taid);
  printf("------\n");
  aataid = badaatree_search(root, &(key.tree), tryaatree_compare);
  assert(!aataid);
  assert(tryaatree_init(&key, 63));
  aataid = badaatree_search(root, &(key.tree), tryaatree_compare);
  assert(aataid);
  
  
  
  
  al_init();
  display = al_create_display(SCREEN_W, SCREEN_H);
  if(!display)  return 1;
  queue   = al_create_event_queue();
  if(!queue)    return 2;
  al_init_primitives_addon();
  al_install_keyboard();
  al_install_mouse();
  al_register_event_source(queue, al_get_keyboard_event_source());
  al_register_event_source(queue, al_get_mouse_event_source());
  main_loop(display, queue);
  alumsender_done(&sender.sender);
  al_destroy_event_queue(queue);
  al_destroy_display(display);
  return 0;
}

