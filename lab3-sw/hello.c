/*
 * Userspace program that communicates with the vga_ball device driver
 * through ioctls
 *
 * Stephen A. Edwards
 * Columbia University
 */

#include <stdio.h>
#include "vga_ball.h"
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define SCREEN_W  640
#define SCREEN_H  480
#define RADIUS    16

int vga_ball_fd;

void set_ball_position(int x, int y)
{
	vga_ball_coord_t coord;
	coord.x = x;
	coord.y = y;
	if (ioctl(vga_ball_fd, VGA_BALL_SET_COORD, &coord)) {
		perror("ioctl(VGA_BALL_SET_COORD) failed");
		return;
	}
}

int main()
{
  static const char filename[] = "/dev/vga_ball";
  int x = SCREEN_W / 2, y = SCREEN_H / 2;
  int dx = 3, dy = 2;

  printf("VGA ball Userspace program started\n");

  if ( (vga_ball_fd = open(filename, O_RDWR)) == -1) {
    fprintf(stderr, "could not open %s\n", filename);
    return -1;
  }

  set_ball_position(x, y);

  while (1) {
		  int next_x = x + dx;
		  int next_y = y + dy;

		  if (next_x - RADIUS < 0 || next_x + RADIUS >= SCREEN_W) {
			  dx = -dx;
			  next_x = x + dx;
		  }
		  if (next_y - RADIUS < 0 || next_y + RADIUS >= SCREEN_H) {
			  dy = -dy;
			  next_y = y + dy;
		  }

		  x = next_x;
		  y = next_y;
		  set_ball_position(x, y);

	  printf("ball_pos: x=%d, y=%d\n", x, y);
	  fflush(stdout);

	  usleep(20000); /* ~50 fps */
  }

  printf("VGA BALL Userspace program terminating\n");
  return 0;
}
