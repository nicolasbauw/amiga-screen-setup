CC = vc
CFLAGS = -c99 +aos68km -I$(NDK_INC)
LDFLAGS = -lamiga -lauto

all:
	$(CC) $(CFLAGS) -o screen main.c $(LDFLAGS)

clean:
	rm screen
