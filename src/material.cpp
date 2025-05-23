#include "world.h"
#include "material.h"
#include <iostream>

Color Material::recurse(const Ray& ray) const{
	

	//Direction opposite to light source
	Vector3D l = (world->getLightSourceList()).front()->getPosition()-ray.getPosition();
	
	//Intensity of light
	Color I = (world->getLightSourceList()).front()->getIntensity(); //(ray.intersected())->shade(ray); //
	
	//Ambient light
	Color ambient = world->getAmbient();

	//Shadow ray

	int n=2;   
    bool flag=false;
    for (int i = 0; i < n; i++) {
        double eps_x = (float(rand())/float((RAND_MAX)) * 1)-0.5;
        double eps_y = (float(rand())/float((RAND_MAX)) * 1)-0.5;
        
        Vector3D o = Vector3D(ray.getPosition().X() + eps_x, ray.getPosition().Y()+ eps_y, ray.getPosition().Z());
        Ray shadow_ray(o, l);
        world->firstIntersection(shadow_ray);
        if(shadow_ray.didHit()){
        	flag=true;
        	break;
        }
    }
	
	if (flag)
	{	
		//no is the normal at the point of intersection
		Vector3D no = unitVector(ray.intersected()->getNormal(ray.getPosition()));
		Vector3D l = unitVector(world->getLightSourceList().front()->getPosition()-ray.getPosition());
		
		//v is the viewing direction (the direction opposite to viewing ray)
		Vector3D v = -unitVector(ray.getDirection()); //unitVector(ray.getOrigin() - ray.getPosition());
		
		//h is the half vector
		Vector3D h = unitVector(v+l);
	
		//lambert shading
		Color lambert = I*double(kd*std::max(0.0,dotProduct(no,l)));
		
		//specular shading
		Color specular = I*pow(double(ks*std::max(0.0,dotProduct(no,h))),n);
		
		//ambient shading
		Color ambient = world->getAmbient()*ka;//I*ka;
		return color*(lambert + specular + ambient);

	}
	return color*ambient;
	
}

Color Material::shade(const Ray& ray, const bool isSolid) const
{
	// Call the recursive ray tracer	
	return recurse(ray);
	//return color;
		
}
