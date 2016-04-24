#include "ball.h"

// Constructor
Ball::Ball(int zCentre)
{
    radius = 100.0f;
    origin = vec3(0,0,zCentre);
}

// Destructor
Ball::~Ball()
{

}

// Draw function (options for with/without textures)
void Ball::draw(GLuint texture)
{
    glMatrixMode(GL_MODELVIEW);

    // get the coordinates and radius
    float x = origin.x;
    float y = origin.y;
    float z = origin.z;
    float resolution = 20.0f;

    // Compute angle of each triangle in fan
    float theta = 2.0f * PI / resolution;

    glColor3f(0.1,0.3,0.6);

    // Uncomment to draw WITHOUT textures
    /*int numSlices = 32;
    int numStacks = 8;
    GLUquadricObj* pQuadric = gluNewQuadric();  // make the quadric
    assert(pQuadric!=NULL); // make sure the quadric exists
    glTranslatef(origin.x, origin.y, origin.z); // move sphere to ball's origin
    gluSphere(pQuadric,radius,numSlices,numStacks); // draw the sphere*/

    // Uncomment to draw WITH textures
    drawTextures(texture);

}

// draw a solid sphere
void Ball::solidSphere(GLdouble radius, GLint slices, GLint stacks)
{
    glPushMatrix(); // remember current matrix
    glTranslatef(origin.x, origin.y, origin.z); // move to ball's origin

    glBegin(GL_LINE_LOOP);
        GLUquadricObj* quadric = gluNewQuadric();
        assert(quadric!=NULL); // make sure the quadric exists
        gluQuadricDrawStyle(quadric, GLU_FILL);
        gluSphere(quadric, radius, slices, stacks);
        gluDeleteQuadric(quadric);
    glEnd();

    glPopMatrix();  // restore old matrix
}

// Check for collision of one particle with ball
bool Ball::collidesWith(Particle * p, int number)
{
    // find distance from ball's origin
    vec3 distanceVector;
    switch(number){
    case 0:
        distanceVector = p->position - origin;
        break;
    case 1:
        distanceVector = p->position1 - origin;
        break;
    case 2:
        distanceVector = p->position2 - origin;
        break;
    default:
        distanceVector = p->position3 - origin;
    }

    float distance = length(distanceVector);

    if( distance < radius * 1.1f )  // <=
    {
        // collision detected
        return true;
    }

    // no collision detected
    return false;
}


// Draw ball with textures
void Ball::drawTextures(GLuint texture)
{
    glPushMatrix();
    glTranslatef(origin.x, origin.y, origin.z);
    glShadeModel( GL_SMOOTH );
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_TEXTURE_2D);
        glColor3f(1, 1, 1);
        glBindTexture(GL_TEXTURE_2D, texture);

            int numSlices = 32;
            int numStacks = 32;
            GLUquadricObj* sphere = gluNewQuadric();  // make the quadric
            assert(sphere!=NULL); // make sure the quadric exists

            gluQuadricDrawStyle(sphere,GLU_FILL);
            gluQuadricNormals(sphere,GLU_SMOOTH);
            gluQuadricOrientation(sphere,GLU_OUTSIDE);
            gluQuadricTexture(sphere,GL_TRUE);
            gluSphere(sphere,radius,numSlices,numStacks); // draw the sphere
            gluDeleteQuadric(sphere);

        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
}
