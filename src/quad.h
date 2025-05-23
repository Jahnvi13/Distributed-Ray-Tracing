//sphere.h
#ifndef _QUAD_H_
#define _QUAD_H_

#include "object.h"
#include "ray.h"
#include "vector3D.h"
#include "color.h"
class Quad : public Object
{
private:
	Vector3D p1;
	Vector3D p2;
	Vector3D p3;
	Vector3D p4;

public:
	Quad(const Vector3D& _vtx1, const Vector3D& _vtx2,  const Vector3D& _vtx3, const Vector3D& _vtx4, Material* mat, Vector3D _movementStep):
		Object(mat, _movementStep), p1(_vtx1), p2(_vtx2), p3(_vtx3), p4(_vtx4)
	{
		isSolid = true;
	}
	virtual void moveObject();
	virtual void resetPosition();
	virtual bool intersect(Ray& r) const;
	virtual Vector3D getNormal(Vector3D r) const
	{
		return unitVector(crossProduct(p2-p1, p3-p1));
	}
};
#endif