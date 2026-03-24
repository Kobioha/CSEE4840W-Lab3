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

#define RADIUS 30
#define H_MAX 640
#define V_MAX 480

int vga_ball_fd;

/* Read and print the background color */
void print_background_color() {
  vga_ball_arg_t vla;
  
  if (ioctl(vga_ball_fd, VGA_BALL_READ_BACKGROUND, &vla)) {
      perror("ioctl(VGA_BALL_READ_BACKGROUND) failed");
      return;
  }
  printf("%02x %02x %02x\n",
	 vla.background.red, vla.background.green, vla.background.blue);
}

/* Set the background color */
void set_background_color(const vga_ball_color_t *c)
{
  vga_ball_arg_t vla;
  vla.background = *c;
  if (ioctl(vga_ball_fd, VGA_BALL_WRITE_BACKGROUND, &vla)) {
      perror("ioctl(VGA_BALL_SET_BACKGROUND) failed");
      return;
  }
}

/* Set the Ball Position */
void set_position(const vga_ball_pos_t *c)
{
	vga_ball_arg_t vla;
	vla.position = *c;
	if (ioctl(vga_ball_fd, VGA_BALL_WRITE_POSITION, &vla)) {
		perror("ioctl(VGA_BALL_SET_POSITION) failed");
		return;
	}
}

int main()
{
  vga_ball_arg_t vla;
  int i;
  static const char filename[] = "/dev/vga_ball";
	/* Direction of ball */
	int dx = 1;
	int dy = 1;

  static const vga_ball_color_t colors[] = {
    { 0xff, 0x00, 0x00 }, /* Red */
    { 0x00, 0xff, 0x00 }, /* Green */
    { 0x00, 0x00, 0xff }, /* Blue */
    { 0xff, 0xff, 0x00 }, /* Yellow */
    { 0x00, 0xff, 0xff }, /* Cyan */
    { 0xff, 0x00, 0xff }, /* Magenta */
    { 0x80, 0x80, 0x80 }, /* Gray */
    { 0x00, 0x00, 0x00 }, /* Black */
    { 0xff, 0xff, 0xff }  /* White */
  };

# define COLORS 9

  printf("VGA ball Userspace program started\n");

  if ( (vga_ball_fd = open(filename, O_RDWR)) == -1) {
    fprintf(stderr, "could not open %s\n", filename);
    return -1;
  }

	vga_ball_pos_t ball_pos;
	ball_pos.x = 0xc3;
	ball_pos.y = 0xc3;

	set_position(&ball_pos);

	while (1) {
		if (ball_pos.x > H_MAX - RADIUS || ball_pos.x < RADIUS) {
			dx = -dx;
		}
		if (ball_pos.y > V_MAX - RADIUS || ball_pos.y < RADIUS) {
			dy = -dy;
		}

		set_position(&ball_pos);

		printf("ball_pos: x=%d. y=%d\n", ball_pos.x, ball_pos.y);
		fflush(stdout);

		ball_pos.x += dx;
		ball_pos.y += dy;

		usleep(10000);
	}

  printf("initial state: ");
  print_background_color();

  for (i = 0 ; i < 24 ; i++) {
    set_background_color(&colors[i % COLORS ]);
    print_background_color();
    usleep(400000);
  }
  printf("VGA BALL Userspace program terminating\n");
  return 0;
}
