
#include "aldmg.h"
#include <stdlib.h>



// #define SCREEN_W 640
// #define SCREEN_H 480

#define SCREEN_W 800
#define SCREEN_H 600


char * tryme[] = {
  "Hello",
  "World",
  "How",
  "Are",
  "You",
  NULL
};

int do_gui(ALDMG_GUI * gui, double fps) {
  static int button_ids[200] = { 0 };
  static int hide[200] = { 0 };
  int index;
  aldmg_set_position(gui, 16, 16);
  aldmg_set_size(gui, 64, 16);
  
  aldmg_labelf(gui, 1, "%lf", fps);
  aldmg_set_position(gui, 16, 32);
  
  
  for (index = 0; index < 200 ; index ++) {    
    if (index % 8 == 0)
    aldmg_set_position(gui, 16, 32 + (index * 2));

    if (!hide[index] ) {
      button_ids[index] = index + 2;
      if (aldmg_button(gui, index + 2, tryme[index % 5])) {
        
        printf("%d clicked!\n", button_ids[index + 1]);
        hide[(index + 1) % 200] = !hide[(index + 1) % 200];
      }    
    }
  } 
  
  /*
  if (show_1) {
    
  }
  if (aldmg_button(gui, 2, "World", 0.15, 0.05, 0.1, 0.2)) {
    printf("2 clicked!\n");
    show_1 = ! show_1;
  }
  */ 
  return 0;
}

int main_loop(ALLEGRO_DISPLAY * display, ALLEGRO_EVENT_QUEUE   * queue) {
  int busy = 1;
  ALLEGRO_EVENT event;
  ALDMG_GUI *   gui;
  double        start;
  double        now;
  unsigned long frames;
  double        fps;
  
  start  = al_get_time();
  frames = 0;
  gui = aldmg_create_gui(display, queue); 
  
  while (busy) {
    now     = al_get_time();
    frames++;
    fps     = frames / (now - start);  
    al_clear_to_color(al_map_rgb(64,128,64));
    
    while(al_get_next_event(queue, &event)) {
      if(event.type == ALLEGRO_EVENT_KEY_DOWN) {
        switch(((ALLEGRO_KEYBOARD_EVENT*)&event)->keycode) {
          case ALLEGRO_KEY_ESCAPE:
             busy = 0;
          default:
            break;
        }
      } else {
        aldmg_handle_event(gui, &event);
      }
    }
    
    do_gui(gui, fps);
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

