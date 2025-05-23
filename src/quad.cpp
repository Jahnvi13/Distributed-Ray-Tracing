//sphere.cpp

#include "quad.h"
#include "ray.h"
#include "world.h"
#include <iostream>
#define  GLM_FORCE_RADIANS
#define  GLM_ENABLE_EXPERIMENTAL

#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

bool Quad::intersect(Ray& r) const
{
	
	glm::mat3 x = glm::mat3(glm::vec3(p1[0]-p2[0], p1[0]-p3[0], r.getDirection()[0]),
							glm::vec3(p1[1]-p2[1], p1[1]-p3[1], r.getDirection()[1]),
							glm::vec3(p1[2]-p2[2], p1[2]-p3[2], r.getDirection()[2]));
	x  = transpose(x);
	glm::vec3 abc = inverse(x)*glm::vec3(p1[0]-r.getOrigin()[0], p1[1]-r.getOrigin()[1], p1[2]-r.getOrigin()[2]);
	if (abc[0]>0 && abc[1]>0 && abc[0]+abc[1]<1){
		r.setParameter(abc[2],this);
		return true;
	}
	x = glm::mat3(glm::vec3(p2[0]-p3[0], p2[0]-p4[0], r.getDirection()[0]),
							glm::vec3(p2[1]-p3[1], p2[1]-p4[1], r.getDirection()[1]),
							glm::vec3(p2[2]-p3[2], p2[2]-p4[2], r.getDirection()[2]));
	x  = transpose(x);
	abc = inverse(x)*glm::vec3(p2[0]-r.getOrigin()[0], p2[1]-r.getOrigin()[1], p2[2]-r.getOrigin()[2]);
	if (abc[0]>0 && abc[1]>0 && abc[0]+abc[1]<1){
		r.setParameter(abc[2],this);
		return true;
	}
	return false;
}
void Quad::moveObject() {
    p1 += movementStep;
    p2 += movementStep;
    p3 += movementStep;
    p4 += movementStep;
    
    numTransform++;
}

void Quad::resetPosition() {
    p1 -= (movementStep*numTransform);
    p2 -= (movementStep*numTransform);
    p3 -= (movementStep*numTransform);
    p4 -= (movementStep*numTransform);
    
    numTransform=0;
}