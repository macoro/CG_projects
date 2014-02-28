# C++ compiler.
CC = g++

# Compile flags
CFLAGS = -O2 -Wall -Wno-deprecated
#CFLAGS = -g -Wall -Wno-deprecated

# QHULL settings
QHULL_DIR = /home/user/Dropbox/CGraphics/D_RRT/src/qhull

# Object files
OBJECT = src/fileio.o \
         src/lut.o \
         src/mesh.o \
         src/mesh_utils.o \
         src/object.o \
         src/pair.o \
         src/pqueue.o \
         src/scene.o

all: decomposer libSWIFT++.a

libSWIFT++.a:
	cd src; $(MAKE) CC="$(CC)" CFLAGS="$(CFLAGS)" QHULL_DIR="$(QHULL_DIR)" libSWIFT++.a; cd ..

lib/libSWIFT++decomp.a:
	cd src; $(MAKE) CC="$(CC)" CFLAGS="$(CFLAGS) -DSWIFT_DECOMP" QHULL_DIR="$(QHULL_DIR)" libSWIFT++decomp.a; cd ..

decomposer: decomposer_c

decomposer_c: lib/libSWIFT++decomp.a only_decomposer_c

decomposer_g: lib/libSWIFT++decomp.a only_decomposer_g

only_decomposer_c: lib/libSWIFT++decomp.a
	cd src; $(MAKE) CC="$(CC)" CFLAGS="$(CFLAGS) -DSWIFT_ALLOW_BOUNDARY -DSWIFT_DECOMP" QHULL_DIR="$(QHULL_DIR)" libSWIFT++decomp.a; cd ..
	cd decomposer; $(MAKE) CC="$(CC)" CFLAGS="$(CFLAGS)" QHULL_DIR="$(QHULL_DIR)" decomposer_c; cd ..

only_decomposer_g: lib/libSWIFT++decomp.a
	cd decomposer; $(MAKE) CC="$(CC)" CFLAGS="$(CFLAGS) -DSWIFT_ALLOW_BOUNDARY" QHULL_DIR="$(QHULL_DIR)" decomposer_g; cd ..

example: example_prog

example_prog:
	cd example; $(MAKE) CC="$(CC)" CFLAGS="$(CFLAGS)" QHULL_DIR="$(QHULL_DIR)"; cd ..

# Remove all but executables
clean:
	rm -f src/*.o lib/*.a example/*.o decomposer/RAPID/*.[ao] decomposer/src/*.[ao] */Makefile.bak decomposer/src/Makefile.bak
	rm -rf *~

# Remove all
realclean:
	rm -f src/*.o lib/*.a example/*.o example/example example/example.exe decomposer/bin/decomposer decomposer/RAPID/*.[ao] decomposer/src/*.[ao] */Makefile.bak decomposer/src/Makefile.bak
	rm -f *.plg *.opt *.ncb */*.plg */*.opt */*.ncb *.ilk */*.ilk
	rm -rf Debug Release */Debug */Release
	rm -f *~ */*~ */*/*~

