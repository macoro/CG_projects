-- Mickey

mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 3)

scene_root = gr.node('root')


require('mickey')

mickey:set_material(mat1)
scene_root:add_child(mickey)
scene_root:rotate('Y', -90)
scene_root:rotate('X', -90)

white_light = gr.light({-1000.0, 150.0, 400.0}, {0.9, 0.9, 0.9}, {1, 0, 0})
orange_light = gr.light({400.0, 1000.0, 150.0}, {0.7, 0.0, 0.7}, {1, 0, 0})

gr.render(scene_root, 'mickey.png', 512, 512,
	  {0, 0, 3}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.1, 0.1, 0.1}, {white_light, orange_light})
