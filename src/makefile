# set the C compiler, for C++ use g++
# use this setting on most sane machines
CC = gcc

# use this line on the engin linux machines
# CC = gcc296

# set the path to the include directory
INCDIR =../include

# set the flags for the C and C++ compiler to give lots of warnings
CFLAGS = -I$(INCDIR) -O2 -Wall -Wstrict-prototypes -Wnested-externs -Wmissing-prototypes -Wmissing-declarations
CPPFLAGS = $(CFLAGS)

# path to the object file directory
ODIR = obj

# path to the lib directory
LIBDIR =../lib

# path to the bin directory
BINDIR =../bin

# libraries to include
LIBS = -lm -limageIO
LFLAGS = -L$(LIBDIR)

# put all of the relevant include files here
_DEPS = ppmIO.h ppmMethods.h Image.h fract.h color.h point.h line.h circle.h polyline.h list.h polygon.h matrix.h view.h module.h drawState.h bezier.h light.h

# convert them to point to the right place
DEPS = $(patsubst %,$(INCDIR)/%,$(_DEPS))

# put a list of the executables here
EXECUTABLES = imagetest mandeltest testbench test4b testbitmap polygonStress hexagon task6_1 test5a test5d test5b test5c proj5_ext1 treeScene scene2d test6a cube test6b task5_4 test7a test7b test7c teapot task6_3 zdebug test8a cubism ball test9A cityScene test9a test9b test9d test9c

# put a list of all the object files here for all executables (with .o endings)
_OBJ = imagetest.o mandeltest.o testbench.o test3a.o test3c.o 3Dmodel.o test4b.o test4a.o testbitmap.o polygonStress.o hexagon.o task6_1.o test5a.o test5d.o test5b.o test5c.o proj5_ext1.o treeScene.o scene2d.o test6a.o cube.o test6b.o task5_4.o test7a.o test7b.o test7c.o teapot.o task6_3.o zdebug.o test8a.o cubism.o ball.o test9A.o cityScene.o test9a.o test9b.o test9d.o test9c.o

# convert them to point to the right place
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

# patterns for compiling source code
# $< is the file that caused the action to occur
$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: %.C $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)


all:
	for i in $(EXECUTABLES); do (make $$i) ; done


# put the specific executable rules here
test3a: $(ODIR)/test3a.o
	$(CC) -g -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

imagetest: $(ODIR)/imagetest.o
	$(CC) -g -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

testbench: $(ODIR)/testbench.o
	$(CC) -g -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

3Dmodel: $(ODIR)/3Dmodel.o
	$(CC) -g -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test4b: $(ODIR)/test4b.o
	$(CC) -g -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test4a: $(ODIR)/test4a.o
	$(CC) -g -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

testbitmap: $(ODIR)/testbitmap.o
	$(CC) -g -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

polygonStress: $(ODIR)/polygonStress.o
	$(CC) -g -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

hexagon: $(ODIR)/hexagon.o
	$(CC) -g -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

task6_1: $(ODIR)/task6_1.o
	$(CC) -g -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test5a: $(ODIR)/test5a.o
	$(CC) -g -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test5d: $(ODIR)/test5d.o
	$(CC) -g -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test5b: $(ODIR)/test5b.o
	$(CC) -g -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test5c: $(ODIR)/test5c.o
	$(CC) -g -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

proj5_ext1: $(ODIR)/proj5_ext1.o
	$(CC) -g -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

treeScene: $(ODIR)/treeScene.o
	$(CC) -g -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

scene2d: $(ODIR)/scene2d.o
	$(CC) -g -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

task5_4: $(ODIR)/task5_4.o
	$(CC) -g -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test6a: $(ODIR)/test6a.o
	$(CC) -g -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

cube: $(ODIR)/cube.o
	$(CC) -g -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test6b: $(ODIR)/test6b.o
	$(CC) -g -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test7a: $(ODIR)/test7a.o
	$(CC) -g -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test7b: $(ODIR)/test7b.o
	$(CC) -g -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test7c: $(ODIR)/test7c.o
	$(CC) -g -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

teapot: $(ODIR)/teapot.o
	$(CC) -g -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

task6_3: $(ODIR)/task6_3.o
	$(CC) -g -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

zdebug: $(ODIR)/zdebug.o
	$(CC) -g -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test8a: $(ODIR)/test8a.o
	$(CC) -g -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

cubism: $(ODIR)/cubism.o
	$(CC) -g -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

ball: $(ODIR)/ball.o
	$(CC) -g -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test9a: $(ODIR)/test9a.o
	$(CC) -g -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test9b: $(ODIR)/test9b.o
	$(CC) -g -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test9c: $(ODIR)/test9c.o
	$(CC) -g -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

cityScene: $(ODIR)/cityScene.o
	$(CC) -g -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test9d: $(ODIR)/test9d.o
	$(CC) -g -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 

