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
-- the arch
-- ##############################################

-- the arc
arc = gr.node('arc')
--scene:add_child(arc)
arc:rotate('Y', 60)
arc:translate(0,0,-10)
p1 = gr.cube('p1')
arc:add_child(p1)
p1:set_material(stone)
p1:translate(-2.4, 0, -0.4)
p1:scale(0.8, 4, 0.8)

p2 = gr.cube('p2')
arc:add_child(p2)
p2:set_material(stone)
p2:translate(1.6, 0, -0.4)
p2:scale(0.8, 4, 0.8)

s = gr.sphere('s')
arc:add_child(s)
s:set_material(stone)
s:translate(0, 4, 0)
s:scale(4, 0.6, 0.6)


-- ##############################################
-- the scene
-- ##############################################

scene = gr.node('scene')
scene:rotate('X', 23)

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


verts, faces = readobj('xmas-ball.obj')
ball = gr.mesh('ball', verts, faces)
scene:add_child(ball)
ball:set_material(red)
ball:scale(0.2, 0.2, 0.2)


-- Use the instanced cow model to place some actual cows in the scene.
-- For convenience, do this in a loop.


-- Place a ring of arches.

for i = 1, 3 do
   an_arc = gr.node('arc' .. tostring(i))
   an_arc:rotate('Y', (i-1) * 60)
   scene:add_child(an_arc)
   an_arc:add_child(arc)
end

gr.render(scene,
	  'ball.png', 512, 512,
	  {0, 2, 30}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.05, 0.05, 0.05}, {gr.light({200, 202, 430}, {0.8, 0.8, 0.8}, {1, 0, 0})})
