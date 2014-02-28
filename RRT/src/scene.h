#ifndef SCENE_H
#define SCENE_H

#include <list>
#include <map>
#include <vector>
#include "algebra.h"
#include "primitive.h"
#include "material.h"
#include "model.h"
#include "mesh.h"
#include "kdtree.h"


class SceneNode;
class JointNode;
class GeometryNode;

class SceneNode 
{
	public:
		SceneNode(const std::string& name);
		virtual ~SceneNode();

		virtual void walk_gl(bool picking = false) const;

		const Matrix4x4& get_transform() const { return m_trans; }
		const Matrix4x4& get_inverse() const { return m_invtrans; }
  
		void set_transform(const Matrix4x4& m)
		{
			m_trans = m;
			m_invtrans = m.invert();
		}
  
		void clear_all_inverse();

		void set_transform(const Matrix4x4& m, const Matrix4x4& i)
		{
			m_trans = m;
			m_invtrans = i;
		}
		
		void add_transform( const Matrix4x4& m)
		{
			m_trans = m_trans*m;
			m_invtrans = m.invert() * m_invtrans;
		}
		
        void add_child(SceneNode* child);

		void remove_child(SceneNode* child)
		{
			m_children.remove(child);
			child -> m_parent = NULL;
		}
  
		SceneNode* get_parent(){
			return m_parent;
		}

		// Callbacks to be implemented.
		// These will be called from Lua.
		void rotate(char axis, double angle);
		void scale(const Vector3D& amount);
		void translate(const Vector3D& amount);
		
		void reset_joints();

		// Returns true if and only if this node is a JointNode
		virtual bool is_joint() const;
  
		virtual void set_picked( bool p ){ m_picked = p; }
		virtual bool is_picked(){ return m_picked; }
		
        static Matrix4x4 rotationMatrix( char axis, double angle, bool radians=false);
		static Matrix4x4 scaleMatrix( const Vector3D& amount );
		static Matrix4x4 translationMatrix( const Vector3D& amount );
  
		unsigned int getId(){ return m_id; }
		const std::string& getName(){ return m_name; }
  
		void getAllJoints( std::vector<JointNode*>& list);
        void getAllNodes( std::map<unsigned int, SceneNode*> &map );

        virtual bool free_collion(Vector3D &x);
        void draw_path(std::vector<Vector3D> &path);
        void draw_tree(kdtree* tree, bool end=false);
        void set_modelstate(Vector3D state, bool radians = true);

        static model *m_model;
  
	protected:
		// Useful for picking
		int m_id;
		std::string m_name;

		// Transformations
		Matrix4x4 m_trans;
		Matrix4x4 m_invtrans;

		// Hierarchy
		typedef std::list<SceneNode*> ChildList;
		ChildList m_children;
  
		SceneNode* m_parent;
        GeometryNode * m_geomodel;


        bool m_picked;
};



class JointNode : public SceneNode 
{
	public:
		JointNode(const std::string& name);
		virtual ~JointNode();

		virtual void walk_gl(bool bicking = false) const;
		virtual bool is_joint() const;

		void set_joint_x(double min, double init, double max);
		void set_joint_y(double min, double init, double max);
		
		virtual void rotate( char axis, double angle );
		virtual void set_picked( bool p );

		struct JointRange { double min, init, max; };

	protected:

		JointRange m_joint_x, m_joint_y;
};

class GeometryNode : public SceneNode 
{
	public:
		GeometryNode(const std::string& name, Primitive* primitive);
		virtual ~GeometryNode();

		virtual void walk_gl(bool picking = false) const;

		const Material* get_material() const;
		Material* get_material();

		void set_material(Material* material){ m_material = material; }

        double get_length(){ return dynamic_cast<Mesh*>(m_primitive)->getLength();}

        virtual bool free_collion(Vector3D &x);

        Primitive* get_primitive(){return m_primitive;}

	protected:
		Material* m_material;
		Primitive* m_primitive;
};

#endif
