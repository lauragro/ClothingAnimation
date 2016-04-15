#include "sim.h"
#include "foundation.h"

// Constructor
Sim::Sim()
{
    cout << "Simulator Initialization\n";

    //myBall = new Ball(0);

    myPerson = new Person();
    personMoved = false;    // in test stage now
    myFlag = new Flag(0, myPerson->body->origin.y - myPerson->body->radius);

    myGround = new Ground(myPerson->body->origin.y + myPerson->body->radius);

    t = 0;

}

// Destructor
Sim::~Sim()
{
    cout << "Simulator Destruction\n";
    delete myFlag;
}

// Draw everything on screen
void Sim::draw(GLuint * textures)
{
    // Draw ground
    myGround->draw(textures[0]);  // for desktop
    //myGround->drawFixedGround();    // for laptop


    // Draw blue circles
    /*for( int i=0; i<myFlag->particlesHigh; i++ )
    {
        for(int j=0; j<myFlag->particlesWide; j++)
        {
            myFlag->particles[i][j]->draw();
        }
    }*/

    // Draw ball
    //myBall->draw(textures[1]);

    // Draw person
    myPerson->draw(textures[1],textures[2]);

    // Draw connecting lines
    /*for( int i=0; i<myFlag->implementedSprings; i++ )
    {
        myFlag->springs[i]->draw(); // for laptop
    }*/

    // Draw flag
    myFlag->draw(); // for desktop

}

// Simulation step using interpolation
void Sim::simStep()
{
    int type = 2;
    float dt = 0.1f;//2f;
    t += dt;

    switch (type)
    {
        case 0:
            rightStep();
            break;
        case 1:
            eulerStep(dt);
            break;
        case 2:
            semiImplicitEuler(dt);
            break;
        default://case 2:
            rungeKuttaStep(dt); // This fails after multiple collisions!
            //break;
    }

}

// Move right simulation step
void Sim::rightStep()
{
    for(int i=0; i<myFlag->particlesHigh; i++)
    {
        for(int j=0; j<myFlag->particlesWide; j++)
        {
            myFlag->particles[i][j]->position.x += 10;
        }
    }
}

