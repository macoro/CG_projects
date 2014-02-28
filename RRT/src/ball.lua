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
scene:rotate('X', 23)


verts, faces = readobj('xmas-ball.obj')
ball = gr.mesh('ball', verts, faces)
scene:add_child(ball)
ball:set_material(red)
ball:translate(0.0, -4, 0.0)
ball:scale(0.2, 0.2, 0.2)


return scene
