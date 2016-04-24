#ifndef BALL_H
#define BALL_H

#include "foundation.h"
#include "particle.h"

class Ball
{
public:
    Ball(int zCentre);
    ~Ball();

    float radius;
    vec3 origin;

    void draw(GLuint texture);
    void solidSphere(GLdouble radius, GLint slices, GLint stacks);
    bool collidesWith(Particle * p, int number);

private:
    void drawTextures(GLuint texture);


};

#endif // BALL_H
