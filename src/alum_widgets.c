#include "alum.h"

typedef struct AlumFrame_ AlumFrame;

struct AlumFrame_ {
  AlumWidget widget;
};

int alumframe_initactions(AlumActions * actions, AlumActions * parent) {
  return -1;
}

int alumframe_draw(void * from, void * to, int message, void * data) {
  AlumWidget * widget = (AlumWidget *) to;
  alum_draw_roundframe(widget->bounds, widget->style.bordersize,
                       widget->style.forecolor,
                       widget->style.backcolor);
  return ALUM_REPLY_OK;
}

typedef struct AlumLabel_ AlumLabel;

struct AlumLabel_ {
  AlumFrame frame;
  char    * text;
};

int alumlabel_draw(void * from, void * to, int message, void * data) {
  AlumWidget * widget = (AlumWidget *) to;
  AlumFrame  * frame  = bad_container(widget, AlumFrame, widget);
  AlumLabel  * label  = bad_container(frame , AlumLabel, frame);
  alumframe_draw(label, widget, ALUM_MESSAGE_DRAW, data);
  return ALUM_REPLY_OK;
}




#ifdef _COMMENT

#define ALUM_CONSOLE_MAX 256


/* A console is a console for command-line interaction and error display. When it's active it captures all input (as long as it's active) */
struct AlumConsole_ {
  AlumWidget  widget;
  
  AlumString * input;
  /** Ring buffer of the lines on the console. */
  AlumString * lines[ALUM_CONSOLE_MAX];
  AlumString * first;
  AlumString * last;
  int     count;
  int     max;
  int     start;
  int     charw;
  int     cursor;
  char  * buf;
  AlumMessenger * command; // called when a command has been entered, if set.
  void * command_data;     // command data.
};


/** Sets the console's command function and data. */
void alumconsole_command_(AlumConsole * self, AlumMessenger * command, 
                          void * data) {
  self->command      = command;
  self->command_data = data;
}

/** Let the console perform a command if possible. returns nonzero on error,
zero if OK. */
int alumconsole_docommand(AlumConsole * self, const char * text) {
  if(!self) return -1;
  if(!self->command) return -2;
  return self->command(self, text, self->command_data);
}



/** Adds a line text from a sized buffer to the console. */
int alumconsole_addbuf(AlumConsole * self, const char * str, size_t size) {
  USTR * storestr;
  if(!self) return -1;
  storestr =  al_ustr_new_from_buffer(str, size);
  
  /** Clean up previous string if any 
  
  */
  if(self->lines[self->last]) { 
    al_ustr_free(self->lines[self->last]);
  }
  
  self->lines[self->last] = storestr;
  self->last++;
  if (self->last >= ALUM_CONSOLE_MAX) {
    self->last = 0;   
    self->first++;
  }
  if (self->first >= ALUM_CONSOLE_MAX) {
    self->first = 0;
  }
  self->count++;
  if (self->count > ALUM_CONSOLE_MAX) {
    self->count = ALUM_CONSOLE_MAX
  }

  return self->count;
}

/** Puts a string on the console .*/
int alumconsole_puts(AlumConsole * self, const char * str) {
  int index;
  int size     = strlen(str);
  int leftsize = size;
  int lines = 0;
  for (index = 0; index < size; 
       index += self->charw, 
       leftsize -= (self->charw) ) {
    int copysize = (leftsize > self->charw ? self->charw : leftsize);
    alumconsole_addstr(self, str + index, copysize);
    lines++;
  }
  return lines;
} 


/** Draws a console. */
void alumconsole_draw(Widget * widget) {
  AlumConsole * self ;
  Font * font    ;
  Color color    ;
  int high, linehigh, index, x, y, index;
  if (!widget_visible(widget)) return;
  
  self  = ALUM_CONTAINER(widget, AlumConsole, widget);
  font  = widget_font(widget);
  color = widget_forecolor(widget);
  
  alum_drawroundframe(widget);
  high        = widget_h(widget) - 10;
  x           = widget_x(widget) +  5;
  y           = widget_y(widget) -  5;
  linehigh    = font_lineheight(font);
  // skip start lines (to allow scrolling backwards) 
  for (skip = self->start; now && (skip > 0); skip --) {
    now = lilis_next(now); // move to next line.
  }
  for (index = high-linehigh; index > 0; index -= linehigh) {
    USTR * textstr;
    if(!now) break;
    textstr = (USTR *) lilis_data(now);
    if(textstr) {
      font_drawstr(font, color, x, y + index, 0, textstr);
    }
    now = lilis_next(now);
  }
  // draw input string
  font_drawstr(font, color, x, y + high - linehigh, 0, self->input);
  // draw start for debugging
  al_draw_textf(font, color, x, y, 0, "start: %d", self->start);
  
}

/** Activates or deactivates the console. */
void alumconsole_active_(AlumConsole * self, int active) {
  if(!self) return;
  widget_active_(&self->widget, active);
}

/** Returns nonzero if console is active zero if not. */
int alumconsole_active(AlumConsole * self) {
  if(!self) return 0;
  return widget_active(&self->widget);
}

/** scrolls the console 1 step in the given direction. */
int alumconsole_scroll(AlumConsole * self, int direction) {
  if((!self) || (!direction)) return FALSE;
  if(direction < 0) self->start--;
  if(direction > 0) self->start++;
  self->start = (self->start < 1) ? 0 : self->start;
  self->start = (self->start > self->max) ? self->max : self->start;
  return WIDGET_HANDLE_OK;
}


