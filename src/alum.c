#include "alum.h"

typedef int AlumStringWidth(const AlumFont *f, AlumString const *ustr);

#ifdef COMMENT

/*
* Function: alum_ustr_getfirstline
*
* Gets the start and end of the first line of text from the ustr.
* If start points at a non-negative integer, it will ignore all characters in
* ustr before start to get the numbered line.
* 
* If a line could be formed, nonzero is returned and valuesare stored in 
* start and stop. 
* This function uses get_width to calculate the width of the text, 
* and break up the text into lines that will fit inside max_width. 
* Newline characters cause a hard return.
* The text will be preferentially broken on spaces, but if that is not possibe, 
* then it will be broken in the middle of a word. 
* if get_width is null, only newlines are taken into consideration.
* A valid AlumStringInfo(ALLEGRO_STRING_INFO) must be passed to help store the 
* info of the resulting AlumString, which must NOT be freed, since it merely 
* refers to the ustr passed in. 
*/

int
alum_ustr_getfirstline(AlumString const * ustr, int line, float maxwidth, 
AlumStringWidth * get_width, int * start, int * stop) {
  int pos, newstart, newstop;
  AlumStringInfo info;
  const AlumString * aid;
  if(!ustr)   return 0;
  if(!start)  return 0;
  if(!stop)   return 0;
  nowstart =  ((*start) >= 0) :  (*start) : 0;
  pos      =  nowstart;
  
  al_ustr_next(ustr, &pos);
  
  return 0;
  
}

#endif


/** Allegro event handler prototypes for the Alum UI Manager. */
   
int alum_handle_joystick_axis
(Alum * self, ALLEGRO_JOYSTICK_EVENT * event);

int alum_handle_joystick_button_down
(Alum * self, ALLEGRO_JOYSTICK_EVENT * event);

int alum_handle_joystick_button_up
(Alum * self, ALLEGRO_JOYSTICK_EVENT * event);

int alum_handle_joystick_configuration
(Alum * self, ALLEGRO_JOYSTICK_EVENT * event);

int alum_handle_keyboard_key_down
(Alum * self, ALLEGRO_KEYBOARD_EVENT * event);

int alum_handle_keyboard_key_char
(Alum * self, ALLEGRO_KEYBOARD_EVENT * event);

int alum_handle_keyboard_key_up
(Alum * self, ALLEGRO_KEYBOARD_EVENT * event);

int alum_handle_mouse_axes
(Alum * self, ALLEGRO_MOUSE_EVENT * event);

int alum_handle_mouse_button_down
(Alum * self, ALLEGRO_MOUSE_EVENT * event);

int alum_handle_mouse_button_up
(Alum * self, ALLEGRO_MOUSE_EVENT * event);

int alum_handle_mouse_enter_display
(Alum * self, ALLEGRO_MOUSE_EVENT * event);

int alum_handle_mouse_leave_display
(Alum * self, ALLEGRO_MOUSE_EVENT * event);

int alum_handle_mouse_warped
(Alum * self, ALLEGRO_MOUSE_EVENT * event);

int alum_handle_timer
(Alum * self, ALLEGRO_TIMER_EVENT * event);

int alum_handle_display_resize
(Alum * self, ALLEGRO_DISPLAY_EVENT * event);

int alum_handle_display_expose
(Alum * self, ALLEGRO_DISPLAY_EVENT * event);

int alum_handle_display_close
(Alum * self, ALLEGRO_DISPLAY_EVENT * event);

int alum_handle_display_found
(Alum * self, ALLEGRO_DISPLAY_EVENT * event);

int alum_handle_display_lost
(Alum * self, ALLEGRO_DISPLAY_EVENT * event);

int alum_handle_display_switch_in
(Alum * self, ALLEGRO_DISPLAY_EVENT * event);

int alum_handle_display_switch_out
(Alum * self, ALLEGRO_DISPLAY_EVENT * event);

int alum_handle_display_orientation
(Alum * self, ALLEGRO_DISPLAY_EVENT * event);


int alum_handle_joystick_axis
(Alum * self, ALLEGRO_JOYSTICK_EVENT * event) {
  return ALUM_REPLY_IGNORE;
}

int alum_handle_joystick_button_down
(Alum * self, ALLEGRO_JOYSTICK_EVENT * event) {
  return ALUM_REPLY_IGNORE;
}

int alum_handle_joystick_button_up
(Alum * self, ALLEGRO_JOYSTICK_EVENT * event) {
  return ALUM_REPLY_IGNORE;
}

int alum_handle_joystick_configuration
(Alum * self, ALLEGRO_JOYSTICK_EVENT * event) {
  return ALUM_REPLY_IGNORE;
}

int alum_handle_keyboard_key_down
(Alum * self, ALLEGRO_KEYBOARD_EVENT * event) {
  return ALUM_REPLY_IGNORE;
}

int alum_handle_keyboard_key_char
(Alum * self, ALLEGRO_KEYBOARD_EVENT * event) {
  return ALUM_REPLY_IGNORE;
}

int alum_handle_keyboard_key_up
(Alum * self, ALLEGRO_KEYBOARD_EVENT * event) {
  return ALUM_REPLY_IGNORE;
}

int alum_handle_mouse_axes
(Alum * self, ALLEGRO_MOUSE_EVENT * event) {
  return ALUM_REPLY_IGNORE;
}

