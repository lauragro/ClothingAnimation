#ifndef CAMERA_H
#define CAMERA_H

#include <QMouseEvent>
#include <QGraphicsView>
#include <glwidget.h>
#include "foundation.h"

class Camera : QGraphicsView
{

public:
    Camera();
    ~Camera();

    void startup();

    vec3 CameraPos;

public:

    //===========Camera stuff adapted from Lucky and 305 assignment===========
        float linelength;
        float vppos_x;
        float vppos_y;

        float camera_phi;
        float camera_phi_speed;
        float camera_theta;
        float camera_theta_speed;

        float dolly_factor;
        double lastX, lastY;
        bool left_pressed;
        bool right_pressed;

        void MouseMove(double x, double y);
        void MouseButton(QMouseEvent *e, bool press);

        void updatePos();
    //================================================================
};

#endif // CAMERA_H
