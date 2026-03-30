#ifndef _VGA_BALL_H
#define _VGA_BALL_H

#include <linux/ioctl.h>

typedef struct {
  __u16 x;
  __u16 y;
} vga_ball_coord_t;

#define VGA_BALL_MAGIC 'q'

/* ioctls and their arguments */
#define VGA_BALL_SET_COORD _IOW(VGA_BALL_MAGIC, 1, vga_ball_coord_t)

#endif
