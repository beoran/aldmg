
#include "alum.h"

/* Additional drawing functions and wrappers for primitive drawing 
functionality. */

#ifdef COMMENT_
/** Draws an image on the current target bitmap according to a 9 part scaling
algorithm. This splits the bitmap in 9 parts, keeps the 4 corners unscaled, but
scales the 4 edges and the center according to the desired size.
The 4 corners should be rectangles of identical size corner_w by corner_h in
the original image. Pass a nonpositive number to have the corner sizes automatically calculated.
new_w and new_h are the new dimensions the new image should have.
This is useful for GUI backgrounds.
*/
void alum_image_blitscale9(Image * img, int xx, int yy,
                                  int new_w, int new_h,
                                  int corner_w, int corner_h) {

  int mid_src_w, mid_src_h, mid_dst_w, mid_dst_h;
  int left_x, right_x, middle_x;
  int dst_y, src_y, part_w;
  int src_w = image_w(img);
  int src_h = image_h(img);
  corner_w  = (corner_w > 0) ? (corner_w) :  src_w / 16;
  corner_h  = (corner_h > 0) ? (corner_h) :  src_h / 16;
  CLAMP_MAX(corner_w, corner_w, (new_w / 4));
  CLAMP_MAX(corner_h, corner_h, (new_h / 4));
  mid_src_w = src_w - (corner_w * 2);
  mid_src_h = src_h - (corner_h * 2);
  mid_dst_w = new_w - (corner_w * 2);
  mid_dst_h = new_h - (corner_h * 2);
  // draw top parts first  
  left_x    = xx; // left side
  right_x   = xx    + mid_dst_w + corner_w; // right side
  middle_x  = xx    + corner_w ; // middle
  dst_y     = yy; // y is he drawing location for the top 3 draws
  // width of the first corner and middle. the second corner starts here
  part_w    = src_w - corner_w;
  // draw, take from the top corner left of the image
  image_drawpart(img, 0, 0, corner_w, corner_h, left_x, dst_y, 0);
  // draw, take from the midle of the image
  image_drawscale(img, corner_w, 0, mid_src_w, corner_h,
                  middle_x, dst_y, mid_dst_w, corner_h, 0);
  // draw, take from the right corner of the image
  image_drawpart(img, part_w, 0, corner_w, corner_h, right_x, dst_y,  0);
  // now shift down, mutatis mutandis.
  dst_y     = yy + corner_h;
  src_y     = corner_h;
  // NOTE the middle left and right must be scaled, unlike the corners
  // that must be copied as they are.
  // draw, take from the middle corner left of the image, stretch vertically 
  image_drawscale(img, 0, src_y, corner_w , mid_src_h,
                  left_x, dst_y, corner_w , mid_dst_h, 0);
  // draw, take from the midle center of the image
  image_drawscale(img, corner_w, src_y, mid_src_w, mid_src_h,
                  middle_x, dst_y, mid_dst_w, mid_dst_h, 0);
  // draw, take from the right center of the image, stretch vertically
  image_drawscale(img, part_w,  src_y, corner_w, mid_src_h,
                  right_x, dst_y, corner_w , mid_dst_h, 0);
  
  dst_y     = yy + new_h - corner_h ; // + mid_dst_h;
  src_y     = src_w - corner_h;
  // draw, take from the lower corner left of the image
  image_drawpart(img, 0, src_y, corner_h, corner_w, left_x, dst_y, 0);
  // draw, take from the lower center of the image
  image_drawscale(img, corner_w,  src_y, mid_src_w, corner_h,
                  middle_x, dst_y, mid_dst_w, corner_h, 0);
  
  // draw, take from the lower corner of the image
  image_drawpart(img, part_w, src_y, corner_h, corner_w, right_x, dst_y,  0);
}

#endif

/*** Draws a filled rectangle at the given position with the given size */
void alum_draw_slab(AlumBox box, AlumColor col) {
  al_draw_filled_rectangle
  (box.at.x, box.at.y, box.at.x+box.size.x, box.at.y+box.size.y, col);
} 

/*** Draws a rounded filled rectange at the given position with the given size
*/
void alum_draw_roundslab(AlumBox box, int rx, int ry, AlumColor col) {
  al_draw_filled_rounded_rectangle
  (box.at.x, box.at.y, box.at.x+box.size.x, box.at.y+box.size.y, rx, ry, col);

} 


/*** Draws an open rectangle at the given position with the given size */
void alum_draw_box(AlumBox box, AlumColor col, int tt) {
  al_draw_rectangle
  (box.at.x, box.at.y, box.at.x+box.size.x, box.at.y+box.size.y, col, tt);
} 


/** Draws a rounded rectangle at the given position with the given size */
void alum_draw_roundbox(AlumBox box, int rx, int ry, AlumColor
col, int tt) {
  al_draw_rounded_rectangle
  (box.at.x, box.at.y, box.at.x+box.size.x, box.at.y+box.size.y, 
   rx, ry, col, tt);
}

/** Draws a filled frame of the given thickness on the active bitmap.
* The outer size of the frame will be ww and hh.
* border color is fg, background color is bg. */
void alum_draw_frame(AlumBox box, int tt, AlumColor fg, AlumColor bg)  {
  // Draw inner frame in bg color.
  alum_draw_slab(box, bg);
  // Draw outer frame in fg color with the given thickness.
  alum_draw_box(box, fg, tt);
}  
  
/** Draws a filled, rounded frame of the given thickness on the active bitmap.
* The rounding is autocalulated. The outer size of the frame will be ww and hh.
* border color is fg, background color is bg.
*/
void alum_draw_roundframe(AlumBox box, int tt, AlumColor fg, AlumColor bg)  {
  int rx = 4;
  int ry = 4;
  // draw inner frame in bg color.
  alum_draw_roundslab(box, rx, ry, bg);
  // draw outer frame in fg color with the given thickness.
  alum_draw_roundbox(box, rx, ry, fg, tt);
}




