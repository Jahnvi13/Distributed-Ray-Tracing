//object.h
#ifndef _OBJECT_H_
#define _OBJECT_H_
#include <iostream>
#include "ray.h"
#include "vector3D.h"
#include "color.h"
#include "material.h"

class Object
{
protected:
	Material *material;
	bool isSolid;
	int numTransform;	// Number of transformations that have happened.
	Vector3D movementStep;
public:
	Object(Material *mat, Vector3D _movementStep): material(mat) , movementStep(_movementStep) {}	
	virtual bool intersect(Ray& ray) const = 0;
	virtual Color shade(const Ray& ray) const
	{
		return material->shade(ray, isSolid);
	}
	virtual void moveObject(){std::cout<<"Object: moveObject()"<<std::endl;};
	virtual void resetPosition(){};
	virtual Vector3D getNormal(Vector3D raypos) const = 0;
};

#endif
