#ifndef ALUM_H_INCLUDED
#define ALUM_H_INCLUDED

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#include "bad.h"

/*
How to do the event handling? 
1) 
Idea: Listeners. In essence, the UI maintains lists for every kind of event
of all widgets that are interested in that event. 
Pro: Faster. Less set up work than bubbling. 
Con: Need to connect listeners. Uses more memory. More difficult for a widget 
that wants to listen only /sometimes/. 

2) 
Idea: Classic bubbling. In essence, the widgets have table/array/tree 
of events they support and the UI sends the events to all widgets
in some predefined order (bubbling). If a widget accepts the event
and "swallows" it , the UI doesn't send the event to widgets after the 
accepting widget, otherwise the event passes over the whole list.
Pro: Less space needed. No need to connect listeners. Widgets can 
handle events flexibly. 
Con: Harder to set up depending on internal structure of supported events.

3)
Allegro 4 dialog style. Like bubbling, but every widget only has one event
handling function pointer that gets passed all events. It uses a big switch then
to decide how to handle the event.
Pro: Least space used, conceptually simple. 
Con: Lots of repeated and possibly redundant big switches in the widget code.

4) 
Idea: Many specific function pointers. Like bubbling of Allegro 4,
but in stead of only one function pointer, there are several, each 
specific for every kind of event, but they are not in a table,
but simply named straightforwardly. 
Pro: Only one or two big switches needed. Should be decently fast, conceptually
quite simple.
Con: Uses a bit of space for the function pointers. Need a switch to 
dynamically look up the correct function pointer for an event.

5) Idea: Composition. How to do this? 

6) Idea: big senders. Like 1, but maintain a single search structure for 
the UI sender, probably under the form of a lookup tree. This tree stores   
triples of message ID, component and function pointer, and must be set up
so the same component can be stored many times in that single tree.
Pro: Actually simpler conceptually since there is a single point of 
registration. 
Con: Complex search structure, probably needs dynamic memory allocation.

Even more generally, when I reflect on event-based programming, the problem is 
to look up what action(s) to undertake depending on the incoming event 
and the states of all the components. In essence, a GUI is a state
machine, however, one where the state is stored in many different components,
and where possible states can be added as a result of incoming input. 

Deciding how to look up the action to take is deciding what "search
structure" to use to find the function or function pointer that will perform
the correct action. 

Since it's not clear which approach has the advantage, I will start out with
the classic bubbling style, with virtual tables, but think of ways to write it
in a style that is as user-friendly as is possible.

*/


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
typedef struct Alum_                   Alum;
typedef struct AlumStyle_              AlumStyle;
typedef struct AlumListener_           AlumListener;
typedef struct AlumSender_             AlumSender;
typedef struct AlumWidget_             AlumWidget;
typedef struct AlumActions_            AlumActions;
typedef enum   AlumFlags_              AlumFlags;
typedef enum   AlumMessageType_        AlumMessageType;
typedef enum   AlumReply_              AlumReply;

typedef struct AlumBasicMessage_       AlumBasicMessage;
typedef struct AlumDisplayMessage_     AlumDisplayMessage;
typedef struct AlumJoystickMessage_    AlumJoystickMessage;
typedef struct AlumKeyboardMessage_    AlumKeyboardMessage;
typedef struct AlumMouseMessage_       AlumMouseMessage;
typedef struct AlumTimerMessage_       AlumTimerMessage;



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


/* Struct: AlumStyle 
* Describes the style of a widget.
*/
struct AlumStyle_ {
  AlumColor     forecolor;
  AlumColor     backcolor;
  AlumFont    * font;
  AlumBitmap  * background;
  int           bordersize;
};



/* Enum: AlumFlags_. 
* Flags for a widget. A widget can be : visible or not
* listening to input not (if it  invisble and not listening 
* input it's inactive, otherwise active) 
* it can have focus or not (no focus listens to input 
* but normally ignores it, focus accepts input).
* Selected is for checkboxes that are selected or buttons that are down, etc.
*/
enum AlumFlags_ {
  ALUM_VISIBLE    = 1,
  ALUM_LISTENING  = 2,
  ALUM_ACTIVE     = 3,
  ALUM_FOCUSED    = 4,
  ALUM_SELECTED   = 8,
};

/* Flag testing macros. */

/* Checks if flag is set.
Flag will be evaluated 2 times so must be a constant or a variable that is 
only read. 
*/
#define ALUM_FLAG_P(WIDGET, FLAG) (((WIDGET)->flag&(FLAG))==(FLAG))
#define ALUM_VISIBLE_P(WIDGET, FLAG)    ALUM_FLAG_P(WIDGET, ALUM_VISIBLE)
#define ALUM_LISTENING_P(WIDGET, FLAG)  ALUM_FLAG_P(WIDGET, ALUM_LISTENING)
#define ALUM_ACTIVE_P(WIDGET, FLAG)     ALUM_FLAG_P(WIDGET, ALUM_ACTIVE)
#define ALUM_FOCUSED_P(WIDGET, FLAG)    ALUM_FLAG_P(WIDGET, ALUM_FOCUSED)
#define ALUM_SELECTED_P(WIDGET, FLAG)   ALUM_FLAG_P(WIDGET, ALUM_SELECTED)

/* Enum: AlumMessageType_ 
 * A type of message sent to a Widget. */
enum AlumMessageType_ {
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
  ALUM_MESSAGE_OTHER        = 31,
  ALUM_MESSAGE_MAX          = 32,
};

