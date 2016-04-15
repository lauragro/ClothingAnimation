#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QGLWidget>
#include "camera.h"
#include "sim.h"
class Camera;
class Sim;

class OpenGLWidget : public QGLWidget
{
    Q_OBJECT


public:
    // Constructor
    OpenGLWidget(QWidget *parent = 0);

    // Destructor
    ~OpenGLWidget();


protected:
    // Initialize graphics engine
    void initializeGL();

    // All painting things
    void paintGL();
    void draw();

    // Resizing
    void resizeGL(int width, int height);

public:
    Sim * mySim;
    Camera * myCamera;
    bool simMode;
    QTimer * timer;
    QTimer * frameTimer;
    void startup();


public:
    Sim * getMySim(){return mySim;}
    Camera * getMyCamera(){return myCamera;}

public slots:
    void button_go();
    void button_stop();
    void button_reset();

    void advanceTime(void);
    void advanceFrame(void);
    void endit(){exit(0);}

protected:
    //==============Mouse Event Handling =============================
        void mousePressEvent(QMouseEvent *e);
        void mouseReleaseEvent(QMouseEvent *e);
        void mouseMoveEvent(QMouseEvent *e);
    //==============Key Event Handling ===============================
        void keyPressEvent(QKeyEvent *k);

private:
        //=====Shader Stuff===================
        GLuint textures[2];

        void initializeShader();
        void Load2DGLTexture(const char * name, const int texID);
        void LoadSphereGLTexture( const char * name, const int texID );
        //====================================

};
#endif // OPENGLWIDGET_H
