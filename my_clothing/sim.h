// Interface for simulation class

#ifndef SIM_H
#define SIM_H

#include "foundation.h"
#include "spring.h"
#include "particle.h"
#include "flag.h"
#include "ball.h"
#include "ground.h"
#include "person.h"

class Flag;
class Spring;
//class vec3;

class Sim
{
    public:
        // Constructor/destructor
        Sim();
        ~Sim();

        // Draws all graphics
        void draw(GLuint * textures);

        // Progesses the simulation forward
        void simStep();
        void rightStep();
        void eulerStep(float dt);
        void semiImplicitEuler(float dt);                 // integrate one time step with s.i.e.
        void rungeKuttaStep(float dt);
        void updateForces(int number);

        // for fixing collisions when person moves
        bool personMoved;
        vec3 directionMoved;
        float stepSize;

        // Check for collisions of one particle
        //bool collidesWithGround(Particle * thisParticle, int number);

        // The components belonging to this simulation
        Flag * myFlag;
        //Ball * myBall;
        Ground * myGround;
        Person * myPerson;

        // Time of sim
        float t;
};

#endif // SIM_H
