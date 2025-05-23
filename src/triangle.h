//sphere.h
#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include "object.h"
#include "ray.h"
#include "vector3D.h"
#include "color.h"
class Triangle : public Object
{
private:
	Vector3D a;
	Vector3D b;
	Vector3D c;
	
public:
	Triangle(const Vector3D& _vtx1, const Vector3D& _vtx2,  const Vector3D& _vtx3, Material* mat, Vector3D _movementStep):
		Object(mat, _movementStep), a(_vtx1), b(_vtx2), c(_vtx3)
	{
		isSolid = true;
	}
	
	virtual bool intersect(Ray& r) const;
	virtual void moveObject();
    virtual void resetPosition();
	virtual Vector3D getNormal(Vector3D r) const
	{
		return unitVector(crossProduct(b-a, c-a));
	}
};
#endif