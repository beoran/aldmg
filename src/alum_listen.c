#include "alum.h"

/* This file implements listener and sender related functionality. */


/* Function: alumlistener_init
* Initializes an AlumListener
*/
AlumListener * 
alumlistener_init(AlumListener * listener, AlumMessenger * listen) {
  listener->listen      = listen;
  badlist_init(&listener->list);
  return listener;
}


/* Function: alumlistener_init
* Deintializes an AlumListener
*/
AlumListener * 
alumlistener_done(AlumListener * listener) {
  listener->listen      = NULL;
  badlist_init(&listener->list);
  return listener;
}


/* Function: alum_sendva
* Sends a message to the listener from the given sender
*/
int 
alum_sendva(AlumSender * sender, AlumListener * listener, 
            int message, va_list args) {
  if((!listener) && (!listener->listen)) return ALUM_REPLY_ERROR;
  return listener->listen(sender, listener, message, args);
}


/* Function: alum_send
* Sends a message to the listener from the given sender
*/
int 
alum_send(AlumSender * sender, AlumListener * listener, 
          int message, ...) {
  int result;
  va_list args;
  va_start(args, message);
  result = alum_sendva(sender, listener, message, args);
  va_end(args);
  return result;
}


/* Function: alumsender_init
* Initializes an AlumSender struct for use.
*/
AlumSender * 
alumsender_init(AlumSender * self) {
  if(!self) return NULL;
  self->chain = NULL;
  return self;
};

/* Function: alumsender_register
* Registers the given listener to listen to the given sender.
*/
AlumSender * 
alumsender_register(AlumSender * self, AlumListener * listener) {  
  AlumListener * oldnode = NULL;
  if((!self) || (!listener)) return NULL;
  if(self->chain) {
    self->chain = badlist_unshift(self->chain, &(listener->list));
  } else {
    self->chain = &(listener->list);
    badlist_init(self->chain);
  }
  return self;
};


int AlumListenerBadListCompare(BadList * one, BadList * two) {
  AlumListener * listener_one;
  AlumListener * listener_two; 
  listener_one = bad_container(one, AlumListener, list);
  listener_two = bad_container(one, AlumListener, list);
  if (listener_one == listener_two) return 0;
  return -1;
}

AlumSender * 
alumsender_unregister_node(AlumSender * self, AlumListener * listener) {
  BadList      * to_remove;
  AlumListener * listener_remove;
  if(self->chain) { 
    to_remove = badlist_search(self->chain, 
                               AlumListenerBadListCompare, &listener->list);
    if(to_remove) {
      if(to_remove == self->chain) {
        self->chain = badlist_next(to_remove);
      }
      badlist_unlink(to_remove);
    } else {
      return NULL;
    }
  }
  return self;
};


/* Function: alumsender_unregister
* Unregisters the given listener from the sender.
* This cleans up any memory used internally by the sender, but NOT the listener.
*/
AlumSender * 
alumsender_unregister(AlumSender * self, AlumListener * listener) {
  AlumListener * base, * aid;
  if ((!self) || (!listener)) return NULL;
  alumsender_unregister_node(self, listener);
  return NULL;
};

/* Function: alumsender_done
* Clean up an  AlumSender struct after use.
* This will call alumsender_unregister on every registered 
* listener, but does not free the listener itself.
*/
AlumSender * alumsender_done(AlumSender * self) {
  AlumListener * aid;
  if (!self) return NULL;
  while (self->chain) {
    alumsender_unregister_node(self, 
                               bad_container(self->chain, AlumListener, list));
  }
  return self;
};


/* Function: alum_broadcastva
* Broadcast a message from the given sender to all it's 
* registered listeners that are interested in that message.
*/
int
alum_broadcastva(AlumSender * self, int message, va_list args) {
  BadList * base, * aid;
  if (!self) return ALUM_REPLY_ERROR;
  base  = self->chain;
  for (aid = self->chain; aid ; aid = badlist_next(aid)) {
    AlumListener * listener;
    va_list args_copy;
    va_copy(args_copy, args);
    listener = bad_container(aid, AlumListener, list);
    int result = alum_sendva(self, listener, message, args_copy);
    listener->last_result = result;
  }
  return 0;
};


/* Function: alum_broadcast
* Broadcast a message from the given sender to all it's 
* registered listeners that are interested in that message.
*/
int
alum_broadcast(AlumSender * self, int message, ...) {
  int result;
  va_list args;
  va_start(args, message);
  result = alum_broadcastva(self, message, args);
  va_end(args);
  return result;
}



/* Function: alumsender_to
* Links a sender,  listener and listening function together in one go.
*/
AlumSender * 
alumsender_link(AlumSender * self, AlumListener * listener, 
                AlumMessenger * listen) {
  alumlistener_init(listener, listen);
  return alumsender_register(self, listener);
}

