#include "renderengine.h"
constexpr double PI = 3.14159265358;
const Color RenderEngine::trace(const double i, const double j)
{
	Vector3D ray_dir = camera->get_ray_direction(i, j);
	Ray ray(camera->get_position(), ray_dir);
	return world->shade_ray(ray);
	//return DOF(ray);
}

// Motion blur and antialiasing

bool RenderEngine::renderLoop()
{
    static int i = 0;
    Color rgb_prev(0,0,0), zero_col(0,0,0);
    bool motionBlur=true;
    for(int j = 0; j<camera->getHeight(); j++) {
        Color color = trace(i,j);
        if(motionBlur) {
	    // Initialize the frame color. We add the pixel colour to this frame color as the object moves and take the average.
            Color framecolor(0, 0, 0);
            float motionBlurFrames = 5.0;
            float sum = 1.0;
            for (int k = 2; k <= ((int)motionBlurFrames); k++) {
                // Move the object
		world->moveObjects();
        
                Color left = trace(double(i-1),double(j));
		left.clamp();
		Color jitcolor = color; 
		    
		// Test if it's an edge and antialias if it is.
		if(!(rgb_prev==zero_col) && (color-left>0.01 || color-rgb_prev>0.01)){
			Color c(0,0,0);
			double n=4;
			double eps = (float(rand())/float((RAND_MAX)) * 0.5);
			
			// Divide a pixel into nxn grid and emanate a ray from each cell.
			// Stratified sampling as given in FCG.
			for(int p=0;p<n;p++){
				for(int q=0;q<n;q++){
					Color f = trace(i-1+(p + eps)/n, j-1+(q + eps)/n);
					c = c+f;
				}
			}
			c = c / (n*n);
			jitcolor=c;
		}
                framecolor = framecolor + jitcolor * k;
                sum+=k;
            }
            color = color + framecolor;
            world->resetObjects();
            motionBlurFrames = 1 / sum;
            color = color * motionBlurFrames;
        }
        
		camera->drawPixel(i, j, color);
		rgb_prev.R(color.r);
		rgb_prev.G(color.g);
		rgb_prev.B(color.b);
    }

    if(++i == camera->getWidth()) {
        i=0;
        return true;
    }
    else
    	return false;
}
// Depth of frame 
Color RenderEngine::DOF(Ray &ray){
    
    // Define the aperture and the focal length
    double aperture=1.0f;
    double f = 40.0f;
    Vector3D eye = camera->get_position();
	
    // The focal plane would be a curved surface f distance from the eye of the camera
    Vector3D fp = eye+f*ray.getDirection();
    
    int n=8;   
    Color rgb(0,0,0);

    for (int i = 0; i < n; i++) {
	    
	// Perturb the origin by this x and y
        double eps_x = (float(rand())/float((RAND_MAX)) * 1)-0.5;
        double eps_y = (float(rand())/float((RAND_MAX)) * 1)-0.5;
        
	// Generate the new ray
        Vector3D o = Vector3D(eye.X()+aperture*eps_x, eye.Y()+aperture*eps_y, eye.Z());
        Ray distributed_ray(o, fp-o);
        Color c = world->shade_ray(distributed_ray);
        c.clamp();
        rgb = rgb+c;
    }
    // Take the average of all the n rays
    return rgb/n;
}
