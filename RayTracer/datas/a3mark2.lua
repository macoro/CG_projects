--
-- CS488 -- Introduction to Computer Graphics
--
-- a3mark.lua
--
-- A very simple scene creating a trivial puppet.  The TAs will be
-- using this scene as part of marking your assignment.  You'll
-- probably want to make sure you get reasonable results with it!

rootnode = gr.node('root')

red = gr.material({1.0, 0.0, 0.0}, {0.1, 0.1, 0.1}, 10)
blue = gr.material({0.0, 0.0, 1.0}, {0.1, 0.1, 0.1}, 10)
green = gr.material({0.0, 1.0, 0.0}, {0.1, 0.1, 0.1}, 10)
white = gr.material({1.0, 1.0, 1.0}, {0.1, 0.1, 0.1}, 10)

torso = gr.sphere('Torso')
rootnode:add_child(torso)
torso:scale(0.8,1.5,0.5)
torso:set_material(green)

shoulders = gr.sphere('Hombros')
torso:add_child(shoulders)
shoulders:translate(0.0,1.5,0.0)
shoulders:scale(1.2, 0.2, 0.5)
shoulders:set_material(red)

hips = gr.sphere('Caderas')
torso:add_child(hips)
hips:translate(0.0, -1.5, 0.0)
hips:scale(0.8, 0.2, 0.5)
hips:set_material(blue)



leftshoulder = gr.joint('Hombro Izquierdo',{-45,0,180},{-180,0,0})
shoulders:add_child(leftshoulder)

leftarm = gr.sphere('Brazo Izquierdo Superior')
leftshoulder:add_child(leftarm)
leftshoulder:translate(-1.2,1.5,0.0)
--leftshoulder:rotate('z',-30)
leftarm:translate(0.0,-1.0,0.0)
leftarm:scale(0.15,1.0,0.2)
leftarm:set_material(blue)

rightshoulder = gr.joint('Hombro Derecho',{-45,0,180},{0,0,180})
shoulders:add_child(rightshoulder)

rightarm = gr.sphere('Brazo Derecho Superior')
rightshoulder:add_child(rightarm)
rightshoulder:translate(1.2,1.5,0.0)
--rightshoulder:rotate('z',30)
rightarm:translate(0.0,-1.0,0.0)
rightarm:scale(0.15,1.0,0.2)
rightarm:set_material(blue)

-- The lights
l1 = gr.light({200,200,400}, {0.8, 0.8, 0.8}, {1, 0, 0})
l2 = gr.light({0, 5, -20}, {0.4, 0.4, 0.8}, {1, 0, 0})

gr.render(rootnode, 'marioneta2.png', 512, 512, 
	  {0, 0, 10 }, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.4, 0.4, 0.4}, {l1, l2})
