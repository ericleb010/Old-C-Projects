
CC	=	gcc
CFLAGS	=	-g -Wall


ALL_OBJS	=	shell-main.o shell-vars.o shell-pipes.o



all: shell

shell : $(ALL_OBJS)
	$(CC) $(CFLAGS) -o shell $(ALL_OBJS)

clean : 
	- rm -f $(ALL_OBJS)
	- rm -f shell