int alum_handle_mouse_button_down
(Alum * self, ALLEGRO_MOUSE_EVENT * event) {
  return ALUM_REPLY_IGNORE;
}

int alum_handle_mouse_button_up
(Alum * self, ALLEGRO_MOUSE_EVENT * event) {
  return ALUM_REPLY_IGNORE;
}

int alum_handle_mouse_enter_display
(Alum * self, ALLEGRO_MOUSE_EVENT * event) {
  return ALUM_REPLY_IGNORE;
}

int alum_handle_mouse_leave_display
(Alum * self, ALLEGRO_MOUSE_EVENT * event) {
  return ALUM_REPLY_IGNORE;
}

int alum_handle_mouse_warped
(Alum * self, ALLEGRO_MOUSE_EVENT * event) {
  return ALUM_REPLY_IGNORE;
}

int alum_handle_timer
(Alum * self, ALLEGRO_TIMER_EVENT * event) {
  return ALUM_REPLY_IGNORE;
}


int alum_handle_display_resize
(Alum * self, ALLEGRO_DISPLAY_EVENT * event) {
  return ALUM_REPLY_IGNORE;
}

int alum_handle_display_expose
(Alum * self, ALLEGRO_DISPLAY_EVENT * event) {
  return ALUM_REPLY_IGNORE;
}

int alum_handle_display_close
(Alum * self, ALLEGRO_DISPLAY_EVENT * event) {
  return ALUM_REPLY_IGNORE;
}

int alum_handle_display_found
(Alum * self, ALLEGRO_DISPLAY_EVENT * event) {
  return ALUM_REPLY_IGNORE;
}

int alum_handle_display_lost
(Alum * self, ALLEGRO_DISPLAY_EVENT * event) {
  return ALUM_REPLY_IGNORE;
}

int alum_handle_display_switch_in
(Alum * self, ALLEGRO_DISPLAY_EVENT * event) {
  return ALUM_REPLY_IGNORE;
}

int alum_handle_display_switch_out
(Alum * self, ALLEGRO_DISPLAY_EVENT * event) {
  return ALUM_REPLY_IGNORE;
}

int alum_handle_display_orientation
(Alum * self, ALLEGRO_DISPLAY_EVENT * event) {
  return ALUM_REPLY_IGNORE;
}



#define DO_REACT(SELF, EVENT, MEMBER, HANDLER)              \
  do {                                                      \
    return alum_handle_##HANDLER(SELF, &((EVENT)->MEMBER)); \
  } while(0)

/* React to an Allegro event. */
int alum_handle_event(Alum * self, ALLEGRO_EVENT * event) {
  switch(event->type) {
    case ALLEGRO_EVENT_JOYSTICK_AXIS:
      DO_REACT(self, event, joystick, joystick_axis);
    case ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN:
      DO_REACT(self, event, joystick, joystick_button_down);
    case ALLEGRO_EVENT_JOYSTICK_BUTTON_UP:
      DO_REACT(self, event, joystick, joystick_button_up);
    case ALLEGRO_EVENT_JOYSTICK_CONFIGURATION:
      DO_REACT(self, event, joystick, joystick_configuration);
    case ALLEGRO_EVENT_KEY_DOWN:
      DO_REACT(self, event, keyboard, keyboard_key_down);
    case ALLEGRO_EVENT_KEY_CHAR:
      DO_REACT(self, event, keyboard, keyboard_key_char);
    case ALLEGRO_EVENT_KEY_UP:
      DO_REACT(self, event, keyboard, keyboard_key_up);   
    case ALLEGRO_EVENT_MOUSE_AXES:
      DO_REACT(self, event, mouse, mouse_axes);
    case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
      DO_REACT(self, event, mouse, mouse_button_down);
    case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
      DO_REACT(self, event, mouse, mouse_button_up);
    case ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY:
      DO_REACT(self, event, mouse, mouse_enter_display);
    case ALLEGRO_EVENT_MOUSE_LEAVE_DISPLAY:
      DO_REACT(self, event, mouse, mouse_leave_display);
    case ALLEGRO_EVENT_MOUSE_WARPED:
      DO_REACT(self, event, mouse, mouse_warped);
    case ALLEGRO_EVENT_TIMER:
      DO_REACT(self, event, timer, timer);
    case ALLEGRO_EVENT_DISPLAY_EXPOSE:
      DO_REACT(self, event, display, display_expose);
    case ALLEGRO_EVENT_DISPLAY_RESIZE:
      DO_REACT(self, event, display, display_resize);
    case ALLEGRO_EVENT_DISPLAY_CLOSE:
      DO_REACT(self, event, display, display_close);
    case ALLEGRO_EVENT_DISPLAY_LOST:
      DO_REACT(self, event, display, display_lost);
    case ALLEGRO_EVENT_DISPLAY_FOUND:
      DO_REACT(self, event, display, display_found);
    case ALLEGRO_EVENT_DISPLAY_SWITCH_IN:
      DO_REACT(self, event, display, display_switch_in);
    case ALLEGRO_EVENT_DISPLAY_SWITCH_OUT:
      DO_REACT(self, event, display, display_switch_out);
    case ALLEGRO_EVENT_DISPLAY_ORIENTATION:
      DO_REACT(self, event, display, display_orientation);
    default:
      return ALUM_REPLY_IGNORE;
  }
}


int alum_update(Alum * self, double dt) {
  return ALUM_REPLY_OK;
}

int alum_draw(Alum * self) {
  return ALUM_REPLY_OK;
}





























