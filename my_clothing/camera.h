#ifndef CAMERA_H
#define CAMERA_H

#include <QMouseEvent>
#include <QGraphicsView>
#include <openglwidget.h>
#include "foundation.h"

class Camera : QGraphicsView
{
    //Q_OBJECT

public:
    Camera();
    ~Camera();

    void startup(float winw, float winh);

    // Constants for camera motion
    /*const double lim=0.5;
    const double RadPerPixel = - 0.01;
    const double MovePerPixel = - 0.1;
*/
    // matrix place holders so we don't keep creating variables
    //glm::mat3 matrix(float);
    //float m11,m12,m13,m21,m22,m23,m31,m32,m33;

    //variables
    //float winw, winh, xangle, yangle, zangle, scale;
    glm::vec3 CameraPos;
    //QPoint lastMousePoint;
    //bool Rotating, Scaling;

    //==============Camera movement mouse events functions ===============
        /*void rotx(int);
        void roty(int);
        void rotz(int);
        void setxFrom(int a);
        void setyFrom(int a);
        void setzFrom(int a);
        void RotateY(float rad);
        void RotateZ(float rad);
        void DoRotate(QPoint desc, QPoint orig);
        void DoScale(QPoint desc, QPoint orig);*/
   //=====================================================================

public:
    //==============Mouse Event Handling =============================
        /*void mousePressEvent(QMouseEvent *e);
        void mouseReleaseEvent(QMouseEvent *e);
        void mouseMoveEvent(QMouseEvent *e);*/
    //================================================================

    //===========Camera stuff from Lucky and 305 assignment===========
        unsigned int width;
        unsigned int height;
        float linelength;// = 0.02f;
        float vppos_x;
        float vppos_y;

        float camera_phi;
        float camera_phi_speed;
        float camera_theta;
        float camera_theta_speed;

        //float camera_radius;
        float dolly_factor;
        double lastX, lastY;
        bool left_pressed;
        bool right_pressed;

        mat4 M_orth, M_persp, M_cam;

        void MouseMove(double x, double y);
        void MouseButton(QMouseEvent *e, bool press);

        void updatePos();
    //================================================================
};

#endif // CAMERA_H
