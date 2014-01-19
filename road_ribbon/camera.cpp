#include "camera.h"

Camera::Camera(Vector3d _position, Vector3d _lookAt)
{
    this->position = _position;
    this->lookAt = _lookAt;
    this->up = Vector3d(0, 1, 0);
    this->left = Vector3d(1, 0, 0);
    this->forward = Vector3d(0, 0, -10);
}

void Camera::move(Vector3d translate)
{
    Vector3d v1 = this->position;
    v1.y = this->lookAt.y;

    Vector3d tz = (this->lookAt - v1).normalize() * -translate.z;
    // Vector3d tx = (this->up.crossProduct(tz)).normalize() * translate.x;
    Vector3d ty = Vector3d(0, 1, 0) * translate.y;

    Vector3d delta = tz + ty;

    this->position += delta;
    this->lookAt += delta;
}

void Camera::rotate(Vector3d angles)
{
    this->position = (this->position - this->lookAt).rotate(this->up, angles.y) + this->lookAt;
    this->left = this->left.rotate(this->up, angles.y);
    this->forward = this->forward.rotate(this->up, angles.y);

    this->position = (this->position - this->lookAt).rotate(this->left, angles.x) + this->lookAt;
}

void Camera::render()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(this->position.x,
              this->position.y,
              this->position.z,
              this->lookAt.x,
              this->lookAt.y,
              this->lookAt.z,
              this->up.x,
              this->up.y,
              this->up.z
    );
}
