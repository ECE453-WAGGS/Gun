ARCH=arm
CC=arm-linux-gnueabihf-gcc
IDIR =../include
CFLAGS=-I$(IDIR)
LIBS=
ODIR=../obj

_DEPS = 
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = zed_signal_app.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

zed_signal_app: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f zed_signal_app $(ODIR)/*.o *~ core $(INCDIR)/*~ 
