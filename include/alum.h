#ifndef ALUM_H_INCLUDED
#define ALUM_H_INCLUDED

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#include "bad.h"

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
#define AlumBitmap      ALLEGRO_BITMAP
#define AlumFont        ALLEGRO_FONT
#define AlumString      ALLEGRO_USTR
#define AlumStringInfo  ALLEGRO_USTR_INFO
#define AlumColor       ALLEGRO_COLOR


/* Forward typedefs. */
typedef struct Alum_          Alum;
typedef struct AlumStyle_     AlumStyle;

typedef struct AlumListener_  AlumListener;
typedef struct AlumSender_    AlumSender;

typedef struct AlumWidget_    AlumWidget;

typedef struct AlumActions_   AlumActions;

typedef enum   AlumFlags_     AlumFlags;
typedef enum   AlumMessage_   AlumMessage;
typedef enum   AlumReply_     AlumReply;




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

/** A message sent to a Widget. */
enum AlumMessage_ {
  ALUM_MESSAGE_ALL          = 0,
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
  ALUM_MESSAGE_REGISTER     = 19,
  ALUM_MESSAGE_UNREGISTER   = 20,
  
  ALUM_MESSAGE_USER         = 24,
  ALUM_MESSAGE_MAX          = 32,
};

/* Enum: AlumReply: the reply of to a message sent. */
enum AlumReply_ { 
  ALUM_REPLY_ERROR     = -1,
  ALUM_REPLY_IGNORE    =  1,
  ALUM_REPLY_OK        =  2,
  ALUM_REPLY_CLOSE     =  3,
  ALUM_REPLY_REDRAW    =  4,
  ALUM_REPLY_REDRAWME  =  5,
  ALUM_REPLY_ASKFOCUS  =  6,
};


/* Type: AlumMessenger.
* AlumMessenger is a function type that is used to send 
* variable messenges from a sender to a receiver.
*/
typedef int AlumMessenger(void * sender, void * receiver,
                            int message, va_list args);


/* Type: AlumListen.
* AlumListen is a function type that is used to send 
* messenges from an AlumSender to an AlumListener.
*/
typedef int AlumListen(AlumSender * sender, AlumListener * listener, 
                        int message, va_list args);

/* Struct: AlumListener
* An AlumListener can be registered with one and only one AlumSender,
* but a sender can have several AlumListeners registered.
* To have the same struct listen to several AlumSenders, you'll need 
* several listeners, however these could have the same listen function.
* You may not modify the fields of this struct that start with _ manually,
* these are for the internal use of the AlumSender and the AlumListener,
* though you may read them to iterate to the next AlumListener for that sender.
* Rationale: the current design avoids the need for dynamic memory allocation.
*/
struct AlumListener_ {
  AlumMessenger   * listen;
  int               last_result;
  /* The following are private and for use of the AlumSender. */
  BadList           list;
};


/* typedef struct AlumSenderNode_ AlumSenderNode; */

/* Struct: AlumSender
* A Sender sends messages to all listeners that are registered with it.
*/
struct AlumSender_ {
  /** A chain (doubly linked list) of AlumListeners. */
  BadList * chain;
};

/** Widget messenger table. */
struct AlumActions_ {
  AlumMessenger * messengers[ALUM_MESSAGE_MAX];
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
  /* Bounds, this is a rectangular box. */
  AlumBox         bounds;
  /* Styling elements. */
  AlumStyle       style;
    
  /* Doubly list of widgets, in drawing order. */
  BadList         list;
  
  /* Methods           */
  AlumActions   * acts;
  
  /* Sender to connect to to get events from this widget. */
  AlumSender      sender;
  
  
  /* Widget elements: */
  /* Unique ID. */
  int id;
  /* Flags (active, disabled, etc) */
  int flags;
  /* Priority of widget, useful for sorting. */
  int z;
  
  
  
  
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
  /* Chain of doubly linked widgets, bottom-first. */
  BadList    * chain;
  /* Last, topmost widget in chain. */
  BadList    * top;
  /* array of senders to register to to receive respective messages. */
  AlumSender    senders[ALUM_MESSAGE_MAX];
  
};


typedef struct AlumConsole_ AlumConsole;



AlumListener * 
alumlistener_init(AlumListener * listener, AlumMessenger * listen);

AlumListener * 
alumlistener_done(AlumListener * listener);

int 
alum_sendva(AlumSender * send, AlumListener * listener,
            int message, int BadVar argv[]);

int 
alum_send(AlumSender * sender, AlumListener * listener, int message, ...);

AlumSender * 
alumsender_init(AlumSender * self);

AlumSender * 
alumsender_register(AlumSender * self, AlumListener * listener);


AlumSender * 
alumsender_unregister_node(AlumSender * self, AlumListener * listener);


AlumSender * 
alumsender_unregister(AlumSender * self, AlumListener * listener);

AlumSender * alumsender_done(AlumSender * self);

int
alum_broadcastva(AlumSender * self, int message, va_list args);

int
alum_broadcast(AlumSender * self, int message, ...);

AlumSender * 
alumsender_link(AlumSender * self, AlumListener * listener, 
                AlumMessenger * listen);



#endif // ALUM_H_INCLUDED