/* Key input event handler for console. */
int alumconsole_handle_keychar(AlumConsole * self, ALLEGRO_EVENT * event) { 
  int ch = event->keyboard.unichar;
  int kc = event->keyboard.keycode;
  switch(kc) {
    // ignore the start-console key
    case ALLEGRO_KEY_F1   : return WIDGET_HANDLE_OK;
    case ALLEGRO_KEY_PGUP : return alumconsole_scroll(self, 1);
    case ALLEGRO_KEY_PGDN : return alumconsole_scroll(self, -1);
    case ALLEGRO_KEY_BACKSPACE:
      // remove last character typed.
      al_ustr_remove_chr(self->input, al_ustr_offset(self->input, -1));
      return WIDGET_HANDLE_OK;
    break;    
    case ALLEGRO_KEY_ENTER: {
      const char * command = al_ustr_c(self->input);
      // execute command
      if(alumconsole_docommand(self, command)) { 
        alumconsole_puts(self, "Error in running comand");
        alumconsole_puts(self, command);
      }
      al_ustr_truncate(self->input, 0);
      // empty string by truncating it
      return WIDGET_HANDLE_OK;
      }
    case ALLEGRO_KEY_ESCAPE:
      alumconsole_active_(self, false); 
      // disable console if esc is pressed.
      return WIDGET_HANDLE_OK;
    default: break;
  }
  
  al_ustr_appendch(self->input, ch);
  return WIDGET_HANDLE_OK;
}


/* Mouse axe event handler for console */
int alumconsole_handle_mouseaxes(AlumConsole * self, ALLEGRO_EVENT * event) { 
  int z = event->mouse.dz;
  // only capture mouse scroll wheel...
  if(z == 0) return WIDGET_HANDLE_IGNORE;
  if(z < 0) return alumconsole_scroll(self, -1);
  if(z > 0) return alumconsole_scroll(self, +1);
  return WIDGET_HANDLE_OK;
}


/** Let the console handle allegro events. Returns 0 if event was consumed,
postoive if not, and negative on error. */

int alumconsole_handle(Widget * widget, ALLEGRO_EVENT * event) { 
  AlumConsole * self = (AlumConsole *) widget;
  if(!widget) return WIDGET_HANDLE_ERROR;
  if(!widget_active(widget)) return WIDGET_HANDLE_IGNORE;
  
  
  switch(event->type) {
    case ALLEGRO_EVENT_KEY_DOWN:
      return WIDGET_HANDLE_OK;
    case ALLEGRO_EVENT_KEY_UP:
      return WIDGET_HANDLE_OK;
    case ALLEGRO_EVENT_KEY_CHAR:
      return alumconsole_handle_keychar(self, event);
    case ALLEGRO_EVENT_MOUSE_AXES:
      return alumconsole_handle_mouseaxes(self, event);
  }
  
  return WIDGET_HANDLE_IGNORE;
}

/* Global console method table. */
static WidgetMetab alumconsole_metab_ = {
  alumconsole_free,
  alumconsole_done,
  alumconsole_draw,
  alumconsole_handle,
  NULL,
};


/** Cleans up a console */
void * alumconsole_done(void * widget) {
  AlumConsole * self = (AlumConsole *) widget;
  Lilis * aid;
  if(!self) return NULL;
  for (aid = self->lines; aid; aid = lilis_next(aid))  {
    al_ustr_free((USTR *)lilis_data(aid));
  }
  self->lines   = lilis_free(self->lines);
  self->buf     = mem_free(self->buf);
  al_ustr_free(self->input);
  self->input   = NULL;
  return widget;
}


/** Deallocates a console. */
void * alumconsole_free(void * widget) {
  AlumConsole * self = (AlumConsole *) widget;
  alumconsole_done(self);
  mem_free(self);
  return NULL;
}

/** Allocates a console. */
AlumConsole * alumconsole_alloc() {
  return STRUCT_ALLOC(AlumConsole);
}

#define CONSOLE_MAX 1000

/** Initializes a console. */
AlumConsole * alumconsole_initall(AlumConsole * self, int id, Rebox bounds, Style style) {
  if(!self) return NULL;
  if(!widget_initall(&self->widget, id, &alumconsole_metab_, bounds, style)) { 
    return NULL;
  }
  self->lines = lilis_newempty();
  if(!self->lines) return NULL;
  self->last  = lilis_addnew(self->lines, NULL);
  if(!self->last) { alumconsole_done(self); return NULL; }
  widget_active_(&self->widget, FALSE);
  self->count = 0;
  // max MUST be at least 2, 3 to see anything...
  self->max   = 1000; 
  self->start = 0;
  self->charw = 80; 
  self->buf   = mem_alloc(self->charw + 1);
   // one extra for NULL at end . 
  if(!self->buf) { alumconsole_done(self); return NULL; }
  self->input = al_ustr_new("");
  self->cursor= 0;
  if(!self->input) { alumconsole_done(self); return NULL; }
  self->command      = NULL;
  self->command_data = NULL;
  return self;
}

/** Initializes a new console. */
AlumConsole * alumconsole_new(int id, Rebox bounds, Style style) {
  AlumConsole * self = alumconsole_alloc();
  if(!alumconsole_initall(self, id, bounds, style)) {
    return alumconsole_free(self);
  }
  return self;
}


#endif