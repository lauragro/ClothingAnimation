#include <QtOpenGL>
#include "glwidget.h"
#include "sim.h"
#include "window.h"
#include <QKeyEvent>
#include <QKeySequence>

// simulation timer interval (millisec)
static int timer_interval = 10;

// Constructor
GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent)
{
    mySim = new Sim();
    myCamera = new Camera();
    startup();

    // set focus policy for responding to keyboard events
    this->setFocusPolicy(Qt::StrongFocus);
}

// Destructor
GLWidget::~GLWidget()
{
    cout << "widget deconstruction" << endl;
    delete mySim;
    delete myCamera;
}

// Connect to signals
void GLWidget::startup()
{
    timer = new QTimer( this );
    frameTimer = new QTimer( this);
    connect( timer, SIGNAL(timeout()), this, SLOT(advanceTime()) );
    connect( frameTimer, SIGNAL(timeout()), this, SLOT(advanceFrame()) );

    // initialize camera settings
    myCamera->startup();
}


// Initialize all graphics
void GLWidget::initializeGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* setup viewing *******************************/
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1,1,-0.75f,0.75f,1,5000);    // hard coding the view box ratio here!

    glMatrixMode(GL_MODELVIEW);

    // white background
    glClearColor(1, 1, 1, 1);

    /* setup shaders ********************************/
    initializeShader();

    /* Setup lighting ******************************/
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Create light components
    GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8, 1.0f };
    GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat position[] = { 1000.0f, 1000.0f, 1000.0f, 1.0f };

    // Assign created components to GL_LIGHT0
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    timer->start(50); // 20 fps: timer should run always, not tied to animation
    // frame timer is controlled by go and stop buttons

}

// Painting things
void GLWidget::paintGL()
{
    myCamera->updatePos();

    // Set the modelview matrix.
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    gluLookAt(myCamera->CameraPos.x, myCamera->CameraPos.y, myCamera->CameraPos.z,  // camera xyz
              0, 0, 0,       // target xyz
              0, -1, 0);     // up xyz

    // if a simulation is present, draw it
    if (mySim != NULL) mySim->draw(textures);

}


/* Slots */

// Advance frame logic (animation stuff)
void GLWidget::advanceFrame()
{
    // if a simulation is present, advance it
    if (mySim != NULL)
    {
        mySim->simStep();
    }

    // if the sim should be stopped, stop it
    if (simMode == false)
    {
        //timer->stop();// timer should run always, not tied to animation
        frameTimer->stop();
        cerr << "Simulation stopped \n";
    }

}

// Advance time logic (camera stuff)
void GLWidget::advanceTime()
{


    // Camera logic adapted from Lucky and 305 assignment
    myCamera->camera_phi += myCamera->camera_phi_speed;
    myCamera->camera_phi_speed *= 0.8;

    if (abs(myCamera->camera_phi_speed) < 1e-3)
    {
        myCamera->camera_phi_speed = 0;
    }

    myCamera->camera_theta += myCamera->camera_theta_speed;
    myCamera->camera_theta_speed *= 0.8;

    if (abs(myCamera->camera_theta_speed) < 1e-3)
    {
        myCamera->camera_theta_speed = 0;
    }

    updateGL();

}

// Go button Logic
void GLWidget::button_go()
{
    simMode = true;
    frameTimer->start( timer_interval );

    cerr << "Button GO" << endl;
}

// Stop button logic
void GLWidget::button_stop()
{
    simMode = false;
    frameTimer->stop();

    cerr << "Button STOP" << endl;

}

// Reset button logic
void GLWidget::button_reset()
{
    button_stop();      // end current simulation

    mySim->initialize();
    //mySim = new Sim();  // create new simulation
    initializeShader(); // reset grass etc.
    startup();          // start timers
    advanceFrame();     // draw starting position

    //wait for button_go
}

/* 2D */
void GLWidget::resizeGL( int winw, int winh )
{
    cerr << "resizing\n";
    glViewport( 0, 0, winw, winh );
    cerr << "width = " << winw << endl;
    cerr << "height = " << winh << endl;
}

/********** React to mouse buttons ***********/
void GLWidget::mousePressEvent(QMouseEvent *e)
{
    myCamera->MouseButton(e, true);
}

