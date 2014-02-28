
require('readobj')

stone = gr.material({0.8, 0.7, 0.7}, {0.0, 0.0, 0.0}, 0)

scene = gr.node('scene')

require('buckyball')

scene:add_child(buckyball)
buckyball:set_material(stone)

scene:translate(0.0,0.0,20)
scene:rotate('X',10)

return scene









