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
#include <string.h>
#include <unistd.h>

#define SCREEN_W  640
#define SCREEN_H  480
#define RADIUS    16

int main()
{
  static const char filename[] = "/dev/vga_ball";
  int fd;
  int x = SCREEN_W / 2, y = SCREEN_H / 2;
  int dx = 3, dy = 2;

  printf("VGA Ball bouncing demo started\n");

  if ((fd = open(filename, O_RDWR)) == -1) {
    fprintf(stderr, "could not open %s\n", filename);
    return -1;
  }

  while (1) {
    x += dx;
    y += dy;

    if (x - RADIUS <= 0 || x + RADIUS >= SCREEN_W)
      dx = -dx;
    if (y - RADIUS <= 0 || y + RADIUS >= SCREEN_H)
      dy = -dy;

    vga_ball_coord_t coord = { .x = (__u16)x, .y = (__u16)y };
    if (ioctl(fd, VGA_BALL_SET_COORD, &coord) < 0) {
      perror("ioctl(VGA_BALL_SET_COORD) failed");
      break;
    }

    printf("ball_pos: x=%d, y=%d\n", x, y);
    fflush(stdout);

    usleep(20000); /* ~50 fps */
  }

  close(fd);
  return 0;
}