// Euler simulation step
void Sim::eulerStep(float dt)
{
    int i,j;
    vec3 x, v, a;

    if(personMoved)
    {
        for( i=0; i<myFlag->particlesHigh; i++ )
        {
            for( j=0; j<myFlag->particlesWide; j++ )
            {
                if(myPerson->collidesWith(myFlag->particles[i][j]))
                {
                    myFlag->particles[i][j]->position += (directionMoved * stepSize);
                    myFlag->particles[i][j]->pinned = true;
                }
                else
                {
                    myFlag->particles[i][j]->pinned = false;
                }
            }
        }

        // reset flag
        personMoved = false;
    }


    // update forces on particles
    updateForces(0);

    // update velocities and positions of particles
    for( i=0; i<myFlag->particlesHigh; i++ )
    {
        for( j=0; j<myFlag->particlesWide; j++ )
        {
            // pin the corners
            /*if(myFlag->particles[i][j]->pinned)
            {
                // set velocity to zero
                //myFlag->particles[i][j]->velocity=vec3(0.0f,0.0f,0.0f);

                //continue;

            // adjust for collision with person
            } else*/ if(myPerson->collidesWith(myFlag->particles[i][j])){

                // set velocity to zero
                //myFlag->particles[i][j]->velocity=vec3(0.0f,0.0f,0.0f);

                // move particle to outside the person
                if(myFlag->particles[i][j]->position.y >= myPerson->head->origin.y + myPerson->head->radius)
                {
                    // bump to outside the body
                    myFlag->particles[i][j]->position = myPerson->body->origin
                            + (normalize(myFlag->particles[i][j]->position - myPerson->body->origin)
                            * myPerson->body->radius * 1.1f);
                } else {
                    // bump to outside head
                    myFlag->particles[i][j]->position = myPerson->head->origin
                            + (normalize(myFlag->particles[i][j]->position - myPerson->head->origin)
                            * myPerson->head->radius * 1.1f);
                }

                continue;

            // adjust for collision with ground
            } else if(myGround->collidesWith(myFlag->particles[i][j])){

                // set velocity to zero
                //myFlag->particles[i][j]->velocity=vec3(0.0f,0.0f,0.0f);

                continue;

            // update all other particles
            }
            else
            {
                // calculate acceleration: a = F/m where m=1
                a = myFlag->particles[i][j]->force;

                // get velocity of spring
                v = myFlag->particles[i][j]->velocity;

                // update velocity
                v = v + dt*a;
                myFlag->particles[i][j]->velocity = v;

                // get position of spring
                x = myFlag->particles[i][j]->position;

                // update position
                x = x + v*dt;// + 0.5f * a * dt * dt;

                myFlag->particles[i][j]->position = x;

                // ignore very small velocities
                if(length(v) <= EPSILON)
                {
                    myFlag->particles[i][j]->velocity = vec3(0.0f,0.0f,0.0f);
                }

            }

        }

    }


    // adjust for any collisions that have taken place this time step
    /*for( i=0; i<myFlag->particlesHigh; i++ )
    {
        for( j=0; j<myFlag->particlesWide; j++)
        {
            if(myPerson->collidesWith(myFlag->particles[i][j])){

                // set velocity to zero
                //myFlag->particles[i][j]->velocity=vec3(0.0f,0.0f,0.0f);

                // move particle to outside the person
                /*if(myFlag->particles[i][j]->position.y >= myPerson->head->origin.y + myPerson->head->radius)
                {
                    // bump to outside the body
                    myFlag->particles[i][j]->position = myFlag->particles[i][j]->position
                            + normalize(myFlag->particles[i][j]->position - myPerson->body->origin)
                            * (myPerson->body->radius - length(myFlag->particles[i][j]->position - myPerson->body->origin));
                } else {
                    // bump to outside head
                    myFlag->particles[i][j]->position = myFlag->particles[i][j]->position
                            + ( normalize(myFlag->particles[i][j]->position - myPerson->head->origin)
                            * (myPerson->head->radius - length(myFlag->particles[i][j]->position - myPerson->head->origin)) );
                }*/

                /*if(myFlag->particles[i][j]->position.y >= myPerson->head->origin.y + myPerson->head->radius)
                {
                    // bump to outside the body
                    myFlag->particles[i][j]->position = myPerson->body->origin
                            + (normalize(myFlag->particles[i][j]->position - myPerson->body->origin)
                            * myPerson->body->radius * 1.1f);
                } else {
                    // bump to outside head
                    myFlag->particles[i][j]->position = myPerson->head->origin
                            + (normalize(myFlag->particles[i][j]->position - myPerson->head->origin)
                            * myPerson->head->radius * 1.1f);
                }
            }
        }
    }*/



}