/* Enum: AlumReply_: the reply of to a message sent. */
enum AlumReply_ { 
  ALUM_REPLY_ERROR     = -1,
  ALUM_REPLY_IGNORE    =  1,
  ALUM_REPLY_OK        =  2,
  ALUM_REPLY_CLOSE     =  3,
  ALUM_REPLY_REDRAW    =  4,
  ALUM_REPLY_REDRAWME  =  5,
  ALUM_REPLY_ASKFOCUS  =  6,
};


/* Type: AlumReact.
* AlumReact is a function type that is used to handle  
* messages sent from a sender to a receiver.
*/
typedef 
int AlumReact(void * sender, void * receiver, int message, void * data);


/* Type: AlumListen.
* AlumListen is a function type that is used to send 
* messenges from an AlumSender to an AlumListener.
*/
typedef 
int AlumListen(AlumSender * sender, AlumListener * listener, 
               int message, void * data);

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
  AlumReact       * listen;
  int               last_result;
  /* The following are private and for use of the AlumSender. */
  BadList           list;
};


/* typedef struct AlumSenderNode_ AlumSenderNode; */

/* Struct: AlumSender
* A Sender sends messages to all listeners that are registered with it.
*/
struct AlumSender_ {
  /* A chain (doubly linked list) of AlumListener_ . */
  BadList * chain;
};


/* Messages that Alum may send to the widgets.  */
struct AlumBasicMessage_ { 
  int    type;
  void  *source;
  double timestamp;
};

struct AlumDisplayMessage_ { 
  struct AlumBasicMessage_ basic;
  int x, y;
  int width, height;
  int orientation;
};

struct AlumJoystickMessage_ { 
  struct AlumBasicMessage_ basic;
  void * joystick;
  int    stick;
  int    axis;
  float  pos;
  int    button;
};

struct AlumKeyboardMessage_ { 
  struct AlumBasicMessage_ basic;
  void        * display; 
  int           keycode;                 
  int           unichar;                 
  unsigned int  modifiers;     
  int           repeat;                 
};

struct AlumMouseMessage_ {
  struct AlumBasicMessage_ basic;
  void  * display;
  int x,  y,  z, w;
  int dx, dy, dz, dw;
  unsigned int button;
  float pressure;
};


struct AlumTimerMessage_ {
  struct AlumBasicMessage_ basic;
  int64_t count;
  double error;
};

typedef 
int AlumBasicAction(Alum * ui, AlumWidget * w, AlumBasicMessage * m);
typedef 
int AlumJoystickAction(Alum * ui, AlumWidget *, AlumJoystickMessage *m);
typedef 
int AlumKeyboardAction(Alum * ui, AlumWidget *, AlumKeyboardMessage *m);
typedef 
int AlumMouseAction(Alum * ui, AlumWidget *, AlumMouseMessage *m);
typedef 
int AlumTimerAction(Alum * ui, AlumWidget *, AlumTimerMessage *m);



/* Widget messenger table. */
struct AlumActions_ {
  AlumReact * reactions[ALUM_MESSAGE_MAX];
};



/* 
Struct: AlumWidget-

Widgets are individual parts of the UI.  
As a simplification, Widgets are considered to occupy "panes" ordered 
in the user interface from back to front. Buttons may seem to be owned by the 
underlying dialog window, but in fact they are not.

A note on pointer ownership: the pointers to font and image in style
are NOT cleaned up, since style is intended to be mostly a shallow copy in which
font and background image are repeated many times.


Widgets in an Alum UI are expected to be set up before the 
UI is displayed, and after that the UIs is best kept statical. To enable this,
most Widgets can be set to non-active to hide and disable interaction
with them.

*/
struct AlumWidget_ {
  /* Bounds, this is a rectangular box. */
  AlumBox         bounds;
  /* Styling elements. */
  AlumStyle       style;
    
  /* Doubly linked list of widgets, in drawing order. */
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


/* Struct: Alum.
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

/* Draws a filled rectangle at the given position with the given size. */
void alum_draw_slab(AlumBox box, AlumColor col);

/* Draws a rounded filled rectange at the given position with the given size. */
void alum_draw_roundslab(AlumBox box, int rx, int ry, AlumColor col);

/* Draws an open rectangle at the given position with the given size */
void alum_draw_box(AlumBox box, AlumColor col, int tt);

/* Draws a rounded rectangle at the given position with the given size */
void alum_draw_roundbox(AlumBox box, int rx, int ry, AlumColor col, int tt);
/* Draws a filled frame of the given thickness on the active bitmap.
* The outer size of the frame will be ww and hh.
* border color is fg, background color is bg. */
void alum_draw_frame(AlumBox box, int tt, AlumColor fg, AlumColor bg);

/* Draws a filled, rounded frame of the given thickness on the active bitmap.
* The rounding is auto calculated. The outer size of the frame will be 
* ww and hh.
* border color is fg, background color is bg.
*/
void alum_draw_roundframe(AlumBox box, int tt, AlumColor fg, AlumColor bg);


AlumListener * 
alumlistener_init(AlumListener * listener, AlumReact * react);

AlumListener * 
alumlistener_done(AlumListener * listener);

int 
alum_sendva(AlumSender * send, AlumListener * listener,
            int message, va_list args);

int 
alum_send(AlumSender * sender, AlumListener * listener, 
          int message, void * data);

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
alum_broadcast(AlumSender * self, int message, void * data);

AlumSender * 
alumsender_link(AlumSender * self, AlumListener * listener, 
                AlumReact * react);



#endif // ALUM_H_INCLUDED