void GLWidget::mouseReleaseEvent(QMouseEvent *e)
{
    myCamera->MouseButton(e, false);
}

void GLWidget::mouseMoveEvent(QMouseEvent *e)
{
    myCamera->MouseMove(e->x(), e->y());
}

/*********** React to keyboard buttons ********/
void GLWidget::keyPressEvent(QKeyEvent *k)
{
    // Amount to move per step
    float stepSize = 10.0f;
    mySim->stepSize = stepSize+1.0f;

    // Left arrow
    if(k->matches(QKeySequence::MoveToPreviousChar))
    {
        vec3 p = vec3(mySim->myPerson->origin.x + stepSize,
                      mySim->myPerson->origin.y,
                      mySim->myPerson->origin.z);

        mySim->myPerson->setOrigin(p);

        mySim->personMoved = true;
        mySim->directionMoved = vec3(1.1f,0.0f,0.0f);

        cout << "LEFT ARROW" << endl;

    }
    // Right arrow
    else if(k->matches(QKeySequence::MoveToNextChar))
    {
        vec3 p = vec3(mySim->myPerson->origin.x - stepSize,
                      mySim->myPerson->origin.y,
                      mySim->myPerson->origin.z);

        mySim->myPerson->setOrigin(p);

        mySim->personMoved = true;
        mySim->directionMoved = vec3(-1.1f,0.0f,0.0f);

        cout << "RIGHT ARROW" << endl;

    }
    // Up arrow
    else if(k->matches(QKeySequence::MoveToPreviousLine))
    {
        vec3 p = vec3(mySim->myPerson->origin.x,
                      mySim->myPerson->origin.y,
                      mySim->myPerson->origin.z - stepSize);

        mySim->myPerson->setOrigin(p);

        mySim->personMoved = true;
        mySim->directionMoved = vec3(0.0f,0.0f,-1.1f);


        cout << "UP ARROW" << endl;

    }
    // Down arrow
    else if(k->matches(QKeySequence::MoveToNextLine))
    {
        vec3 p = vec3(mySim->myPerson->origin.x,
                      mySim->myPerson->origin.y,
                      mySim->myPerson->origin.z + stepSize);

        mySim->myPerson->setOrigin(p);

        mySim->personMoved = true;
        mySim->directionMoved = vec3(0.0f,0.0f,1.1f);

        cout << "DOWN ARROW" << endl;

    }

}


//========TEXTURES================================================
void GLWidget::Load2DGLTexture( const char * name, const int texID )
{
    QImage img;

    if(!img.load(name)){
        std::cerr << "ERROR in loading image" << std::endl;
    }

    QImage t = QGLWidget::convertToGLFormat(img);

    glGenTextures(1, &textures[texID]);
    glBindTexture(GL_TEXTURE_2D, textures[texID]);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, t.width(), t.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, t.bits());

        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glDisable(GL_TEXTURE_2D);
}

void GLWidget::LoadSphereGLTexture( const char * name, const int texID )
{
    QImage img;

    if(!img.load(name))
    {
        std::cerr << "ERROR in loading image" << std::endl;
    }

    QImage t = QGLWidget::convertToGLFormat(img);

    glGenTextures(1, &textures[texID]);
    glBindTexture(GL_TEXTURE_2D, textures[texID]);  // THIS TEXTURE APPEARS WHEN GL_TEXTURE_2D IS USED INSTEAD
        glTexImage2D(GL_TEXTURE_2D, 0, 3, t.width(), t.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, t.bits());
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    glDisable(GL_TEXTURE_2D);
}

void GLWidget::initializeShader()
{
    //glEnable(GL_CULL_FACE);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_TEXTURE_2D);
    //glEnable(GL_FRAMEBUFFER_SRGB);  // proper colour balancing


    //Load2DGLTexture("../American_oak_pxr128.tif",0);
    Load2DGLTexture("../Gray_marble_pxr128_bmp.tif",0);

    //LoadSphereGLTexture("../volleyball.jpg",1);

    LoadSphereGLTexture("../Penguin-Face.png",1);
    LoadSphereGLTexture("../Penguin-Body.png",2);

}