// Semi-Implicit Euler simulation step
void Sim::semiImplicitEuler(float dt)
{
    int i,j;
    vec3 x,v,a,x_prime,v_prime,a_prime;
    Particle *p;

    // Check if user moved character
    if(personMoved)
    {
        for( i=0; i<myFlag->particlesHigh; i++ )
        {
            for( j=0; j<myFlag->particlesWide; j++ )
            {
                if(myPerson->collidesWith(myFlag->particles[i][j]))
                {
                    myFlag->particles[i][j]->position += (directionMoved * stepSize);
                    //myFlag->particles[i][j]->pinned = true;
                }
                else
                {
                    //myFlag->particles[i][j]->pinned = false;
                }
            }
        }

        // reset flag
        personMoved = false;
    }


    // get values for current x,v,a
    updateForces(0);
    for(i=0; i<myFlag->particlesHigh; i++)
    {
        for(j=0; j<myFlag->particlesWide; j++)
        {
            p = myFlag->particles[i][j];

            if(p->pinned)
            {
                p->position = myPerson->body->origin - vec3(0.0f,myPerson->body->radius,0.0f);

                continue;
            }

            else if(myPerson->collidesWith(p)){//myFlag->particles[i][j])){

                // set velocity to zero
                //myFlag->particles[i][j]->velocity=vec3(0.0f,0.0f,0.0f);

                // move particle to outside the person
                if(p->position.y >= myPerson->head->origin.y + myPerson->head->radius)
                //if(myFlag->particles[i][j]->position.y >= myPerson->head->origin.y + myPerson->head->radius)
                {
                    // bump to outside the body
                    p->position = myPerson->body->origin
                            + (normalize(p->position - myPerson->body->origin)
                            * myPerson->body->radius * 1.1f);
                    //myFlag->particles[i][j]->position = myPerson->body->origin
                    //        + (normalize(myFlag->particles[i][j]->position - myPerson->body->origin)
                    //        * myPerson->body->radius * 1.1f);
                } else {
                    // bump to outside head
                    p->position = myPerson->head->origin
                            + (normalize(p->position - myPerson->head->origin)
                            * myPerson->head->radius * 1.1f);
                    //myFlag->particles[i][j]->position = myPerson->head->origin
                    //        + (normalize(myFlag->particles[i][j]->position - myPerson->head->origin)
                    //        * myPerson->head->radius * 1.1f);
                }

                continue;

            // adjust for collision with ground
            } else if(myGround->collidesWith(myFlag->particles[i][j])){

                // set velocity to zero
                //myFlag->particles[i][j]->velocity=vec3(0.0f,0.0f,0.0f);

                continue;

            // update all other particles
            }
            else
            {
                // values at current time
                x = p->position_old;
                v = p->velocity_old;
                a = p->acceleration_old;

                // euler to approximate future values
                v_prime = v + dt * a;
                x_prime = x + dt * v_prime;
                a_prime = p->acceleration;

                // use approx future values to find s.e.i. result
                v = v + dt * a_prime;
                p->velocity = v;
                p->position += v*dt;
                p->acceleration = a_prime;

                p->position1 = x_prime;
                p->velocity1 = v_prime;

            }

        }

    }

    // get values for next x,v,a
    /*updateForces(1);
    for(i=0; i<myFlag->particlesHigh; i++)
    {
        for(j=0; j<myFlag->particlesWide; j++)
        {

                p = myFlag->particles[i][j];
                // use approx future values to find s.e.i. result
                x = p->position;
                v = p->velocity;
                a_prime = p->acceleration1;

                v = v + dt * a_prime;
                //x = x + dt * v;
                p->velocity = v;
                p->position = x + v*dt;
                p->acceleration = a_prime;



        }

    }*/
}

