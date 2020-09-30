CC = vc
CFLAGS = -c99 +aos68k -I$(NDK_INC)
LDFLAGS = -lamiga -lauto

all:
	$(CC) $(CFLAGS) -o graphics main.c $(LDFLAGS)

clean:
	rm graphics
