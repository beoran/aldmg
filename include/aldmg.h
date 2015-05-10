#ifndef ALUM_H_INCLUDED
#define ALUM_H_INCLUDED

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#include "bad.h"

/* Forward typedefs. */
typedef struct ALDMG_GUI_              ALDMG_GUI;
typedef struct ALDMG_STYLE_            ALDMG_STYLE;

/*
*
* Represents a point in 2D space, or a 2D size.
*/
typedef struct ALDMG_POINT_ {
  float x, y;
} ALDMG_POINT;

/*
*
* Represents a rectangular box.
*/
typedef struct ALDMG_BOX_ {
  ALDMG_POINT at;
  ALDMG_POINT size;
} ALDMG_BOX;


/*
* Describes the style of a widget.
*/
struct ALDMG_STYLE_ {
  ALLEGRO_COLOR     forecolor;
  ALLEGRO_COLOR     backcolor;
  ALLEGRO_FONT    * font;
  ALLEGRO_BITMAP  * background;
  int               bordersize;
};

/* Creates a gui for a given display and event queue. */
ALDMG_GUI * aldmg_create_gui(
    ALLEGRO_DISPLAY * display, ALLEGRO_EVENT_QUEUE * queue);
    
int aldmg_handle_event(ALDMG_GUI * gui, ALLEGRO_EVENT * event);

void aldmg_destroy_gui(ALDMG_GUI * gui);
    aldmg_draw_gui(gui);    



#endif // ALUM_H_INCLUDED
