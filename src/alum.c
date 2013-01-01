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


