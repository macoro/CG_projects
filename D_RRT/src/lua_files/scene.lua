-- We'll need an extra function that knows how to read Wavefront .OBJ
-- files.


stone = gr.material({0.8, 0.7, 0.7}, {0.0, 0.0, 0.0}, 0)
grass = gr.material({0.1, 0.7, 0.1}, {0.0, 0.0, 0.0}, 0)
hide = gr.material({0.84, 0.6, 0.53}, {0.3, 0.3, 0.3}, 20)
red = gr.material({1.0, 0.0, 0.0}, {0.1, 0.1, 0.1}, 10)
blue = gr.material({0.0, 0.0, 1.0}, {0.1, 0.1, 0.1}, 10)

-- ##############################################
-- the scene
-- ##############################################

scene = gr.node('scene')

---some obtacules


require('cylinder')

cyl:set_material(red)
scene:add_child(cyl)
cyl:translate(-30,0.0,60)
cyl2:set_material(hide)
scene:add_child(cyl2)
cyl2:translate(90,5.0,50)
cyl3:set_material(stone)
scene:add_child(cyl3)
cyl3:translate(-150,0.0,-100)
cyl4:set_material(stone)
scene:add_child(cyl4)
cyl4:translate(-100,0.0,-150)

cyl7:set_material(stone)
scene:add_child(cyl7)
cyl7:translate(-40,-5.0,20)
--cyl7:rotate('Z', 90)
cyl8:set_material(stone)
scene:add_child(cyl8)
cyl8:translate(-100,0.0,-50)
cyl9:set_material(blue)
scene:add_child(cyl9)
cyl9:translate(150,-8.0,150)
--cyl9:rotate('Z', 90)
cyl10:set_material(hide)
scene:add_child(cyl10)
cyl10:translate(-80,0.0,-100)


require('dodeca')
dodeca:set_material(stone)
scene:add_child(dodeca)
dodeca:translate(-60,0.0,-60)
dodeca1:set_material(hide)
scene:add_child(dodeca1)
dodeca1:translate(10,0.0,50)
dodeca2:set_material(red)
scene:add_child(dodeca2)
dodeca2:translate(-200,0.0,-200)


require('icosa')
icosa:set_material(blue)
scene:add_child(icosa)
icosa1:set_material(stone)
scene:add_child(icosa1)
icosa1:translate(-80,4.0, -5.0)
icosa2:set_material(hide)
scene:add_child(icosa2)
icosa2:translate(50.0,8.0, 25.0)
icosa3:set_material(hide)
scene:add_child(icosa3)
icosa3:translate(-180,0.0, -150.0)

scene:translate(0.0,-50,-500)
scene:rotate('X',90)

return scene







