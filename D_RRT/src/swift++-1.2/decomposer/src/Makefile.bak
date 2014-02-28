.SUFFIXES: .o .cpp

# Header files
HDS = -I../include -I$(QHULL_DIR) -I../RAPID -I../../include

# Library directories
LIBDIRS_C = -L$(QHULL_DIR) -L../../lib -L../RAPID 

LIBDIRS_G = -L/usr/X11R6/lib $(LIBDIRS_C)

# Libraries
OTHER_LIBS = -lm #-ldl

LIBS_C = -lRAPIDdecomp -lSWIFT++decomp -lqhull $(OTHER_LIBS)

LIBS_G = $(LIBS_C) -lGLU -lGL -ltcl8.3 -ltk8.3 -lXext -lXi -lXmu -lX11 \
         $(OTHER_LIBS)

# Object files
OBJECT_C = convex.o cvxutils.o gui.o io.o main.o

OBJECT_G = $(OBJECT_C) guidraw.o togl.o viewer.o

SOURCES = $(OBJECT_G:.o=.cpp)

.cpp.o:
	$(CC) -c $(CFLAGS) -DSWIFT_DECOMP $(HDS) $*.cpp

decomposer_c: clean depend $(OBJECT_C)
	$(CC) $(CFLAGS) -o decomposer $(OBJECT_C) $(LIBDIRS_C) $(LIBS_C)

decomposer_g: clean depend $(OBJECT_G)
	$(CC) $(CFLAGS) -o decomposer $(OBJECT_G) $(LIBDIRS_G) $(LIBS_G)

depend:
	makedepend -Y -- $(CFLAGS) $(HDS) -- $(SOURCES)

clean:
	rm -f *.o

# DO NOT DELETE

convex.o: ../../include/SWIFT.h ../../include/SWIFT_config.h
convex.o: ../../include/SWIFT_common.h ../../include/SWIFT_array.h
convex.o: ../../include/SWIFT_mesh.h ../../include/SWIFT_linalg.h
convex.o: ../../include/SWIFT_lut.h ../../include/SWIFT_mesh_utils.h
convex.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/qhull_a.h
convex.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/libqhull.h
convex.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/user.h
convex.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/stat.h
convex.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/random.h
convex.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/mem.h
convex.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/qset.h
convex.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/geom.h
convex.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/merge.h
convex.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/poly.h
convex.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/io.h
convex.o: ../include/cvxutils.h ../include/convex.h
cvxutils.o: ../RAPID/RAPID.h ../RAPID/RAPID_private.h ../../include/SWIFT.h
cvxutils.o: ../../include/SWIFT_config.h ../../include/SWIFT_common.h
cvxutils.o: ../../include/SWIFT_array.h ../../include/SWIFT_mesh.h
cvxutils.o: ../../include/SWIFT_linalg.h ../../include/SWIFT_lut.h
cvxutils.o: ../../include/SWIFT_mesh_utils.h
cvxutils.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/qhull_a.h
cvxutils.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/libqhull.h
cvxutils.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/user.h
cvxutils.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/stat.h
cvxutils.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/random.h
cvxutils.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/mem.h
cvxutils.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/qset.h
cvxutils.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/geom.h
cvxutils.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/merge.h
cvxutils.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/poly.h
cvxutils.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/io.h
cvxutils.o: ../include/cvxutils.h
gui.o: ../../include/SWIFT.h ../../include/SWIFT_config.h
gui.o: ../../include/SWIFT_common.h ../../include/SWIFT_array.h
gui.o: ../../include/SWIFT_mesh.h ../../include/SWIFT_linalg.h
gui.o: ../../include/SWIFT_lut.h ../../include/SWIFT_mesh_utils.h
gui.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/qhull_a.h
gui.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/libqhull.h
gui.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/user.h
gui.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/stat.h
gui.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/random.h
gui.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/mem.h
gui.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/qset.h
gui.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/geom.h
gui.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/merge.h
gui.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/poly.h
gui.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/io.h
gui.o: ../../include/SWIFT_fileio.h ../include/convex.h ../include/gui.h
io.o: ../../include/SWIFT.h ../../include/SWIFT_config.h
io.o: ../../include/SWIFT_common.h ../../include/SWIFT_array.h
io.o: ../../include/SWIFT_mesh.h ../../include/SWIFT_linalg.h
io.o: ../../include/SWIFT_lut.h ../../include/SWIFT_fileio.h
main.o: ../../include/SWIFT.h ../../include/SWIFT_config.h
main.o: ../../include/SWIFT_common.h ../../include/SWIFT_array.h
main.o: ../../include/SWIFT_mesh.h ../../include/SWIFT_linalg.h
main.o: ../../include/SWIFT_lut.h ../../include/SWIFT_fileio.h
main.o: ../include/gui.h
togl.o: ../include/togl.h