// Runge Kutta 4th Order simulation step
void Sim::rungeKuttaStep(float dt)
{
    vec3 x,v, dx1, dx2, dx3, dx4, dv1, dv2, dv3, dv4;
    int i,j;

    // Step 1
    updateForces(0);
    for(i=0; i<myFlag->particlesHigh; i++)
    {
        for(j=0; j<myFlag->particlesWide; j++)
        {
            // pin the corners and detect collisions
            if(myFlag->particles[i][j]->pinned
                    || myPerson->collidesWith(myFlag->particles[i][j]) || myGround->collidesWith(myFlag->particles[i][j]))
            {
                // set velocity to zero
                myFlag->particles[i][j]->velocity=vec3(0.0f,0.0f,0.0f);
                myFlag->particles[i][j]->velocity1=vec3(0.0f,0.0f,0.0f);
                myFlag->particles[i][j]->velocity2=vec3(0.0f,0.0f,0.0f);
                myFlag->particles[i][j]->velocity3=vec3(0.0f,0.0f,0.0f);

                continue;   // skip euler for this particle
            }

            // update all other particles
            else
            {
                // capture current position and velocity
                x = myFlag->particles[i][j]->position;
                v = myFlag->particles[i][j]->velocity;

                // calculate RK4 values
                dx1 = dt * v;
                dv1 = dt * myFlag->particles[i][j]->force;  //acceleration(x,T);

                // store intermediate values
                myFlag->particles[i][j]->position1 = x + dx1/2.0f;
                myFlag->particles[i][j]->velocity1 = v + dv1/2.0f;
            }
        }
    }

    // Step 2
    updateForces(1);
    for(i=0; i<myFlag->particlesHigh; i++)
    {
        for(j=0; j<myFlag->particlesWide; j++)
        {
            // pin the corners and check for collisions
            if(myFlag->particles[i][j]->pinned
                    || myPerson->collidesWith(myFlag->particles[i][j]) || myGround->collidesWith(myFlag->particles[i][j]))
            {
                // set velocity to zero
                myFlag->particles[i][j]->velocity=vec3(0.0f,0.0f,0.0f);
                myFlag->particles[i][j]->velocity1=vec3(0.0f,0.0f,0.0f);
                myFlag->particles[i][j]->velocity2=vec3(0.0f,0.0f,0.0f);
                myFlag->particles[i][j]->velocity3=vec3(0.0f,0.0f,0.0f);

                continue;   // skip euler for this particle
            }

            // update all other particles
            else
            {
                // capture current position and velocity
                x = myFlag->particles[i][j]->position;
                v = myFlag->particles[i][j]->velocity;

                // calculate RK4 values
                dx2 = dt * (v + dv1/2.0f);
                dv2 = dt * myFlag->particles[i][j]->force;  //acceleration(x + dx1/2.0f, T + dt/2.0f);

                // store intermediate values
                myFlag->particles[i][j]->position2 = x + dx2/2.0f;
                myFlag->particles[i][j]->velocity2 = v + dv2/2.0f;
            }
        }
    }

    // Step 3
    updateForces(2);
    for(i=0; i<myFlag->particlesHigh; i++)
    {
        for(j=0; j<myFlag->particlesWide; j++)
        {
            // pin the corners and check for collisions
            if(myFlag->particles[i][j]->pinned
                    || myPerson->collidesWith(myFlag->particles[i][j]) || myGround->collidesWith(myFlag->particles[i][j]))
            {
                // set velocity to zero
                myFlag->particles[i][j]->velocity=vec3(0.0f,0.0f,0.0f);
                myFlag->particles[i][j]->velocity1=vec3(0.0f,0.0f,0.0f);
                myFlag->particles[i][j]->velocity2=vec3(0.0f,0.0f,0.0f);
                myFlag->particles[i][j]->velocity3=vec3(0.0f,0.0f,0.0f);

                continue;   // skip euler for this particle
            }

            // update all other particles
            else
            {
                // capture current position and velocity
                x = myFlag->particles[i][j]->position;
                v = myFlag->particles[i][j]->velocity;

                // calculate RK4 values
                dx3 = dt * (v + dv2/2.0f);
                dv3 = dt *  myFlag->particles[i][j]->force;    //acceleration(x + dx2/2.0f, T + dt/2.0f);

                // store intermediate values
                myFlag->particles[i][j]->position3 = x + dx3;
                myFlag->particles[i][j]->velocity3 = v + dv3;
            }
        }
    }

    // Step 4
    updateForces(3);
    for(i=0; i<myFlag->particlesHigh; i++)
    {
        for(j=0; j<myFlag->particlesWide; j++)
        {
            // pin the corners and check for collisions
            if(myFlag->particles[i][j]->pinned
                    || myPerson->collidesWith(myFlag->particles[i][j]) || myGround->collidesWith(myFlag->particles[i][j]))
            {
                // set velocity to zero
                myFlag->particles[i][j]->velocity=vec3(0.0f,0.0f,0.0f);
                myFlag->particles[i][j]->velocity1=vec3(0.0f,0.0f,0.0f);
                myFlag->particles[i][j]->velocity2=vec3(0.0f,0.0f,0.0f);
                myFlag->particles[i][j]->velocity3=vec3(0.0f,0.0f,0.0f);

                continue;   // skip euler for this particle
            }

            // update all other particles
            else
            {
                // capture current position and velocity
                x = myFlag->particles[i][j]->position;
                v = myFlag->particles[i][j]->velocity;

                // calculate RK4 values
                dx4 = dt * (v + dv3);
                dv4 = dt * myFlag->particles[i][j]->force;    // acceleration(x + dx3, T + dt);

                // update final positions and velocities
                myFlag->particles[i][j]->position = x + dx1/6.0f + dx2/3.0f + dx3/3.0f + dx4/6.0f;
                myFlag->particles[i][j]->velocity = v + dv1/6.0f + dv2/3.0f + dv3/3.0f + dv4/6.0f;
            }
        }
    }
}


