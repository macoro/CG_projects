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



leftelbow = gr.joint('Codo Izquierdo',{-10,0,10},{0,0,120})
leftarm:add_child(leftelbow)

leftforearm = gr.sphere('Antebrazo Izquierdo')
leftelbow:add_child(leftforearm)
leftelbow:translate(0.0,-2.0,0.0)
--leftelbow:rotate('z',60)
leftforearm:translate(0.0,-1.0,0.0)
leftforearm:scale(0.1,1.0,0.15)
leftforearm:set_material(green)

rightelbow = gr.joint( 'Codo Derecho',{-10,0,10},{-120,0,0})
rightarm:add_child(rightelbow)

rightforearm = gr.sphere('Antebrazo Derecho')
rightelbow:add_child(rightforearm)
rightelbow:translate(0.0,-2.0,0.0)
--rightelbow:rotate('z',-60)
rightforearm:translate(0.0,-1.0,0.0)
rightforearm:scale(0.1,1.0,0.15)
rightforearm:set_material(green)



leftwrist = gr.joint('Muñeca Izquierda',{-90,0,90},{-90,0,90})
leftforearm:add_child(leftwrist)

lefthand = gr.sphere('Mano Izquierda')
leftwrist:add_child(lefthand)
leftwrist:translate(0.0,-2.0,0.0)
--leftwrist:rotate('z',-30);
lefthand:translate(0.0,-0.3,0.0)
lefthand:scale(0.1,0.3,0.15)
lefthand:set_material(red)

rightwrist = gr.joint('Muñeca Derecha',{-90,0,90},{-90,0,90})
rightforearm:add_child(rightwrist)

righthand = gr.sphere('Mano Derecha')
rightwrist:add_child(righthand)
rightwrist:translate(0.0,-2.0,0.0)
--rightwrist:rotate('z',30);
righthand:translate(0.0,-0.3,0.0)
righthand:scale(0.1,0.3,0.15)
righthand:set_material(red)



lefthip = gr.joint('Cadera Izquierda',{-120,0,30},{-90,0,20})
hips:add_child(lefthip)

leftthigh = gr.sphere('Muslo Izquierdo')
lefthip:add_child(leftthigh)
lefthip:translate(-0.5,-1.5,0.0)
--lefthip:rotate('z',-15)
leftthigh:translate(0.0,-1.0,0.0)
leftthigh:scale(0.15,1.0,0.25)
leftthigh:set_material(green)

righthip = gr.joint('Cadera Derecha',{-120,0,30},{-20,0,90})
hips:add_child(righthip)

rightthigh = gr.sphere('Muslo Derecho')
righthip:add_child(rightthigh)
righthip:translate(0.5,-1.5,0.0)
--righthip:rotate('z',15)
rightthigh:translate(0.0,-1.0,0.0)
rightthigh:scale(0.15,1.0,0.25)
rightthigh:set_material(green)



leftknee = gr.joint('Rodilla Izquierda',{0,0,120},{0,0,0})
leftthigh:add_child(leftknee)

leftcalf = gr.sphere('Pantorrilla Izquierda')
leftknee:add_child(leftcalf)
leftknee:translate(0.0,-2.0,0.0)
leftcalf:translate(0.0,-0.8,0.0)
leftcalf:scale(0.15,0.8,0.20)
leftcalf:set_material(blue)

rightknee = gr.joint('Rodilla Derecha',{0,0,120},{0,0,0})
rightthigh:add_child(rightknee)

rightcalf = gr.sphere('Pantorrilla Derecha')
rightknee:add_child(rightcalf)
rightknee:translate(0.0,-2.0,0.0)
rightcalf:translate(0.0,-0.8,0.0)
rightcalf:scale(0.15,0.8,0.20)
rightcalf:set_material(blue)



leftankle = gr.joint('Tobillo Izquierdo',{0,0,90},{-30,0,30})
leftcalf:add_child(leftankle)

leftfoot = gr.sphere('Pie Izquierdo')
leftankle:add_child(leftfoot)
leftankle:translate(0.0,-1.6,0.0)
leftfoot:translate(0.0,-0.06,0.5)
leftfoot:scale(0.15,0.06,0.6)
leftfoot:set_material(red)

rightankle = gr.joint('Tobillo Derecho',{0,0,90},{-30,0,30})
rightcalf:add_child(rightankle)

rightfoot = gr.sphere('Pie Derecho')
rightankle:add_child(rightfoot)
rightankle:translate(0.0,-1.6,0.0)
rightfoot:translate(0.0,-0.06,0.5)
rightfoot:scale(0.15,0.06,0.6)
rightfoot:set_material(red)



nape = gr.joint( 'Nuca',{-30,0,30},{0,0,0})
shoulders:add_child(nape)

neck = gr.sphere('Cuello')
nape:add_child(neck)
nape:translate(0.0,1.5,0.0)
neck:translate(0.0,0.4,0.0)
neck:scale(0.25,0.4,0.25)
neck:set_material(blue)

headneck = gr.joint( 'Articulacion Cabeza-Cuello',{-30,0,30},{-90,0,90})
neck:add_child(headneck)

head = gr.sphere('Cabeza')
headneck:add_child(head)
headneck:translate(0.0,0.6,0.0)
head:translate(0.0,0.7,0.0)
head:scale(0.7,0.7,0.7)
head:set_material(green)

nose = gr.sphere('Nariz')
headneck:add_child(nose)
nose:translate(0.0,0.5,0.7)
nose:scale(0.15,0.15,0.3)
nose:set_material(red)

--rootnode:translate(0.0, 1.0, -10.0)
--rootnode:rotate('y', -90.0)

return rootnode
