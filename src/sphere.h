//sphere.h
#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "object.h"
#include "ray.h"
#include "vector3D.h"
#include "color.h"

class Sphere : public Object
{
private:
	Vector3D position;
	double radius;

public:
	Sphere(const Vector3D& _pos, double _rad, Material* mat, Vector3D _movementStep):
		Object(mat, _movementStep), position(_pos), radius(_rad)
	{
		isSolid = true;
	}
	virtual void moveObject();
	virtual void resetPosition();
	virtual bool intersect(Ray& r) const;
	virtual Vector3D getNormal(Vector3D r) const;
};
#endif