// Update forces on all particles
void Sim::updateForces(int number)
{
    int i,j;
    glm::vec3 force, pos, v, normalForce, frictionForce;

    float dangerSquare[4] = {myPerson->origin.x - myPerson->body->radius,  // xmin
                            myPerson->origin.x + myPerson->body->radius,  // mxax
                            myPerson->origin.z - myPerson->body->radius,  // zmin
                            myPerson->origin.z + myPerson->body->radius}; // zmax

    // Reset forces
    for( i=0; i<myFlag->particlesHigh; i++ )
    {
        for( j=0; j<myFlag->particlesWide; j++ )
        {
            // update old positions/velocities/accelerations
            myFlag->particles[i][j]->position_old = myFlag->particles[i][j]->position;
            myFlag->particles[i][j]->velocity_old = myFlag->particles[i][j]->velocity;
            myFlag->particles[i][j]->acceleration_old = myFlag->particles[i][j]->acceleration;

            // reset spring force for next calculation
            myFlag->particles[i][j]->springForce = vec3(0.0f,0.0f,0.0f);

            // add dampening force externally
            switch(number){
            case 0:
                v = myFlag->particles[i][j]->velocity;
                pos = myFlag->particles[i][j]->position;
                break;
            case 1:
                v = myFlag->particles[i][j]->velocity1;
                pos = myFlag->particles[i][j]->position1;
                break;
            case 2:
                v = myFlag->particles[i][j]->velocity2;
                pos = myFlag->particles[i][j]->position2;
                break;
            default://case 3:
                v = myFlag->particles[i][j]->velocity3;
                pos = myFlag->particles[i][j]->position3;
                //break;
            }

            // Fext = Fgravity + Fdamp + Fwind
            myFlag->particles[i][j]->externalForce = myFlag->particles[i][j]->gravityForce - v * myFlag->dampingConstant;
                    //+ 20.0f*vec3(abs(sin(t/10)), 0, 0); // wind;

            // Fext += Fn + Ff if collisions occur
            if(myPerson->collidesWith(myFlag->particles[i][j]))
            {

                myFlag->particles[i][j]->velocity = myPerson->velocity;

                // compute the response force
                if(pos.y > myPerson->head->origin.y + myPerson->head->radius)
                //if(myFlag->particles[i][j]->position.y > myPerson->head->origin.y + myPerson->head->radius)
                {
                    normalForce = -1.0f * normalize(pos - myPerson->body->origin)
                            * dot(myFlag->particles[i][j]->force, normalize(pos - myPerson->body->origin))
                            * length(pos - myPerson->body->origin);
                    //normalForce = -1.0f * normalize(myFlag->particles[i][j]->position - myPerson->body->origin)
                    //        * dot(myFlag->particles[i][j]->force, normalize(myFlag->particles[i][j]->position - myPerson->body->origin))
                    //        * length(myFlag->particles[i][j]->position - myPerson->body->origin);
                } else {
                    normalForce = -1.0f * normalize(pos - myPerson->head->origin)
                            * dot(myFlag->particles[i][j]->force, normalize(pos - myPerson->head->origin))
                            * length(pos - myPerson->head->origin);
                    //normalForce = -1.0f * normalize(myFlag->particles[i][j]->position - myPerson->head->origin)
                    //        * dot(myFlag->particles[i][j]->force, normalize(myFlag->particles[i][j]->position - myPerson->head->origin))
                    //        * length(myFlag->particles[i][j]->position - myPerson->head->origin);
                }

                myFlag->particles[i][j]->externalForce += normalForce;

                // compute the friction
                frictionForce = -1.0f * myPerson->coefficientOfFriction * length(normalForce) * normalize(myFlag->particles[i][j]->force);

                // limit the friction so it doesn't move the particle
                if(length(frictionForce) >= length(myFlag->particles[i][j]->force))//dot(myFlag->particles[i][j]->force, frictionForce))
                {
                    frictionForce = -1.0f * myFlag->particles[i][j]->force;   // dot(myFlag->particles[i][j]->force, frictionForce);
                    //myFlag->particles[i][j]->velocity = myPerson->velocity; // velocity is equal to person if friction stops it
                }

                myFlag->particles[i][j]->externalForce += frictionForce;
            }

            if(myGround->collidesWith(myFlag->particles[i][j])
                    && (   myFlag->particles[i][j]->position.x < dangerSquare[0]    // add forces only when outside the danger square
                        || myFlag->particles[i][j]->position.x > dangerSquare[1]
                        || myFlag->particles[i][j]->position.z < dangerSquare[2]
                        || myFlag->particles[i][j]->position.z > dangerSquare[3]) )
            {
                // compute the response force
                normalForce = myGround->topNormal * dot(myFlag->particles[i][j]->force, myGround->topNormal);
                myFlag->particles[i][j]->externalForce += normalForce;

                // compute the friction
                frictionForce = -1.0f * myGround->coefficientOfFriction * length(normalForce) * normalize(myFlag->particles[i][j]->force);

                // limit the friction so it doesn't move the particle
                if(length(frictionForce) >= length(myFlag->particles[i][j]->force)) // dot(myFlag->particles[i][j]->force, frictionForce))
                {
                    frictionForce = -1.0f * myFlag->particles[i][j]->force; // dot(myFlag->particles[i][j]->force, frictionForce);

                    switch(number){
                    case 0:
                        myFlag->particles[i][j]->velocity = vec3(0.0f,0.0f,0.0f);

                        break;
                    case 1:
                        myFlag->particles[i][j]->velocity1 = vec3(0.0f,0.0f,0.0f);
                        break;
                    case 2:
                        myFlag->particles[i][j]->velocity2 = vec3(0.0f,0.0f,0.0f);
                        break;
                    default://case 3:
                        myFlag->particles[i][j]->velocity3 = vec3(0.0f,0.0f,0.0f);
                    }


                }

                myFlag->particles[i][j]->externalForce += frictionForce;
            }

        }

    }

    // Add on current spring forces
    for( i=0; i<myFlag->implementedSprings; i++ )
    {
        // get current spring force
        force = myFlag->springs[i]->force(number);

        // add current spring force to end particles
        myFlag->springs[i]->particle1->springForce += force;
        myFlag->springs[i]->particle2->springForce -= force;

    }

    // Update total forces on particles
    for( i=0; i<myFlag->particlesHigh; i++ )
    {
        for( j=0; j<myFlag->particlesWide; j++ )
        {
            // F = Fext + Fspring
            force = myFlag->particles[i][j]->externalForce + myFlag->particles[i][j]->springForce;
            myFlag->particles[i][j]->force = force;

            switch(number){
            case 0:
                myFlag->particles[i][j]->acceleration = force;  // mass = 1
                break;
            default:
                myFlag->particles[i][j]->acceleration1 = force; // mass = 1
                //break;
            }
        }

    }

    //cout << "(" << myFlag->particles[16][16]->force.x << ", " << myFlag->particles[16][16]->force.y
    //      << ", " << myFlag->particles[16][16]->force.z << ")" << endl;

}
