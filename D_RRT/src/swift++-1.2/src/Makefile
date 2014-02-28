.SUFFIXES: .o .cpp

HDS = -I../include -I$(QHULL_DIR)

# Object files
OBJECT = fileio.o \
         lut.o \
         mesh.o \
         mesh_utils.o \
         object.o \
         pair.o \
         pqueue.o \
         scene.o

SOURCES = $(OBJECT:.o=.cpp)

.cpp.o:
	$(CC) -c $(CFLAGS) $(HDS) $*.cpp

all: libSWIFT++.a

libSWIFT++.a: clean depend $(OBJECT)
	ar ruvs ../lib/libSWIFT++.a $(OBJECT)
	rm *.o

libSWIFT++decomp.a: clean depend $(OBJECT)
	ar ruvs ../lib/libSWIFT++decomp.a $(OBJECT)
	rm *.o

depend:
	makedepend -Y -- $(HDS) -- $(SOURCES)

clean:
	rm -f *.o

# DO NOT DELETE

fileio.o: ../include/SWIFT.h ../include/SWIFT_config.h
fileio.o: ../include/SWIFT_common.h ../include/SWIFT_array.h
fileio.o: ../include/SWIFT_mesh.h ../include/SWIFT_linalg.h
fileio.o: ../include/SWIFT_lut.h ../include/SWIFT_fileio.h
lut.o: ../include/SWIFT_config.h ../include/SWIFT_common.h
lut.o: ../include/SWIFT_linalg.h ../include/SWIFT_mesh.h
lut.o: ../include/SWIFT_array.h ../include/SWIFT_lut.h
mesh.o: ../include/SWIFT_config.h ../include/SWIFT_common.h
mesh.o: ../include/SWIFT_linalg.h ../include/SWIFT_array.h
mesh.o: ../include/SWIFT_mesh.h ../include/SWIFT_lut.h
mesh.o: ../include/SWIFT_mesh_utils.h
mesh.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/qhull_a.h
mesh.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/libqhull.h
mesh.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/user.h
mesh.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/stat.h
mesh.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/random.h
mesh.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/mem.h
mesh.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/qset.h
mesh.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/geom.h
mesh.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/merge.h
mesh.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/poly.h
mesh.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/io.h
mesh_utils.o: ../include/SWIFT_config.h ../include/SWIFT_common.h
mesh_utils.o: ../include/SWIFT_linalg.h ../include/SWIFT_array.h
mesh_utils.o: ../include/SWIFT_mesh.h ../include/SWIFT_lut.h
mesh_utils.o: ../include/SWIFT_mesh_utils.h
mesh_utils.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/qhull_a.h
mesh_utils.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/libqhull.h
mesh_utils.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/user.h
mesh_utils.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/stat.h
mesh_utils.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/random.h
mesh_utils.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/mem.h
mesh_utils.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/qset.h
mesh_utils.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/geom.h
mesh_utils.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/merge.h
mesh_utils.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/poly.h
mesh_utils.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/io.h
object.o: ../include/SWIFT_config.h ../include/SWIFT_common.h
object.o: ../include/SWIFT_linalg.h ../include/SWIFT_mesh.h
object.o: ../include/SWIFT_array.h ../include/SWIFT_lut.h
object.o: ../include/SWIFT_object.h ../include/SWIFT_pair.h
object.o: ../include/SWIFT_boxnode.h
pair.o: ../include/SWIFT_config.h ../include/SWIFT_common.h
pair.o: ../include/SWIFT_linalg.h ../include/SWIFT_array.h
pair.o: ../include/SWIFT_mesh.h ../include/SWIFT_lut.h
pair.o: ../include/SWIFT_object.h ../include/SWIFT_pair.h
pair.o: ../include/SWIFT_boxnode.h
scene.o: ../include/SWIFT.h ../include/SWIFT_config.h
scene.o: ../include/SWIFT_common.h ../include/SWIFT_array.h
scene.o: ../include/SWIFT_linalg.h ../include/SWIFT_mesh.h
scene.o: ../include/SWIFT_lut.h ../include/SWIFT_mesh_utils.h
scene.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/qhull_a.h
scene.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/libqhull.h
scene.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/user.h
scene.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/stat.h
scene.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/random.h
scene.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/mem.h
scene.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/qset.h
scene.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/geom.h
scene.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/merge.h
scene.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/poly.h
scene.o: /home/user/Dropbox/CGraphics/D_RRT/src/qhull/io.h
scene.o: ../include/SWIFT_boxnode.h ../include/SWIFT_object.h
scene.o: ../include/SWIFT_pair.h ../include/SWIFT_fileio.h
