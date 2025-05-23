//sphere.cpp

#include "triangle.h"
#include <iostream>
#define  GLM_FORCE_RADIANS
#define  GLM_ENABLE_EXPERIMENTAL

#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

bool Triangle::intersect(Ray& r) const
{
	
	glm::mat3 x = glm::mat3(glm::vec3(a[0]-b[0], a[0]-c[0], r.getDirection()[0]),
							glm::vec3(a[1]-b[1], a[1]-c[1], r.getDirection()[1]),
							glm::vec3(a[2]-b[2], a[2]-c[2], r.getDirection()[2]));
	x  = transpose(x);
	glm::vec3 abc = inverse(x)*glm::vec3(a[0]-r.getOrigin()[0], a[1]-r.getOrigin()[1], a[2]-r.getOrigin()[2]);
	if (abc[0]>0 && abc[1]>0 && abc[0]+abc[1]<1){
		r.setParameter(abc[2],this);
		return true;
	}
	return false;

}
void Triangle::moveObject() {
    a += movementStep;
    b += movementStep;
    c += movementStep;
    numTransform++;
}

void Triangle::resetPosition() {
    a -= (movementStep*numTransform);
    b -= (movementStep*numTransform);
    c -= (movementStep*numTransform);
    numTransform=0;
}

