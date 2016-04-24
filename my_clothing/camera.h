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

    glm::vec3 CameraPos;

public:

    //===========Camera stuff from Lucky and 305 assignment===========
        unsigned int width;
        unsigned int height;
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

        mat4 M_orth, M_persp, M_cam;

        void MouseMove(double x, double y);
        void MouseButton(QMouseEvent *e, bool press);

        void updatePos();
    //================================================================
};

#endif // CAMERA_H
