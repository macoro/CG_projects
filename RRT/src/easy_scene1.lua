-- We'll need an extra function that knows how to read Wavefront .OBJ
-- files.

require('readobj')

stone = gr.material({0.8, 0.7, 0.7}, {0.0, 0.0, 0.0}, 0)
grass = gr.material({0.1, 0.7, 0.1}, {0.0, 0.0, 0.0}, 0)
hide = gr.material({0.84, 0.6, 0.53}, {0.3, 0.3, 0.3}, 20)
red = gr.material({1.0, 0.0, 0.0}, {0.1, 0.1, 0.1}, 10)
blue = gr.material({0.0, 0.0, 1.0}, {0.1, 0.1, 0.1}, 10)

-- ##############################################
-- the scene
-- ##############################################

scene = gr.node('scene')

-- the floor

plane = gr.mesh('plane', {
		   { -500, 0, -500 }, 
		   {  500, 0, -500 }, 
		   {  500,  0, 500 }, 
		   { -500, 0, 500  }
		}, {
		   {3, 2, 1, 0}
		})
scene:add_child(plane)
plane:set_material(grass)
plane:translate(0.0,-1.3,0.0)


---the truck
verts, faces = readobj('L200-OBJ/L200-OBJ.obj')
truck = gr.mesh('model', verts, faces)
truck:translate(-400.0,0.0,-400.0)
scene:add_child(truck)
truck:set_material(red)

--truck:rotate('Y',60)


---some obtacules
require('cylinder')

cyl:set_material(hide)
scene:add_child(cyl)
cyl:translate(-150,0.0,100)
cyl2:set_material(hide)
scene:add_child(cyl2)
cyl2:translate(150,0.0,100)
cyl3:set_material(stone)
scene:add_child(cyl3)
cyl3:translate(-150,0.0,-100)
cyl4:set_material(blue)
scene:add_child(cyl4)
cyl4:translate(350,0.0,-200)


cyl8:set_material(stone)
scene:add_child(cyl8)
cyl8:translate(-100,0.0,-250)

cyl10:set_material(hide)
scene:add_child(cyl10)
cyl10:translate(-300,0.0,200)

require('dodeca')
dodeca:set_material(stone)
scene:add_child(dodeca)
dodeca:translate(300,0.0,-150)
dodeca1:set_material(hide)
scene:add_child(dodeca1)
dodeca1:translate(100,0.0,-100)
dodeca2:set_material(stone)
scene:add_child(dodeca2)
dodeca2:translate(250,0.0,10)




require('icosa')
icosa:set_material(stone)
scene:add_child(icosa)
icosa1:set_material(stone)
scene:add_child(icosa1)
icosa1:translate(-180,0.0, -25.0)
icosa2:set_material(hide)
scene:add_child(icosa2)
icosa2:translate(-220,0.0, -150.0)
icosa3:set_material(blue)
scene:add_child(icosa3)
icosa3:translate(-310,0.0, -200.0)



scene:translate(0.0,-50,-600)
scene:rotate('X',60)

return scene







