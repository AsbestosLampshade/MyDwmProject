#include"input_collector.h"

static volatile sig_atomic_t stop=0;

struct ev_return print_events(int fd)
{
	struct input_event ev[64];
	int i, rd;
	i=0;
	fd_set rdfs;
	struct ev_return out;
	//Check if fd exists or not
	if(fcntl(fd, F_GETFD) == -1 || errno == EBADF){
		out.type=0;
		out.code=0;
		out.value=0;
	}
	FD_ZERO(&rdfs);
	FD_SET(fd, &rdfs);

	select(fd + 1, &rdfs, NULL, NULL, NULL);
	rd = read(fd, ev, sizeof(ev));

	if (rd < (int) sizeof(struct input_event)) {
		printf("expected %d bytes, got %d\n", (int) sizeof(struct input_event), rd);
		perror("\nevtest: error reading");
		return out;

	}
	out.type=ev[i].type;
	out.code=ev[i].code;
	out.value=ev[i].value;
	ioctl(fd, EVIOCGRAB, (void*)0);
	return out;
}

int setup_controller(){
	
	char *filename;
	int devnum;
	char *device;
	device=getenv("ALF_CONTROLLER_DEV_NUM");
	if(device==NULL){
		printf("No such device found");
		return -1;
	}
	devnum=atoi(device);
	asprintf(&filename, "/dev/input/event%d",
		 devnum);
	printf("%s",filename);
	int fd = open(filename, O_RDONLY);
	if (fd < 0) {
		printf("ERROR: %s", strerror(errno));
		return -1;
	}
	return fd;
}

int interpret_output(struct ev_return inp){
	if(inp.type==0)
		return 0;
	else if(inp.type==3){
		if(inp.code==5)
			return 1;
		else
			return 2;
	}
	else if(inp.type==1){
		if(inp.code==304)
			return 3;
		else if(inp.code==305)
			return 4;
		else if(inp.code==307)
			return 5;
		else if(inp.code==308)
			return 6;
		else if(inp.code==310 && inp.value==1)
			return 11;
		else if(inp.code==311 && inp.value==1)
			return 12;
	}
	else if(inp.type==3){
		if(inp.code==17){
			if(inp.value==-1)
				return 7;
			else
				return 8;
		}
		else if(inp.code==16){
			if(inp.value==-1)
				return 9;
			else 
				return 10;
		}
	}
	return -1;
}
