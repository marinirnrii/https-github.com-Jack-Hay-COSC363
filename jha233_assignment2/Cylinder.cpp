#include "Cylinder.h"
#include <math.h>

float Cylinder::intersect(glm::vec3 position, glm::vec3 dir)
{
    glm::vec3 d = position - center;
    float a = (dir.x * dir.x) + (dir.z * dir.z);
    float b = 2 * (dir.x * d.x + dir.z * d.z);
    float c = d.x * d.x + d.z * d.z - (radius * radius);
    
    //quadratic equations
    float quadratic1 = (-b - sqrt(b*b - 4*(a*c)))/(2 * a);
    float quadratic2 = (-b + sqrt(b*b - 4*(a*c)))/(2 * a);  
    
    float yPos = position.y + dir.y*quadratic2;

    if((yPos >= center.y) && (yPos <= center.y + height)){
        return quadratic1;
    } else if (quadratic2< 0.01) quadratic2 = -1;
    
    yPos = position.y + dir.y*quadratic1;
    if ((yPos >= center.y) && (yPos <= center.y + height)) {
        return quadratic2;
    } else {
        return -1;
    }
}

/**
* Returns the unit normal vector at a given point.
*/
glm::vec3 Cylinder::normal(glm::vec3 p)
{
    glm::vec3 normal = glm::vec3((p-center).x, 0, (p-center).z);
    return glm::normalize(normal); //normalize
}
