#include "camera.h"
#include <float.h>

// Constructor
Camera::Camera()
{
    linelength = 0.02f;
}

// Destructor
Camera::~Camera()
{
    cout << "camera destruction" << endl;
}

void Camera::startup()
{
    dolly_factor = 1000.0;
    CameraPos.x = 1;
    CameraPos.y = 1;
    CameraPos.z = 10;

    vppos_x = 0;
    vppos_y = 0;

    camera_phi = PI/2;
    camera_phi_speed = 0;
    camera_theta = 0;
    camera_theta_speed = 0;

    left_pressed = false;
    right_pressed = false;

}

//=====================Functions adapted from Lucky and 305 assignment======
void Camera::MouseMove(double x, double y)
{
   vppos_x = (float)(x) / 256 - 1;
   vppos_y = 1 - (float)(y) / 256;

   if (left_pressed)
   {
      camera_theta_speed = (float)(x - lastX) * - 0.01f; //?
      camera_phi_speed = (float)(y - lastY) * 0.01f;
   }

   if (right_pressed)
   {
      dolly_factor += (lastY - y);
      if (dolly_factor < 100) dolly_factor = 100;
      if (dolly_factor > 2000) dolly_factor = 2000;

   }

   lastX = x;
   lastY = y;
}

void Camera::MouseButton(QMouseEvent *e, bool press)
{
    if (e->button() == Qt::LeftButton)
    {
        if (press == true)
        {
            lastX = e->x();
            lastY = e->y();
            left_pressed = true;
        }
        else left_pressed = false;
    };

    if (e->button() == Qt::RightButton)
    {
        if (press == true)
        {
            lastX = e->x();
            lastY = e->y();
            right_pressed = true;
        }
        else right_pressed = false;
    };
}

void Camera::updatePos()
{
    CameraPos.x = dolly_factor * sin(camera_phi) * sin(camera_theta);
    CameraPos.y = dolly_factor * cos(camera_phi);
    CameraPos.z = dolly_factor * sin(camera_phi) * cos(camera_theta);
}

