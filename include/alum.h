#ifndef ALUM_H_INCLUDED
#define ALUM_H_INCLUDED

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

/* Const: FALSE.
* Of course we define our own boolean value for FALSE.
*/
#ifndef FALSE 
#define FALSE 0
#endif 

/* Const: TRUE.
* Of course we define our own boolean value for TRUE.
*/
#ifndef TRUE 
#define TRUE (!FALSE)
#endif 

/* 
* Typedef: AlumBitmap
* A shorthand for ALLEGRO_BITMAP
*
* Typedef: AlumFont
* A shorthand for ALLEGRO_FONT
*
* Typedef: AlumString
* A shorthand for ALLEGRO_USTR
*
* Typedef: AlumColor
* A shorthand for ALLEGRO_COLOR
*/
#define AlumBitmap  ALLEGRO_BITMAP
#define AlumFont    ALLEGRO_FONT
#define AlumString  ALLEGRO_USTR
#define AlumColor   ALLEGRO_COLOR


/* Forward typedefs. */
typedef struct Alum_          Alum;
typedef struct AlumStyle_     AlumStyle;
typedef struct AlumWidget_    AlumWidget;
typedef struct AlumActions_   AlumActions;
typedef enum   AlumFlags_     AlumFlags;


/*
* Struct: AlumPoint
*
* Represents a point in 2D space, or a 2D size.
*/
typedef struct AlumPoint_ {
  float x, y;
} AlumPoint;

/*
* Struct: AlumBox
*
* Represents a rectangular box.
*/
typedef struct AlumBox_ {
  AlumPoint at;
  AlumPoint size;
} AlumBox;


/* Struct: AlumStyle. 
* Describes the style of a widget.
*/
struct AlumStyle_ {
  AlumColor     forecolor;
  AlumColor     backcolor;
  AlumFont    * font;
  AlumBitmap  * background;
};



/* Enum: AlumFlags. 
* Flags for a widget. A widget can be : visible or not
* listening to input not (if it  invisble and not listening 
* input it's inactive, otherwise active) 
* it can have focus or not (no focus listens to input 
* but normally ignores it, focus accepts input).
* Selected is for checkboxes that are selected or buttons that are down, etc.
*/
enum AlumFlags {
  ALUM_VISIBLE    = 1,
  ALUM_LISTENING  = 2,
  ALUM_ACTIVE     = 3,
  ALUM_FOCUSED    = 4,
  ALUM_SELECTED   = 8,
};

/* Flag testing macros. */

/** Checks if flag is set.
Flag will be evaluated 2 times so must be a constant or a variable that is 
only read. 
*/
#define ALUM_FLAG_P(WIDGET, FLAG) (((WIDGET)->flag&(FLAG))==(FLAG))
#define ALUM_VISIBLE_P(WIDGET, FLAG)    ALUM_FLAG_P(WIDGET, ALUM_VISIBLE)
#define ALUM_LISTENING_P(WIDGET, FLAG)  ALUM_FLAG_P(WIDGET, ALUM_LISTENING)
#define ALUM_ACTIVE_P(WIDGET, FLAG)     ALUM_FLAG_P(WIDGET, ALUM_ACTIVE)
#define ALUM_FOCUSED_P(WIDGET, FLAG)    ALUM_FLAG_P(WIDGET, ALUM_FOCUSED)
#define ALUM_SELECTED_P(WIDGET, FLAG)   ALUM_FLAG_P(WIDGET, ALUM_SELECTED)

/* Enum: AlumReply: the reply of an Action. */
enum AlumReply { 
  ALUM_REPLY_ERROR     = -1,
  ALUM_REPLY_IGNORE    =  1,
  ALUM_REPLY_OK        =  2,
  ALUM_REPLY_CLOSE     =  3,
  ALUM_REPLY_REDRAW    =  4,
  ALUM_REPLY_REDRAWME  =  5,
  ALUM_REPLY_ASKFOCUS  =  6,
};

/** A message sent to a Widget. */
enum AlumMessage {
  ALUM_MESSAGE_INIT         = 1,
  ALUM_MESSAGE_DONE         = 2,
  ALUM_MESSAGE_DRAW         = 3,
  ALUM_MESSAGE_UPDATE       = 4,
  ALUM_MESSAGE_ACTIVATE     = 5,
  ALUM_MESSAGE_CLICK        = 6,
  ALUM_MESSAGE_DOUBLECLICK  = 7,
  ALUM_MESSAGE_KEY          = 8,
  ALUM_MESSAGE_TEXT         = 9,
  ALUM_MESSAGE_WANTFOCUS    = 10,
  ALUM_MESSAGE_GOTFOCUS     = 11,
  ALUM_MESSAGE_LOSTFOCUS    = 12,
  ALUM_MESSAGE_GOTMOUSE     = 13,
  ALUM_MESSAGE_LOSTMOUSE    = 14,
  ALUM_MESSAGE_IDLE         = 15,
  ALUM_MESSAGE_RADIO        = 16,
  ALUM_MESSAGE_WHEEL        = 17,
  ALUM_MESSAGE_USER         = 24,
  ALUM_MESSAGE_MAX          = 32,
};


/** Message data sent to a widget. */
typedef struct AlumMessageData_ {
  ALLEGRO_EVENT * event;
  double          dt;
} AlumMessageData;

/** Action to take on receiving a message
*/
typedef int AlumAction(AlumWidget * self, int message, va_list args);


/** Widget method table */
struct AlumActions_ {
  AlumAction * acts[ALUM_MESSAGE_MAX];
};


/* 

Widgets are individual parts of the UI.  
As a simplification, Widgets are considered to occupy "panes" ordered 
in the user interface from back to front. Buttons may seem to be owned by the underlying dialog window, but in fact they are not.

A note on pointer ownership: the pointers to font and image in style
are NOT cleaned up, since style is intended to be mostly a shallow copy in which
font and background image are repeated many times.
*/
struct AlumWidget_ {
  /* Methods           */
  AlumActions   * acts;
  /* Bounds, this is a rectangular box. */
  AlumBox         bounds;
  /* Styling elements. */
  AlumStyle       style;
  /* Widget elements: */
  /* Unique ID. */
  int id;
  /* Flags (active, disabled, etc) */
  int flags;
  /* Priority of widget */
  int z;
  /* Index of widget in parent UI manager. */
  int index;
};


/** Struct: Alum
* The core UI Manager.
*/
struct Alum_ {
  AlumWidget * mouse_on;
  AlumWidget * focus_on;
  AlumWidget * joy_on;
  int click_wait;
  int amount;
  AlumWidget **widgets;
};




#endif // ALUM_H_INCLUDED
