
#include "aldmg.h"
#include <stdlib.h>

#define SCREEN_W 640
#define SCREEN_H 480


int do_gui(ALDMG_GUI * gui) {
  return 0;
}

int main_loop(ALLEGRO_DISPLAY * display, ALLEGRO_EVENT_QUEUE   * queue) {
  int busy = 1;
  ALLEGRO_EVENT event;
  ALDMG_GUI * gui;
  
  aldmg_create_gui(display, queue); 
  
  while (busy) {
    while(al_get_next_event(queue, &event)) {
      if(event.type == ALLEGRO_EVENT_KEY_DOWN) {
        switch(((ALLEGRO_KEYBOARD_EVENT*)&event)->keycode) {
          case ALLEGRO_KEY_ESCAPE:
             busy = 0;
          default:
            break;
        }
      } else {
        aldmg_handle_event(gui, event);
      }
    }
    
    do_gui(gui);
    aldmg_draw_gui(gui);
    
    al_flip_display();
  }
  return busy;
}


int main(void) {
  ALLEGRO_DISPLAY       * display;
  ALLEGRO_EVENT_QUEUE   * queue;
  srand(time(NULL));
  
    
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
  al_destroy_event_queue(queue);
  al_destroy_display(display);
  return 0;
}

