# set the C compiler, for C++ use g++

# use this setting on most sane machines
CC = gcc

# use this setting on the engin elf machines
#CC = gcc296

# set the path to the include directory
INCDIR =../include

# set the flags for the C and C++ compiler to give lots of warnings
CFLAGS = -I$(INCDIR) -O2 -Wall -Wstrict-prototypes -Wnested-externs -Wmissing-prototypes -Wmissing-declarations
CPPFLAGS = $(CFLAGS)

# library tool defs
AR = ar
ARFLAGS = rv
RANLIB = ranlib

# path to the object file directory
ODIR = obj

# path to the lib directory
LIBDIR =.

# path to the bin directory
BINDIR =../bin

# put all of the relevant include files here
_DEPS = ppmIO.h color.h point.h line.h circle.h ellipse.h polyline.h list.h polygon.h  ppmMethods.h Image.h fract.h matrix.h view.h module.h drawState.h textreader.h bezier.h light.h plyRead.h

# convert them to point to the right place
DEPS = $(patsubst %,$(INCDIR)/%,$(_DEPS))

# put a list of all the object files (with .o endings)
_COMMON = ppmIO.o color.o point.o line.o circle.o ellipse.o polyline.o list.o polygon.o ppmMethods.o Image.o fract.o matrix.o view.o module.o drawState.o textreader.o bezier.o light.o plyRead.o

# convert them to point to the right place
COMMON = $(patsubst %,$(ODIR)/%,$(_COMMON))


# patterns for compiling source code
$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: %.C $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

# output library
OUTPUT_LIB = libimageIO.a

# make rules
all: $(OUTPUT_LIB)

$(OUTPUT_LIB) : $(COMMON) $(DEPS)
	rm -f $@
	$(AR) $(ARFLAGS) $@ $(COMMON)
	$(RANLIB) $@

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 

