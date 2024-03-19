#ifndef INPUT_COLLECTOR_H
#define INPUT_COLLECTOR_H

#define _GNU_SOURCE
#include<linux/input.h>
#include<sys/ioctl.h>
#include<string.h>
#include<errno.h>
#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<fcntl.h>

//Defines/Presets

#define RT 1
#define LT 2
#define A 3
#define B 4
#define X 5
#define Y 6
#define L_Up 7
#define L_Down 8
#define L_Left 9
#define L_Right 10
#define LB 11
#define RB 12

struct ev_return{
	int type;
	int code;
	int value;
};

int setup_controller();
struct ev_return print_events(int fd);
int interpret_output(struct ev_return);

#endif
