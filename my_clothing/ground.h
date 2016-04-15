#ifndef GROUND_H
#define GROUND_H

#include "foundation.h"
#include "particle.h"

class Ground
{
public:
    Ground(int height);
    ~Ground();

    draw(GLuint texture);
    void drawFixedGround();

    void initializeShader();
    bool collidesWith(Particle *p);

    // parameters for collision detection
    float y;    // Height
    float xmin;
    float xmax;
    float zmin;
    float zmax;
    float ymax;
    float ymin;

    float coefficientOfFriction;

    // normal vectors
    vec3 bottomNormal;
    vec3 frontNormal;
    vec3 rightNormal;
    vec3 topNormal;
    vec3 leftNormal;
    vec3 backNormal;

private:
    static const int width = 256;  // length of a side of the square ground

    //=====Shader Stuff===================
    void drawTextures(GLuint texture);
    //====================================

    // todo: fix this

    //==========================================
};

#endif // GROUND_H
