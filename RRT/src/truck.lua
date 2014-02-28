-- A more truck
-- We'll need an extra function that knows how to read Wavefront .OBJ
-- files.

require('readobj')

stone = gr.material({0.8, 0.7, 0.7}, {0.0, 0.0, 0.0}, 0)
grass = gr.material({0.1, 0.7, 0.1}, {0.0, 0.0, 0.0}, 0)
hide = gr.material({0.84, 0.6, 0.53}, {0.3, 0.3, 0.3}, 20)
gold = gr.material({0.9, 0.8, 0.4}, {0.8, 0.8, 0.4}, 25)
red = gr.material({1.0, 0.0, 0.0}, {0.1, 0.1, 0.1}, 10)


-- ##############################################
-- the scene
-- ##############################################

scene = gr.node('scene')
scene:rotate('X', 30)

-- the floor

plane = gr.mesh('plane', {
		   { -1, 0, -1 }, 
		   {  1, 0, -1 }, 
		   {  1,  0, 1 }, 
		   { -1, 0, 1  }
		}, {
		   {3, 2, 1, 0}
		})
scene:add_child(plane)
plane:set_material(grass)
plane:scale(30, 30, 30)


verts, faces = readobj('L200-OBJ/L200-OBJ.obj')
truck = gr.mesh('truck', verts, faces)
scene:add_child(truck)
truck:set_material(red)
truck:scale(0.1, 0.1, 0.1)
--truck:rotate('Y',60)

return scene
