#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"
#include "vector3.h"

class Camera {
   public:
    Camera() {
        position = Vector3<double>{0.0, 0.0, 0.0};
        lookAtPosition = Vector3<double>{0.0, 0.0, -2.0};
        up = Vector3<double>{0.0, 1.0, 0.0};
        distFromScene = 1.0;
        horizSize = 1.0;
        aspectRatio = 1.0;

        updateCameraGeometry();
    }
    Camera(double _x, double _y, double _z) : position(Vector3<double>{_x, _y, _z}) {}

    Vector3<double> getPosition() {
        return position;
    }

    void setPosition(Vector3<double> const &v) {
        position = v;
    }

    Vector3<double> getUp() {
        return up;
    }

    void setUp(Vector3<double> const &v) {
        up = v;
    }

    Vector3<double> getLookAtPosition() {
        return lookAtPosition;
    }

    void setLookAtPosition(Vector3<double> const &v) {
        lookAtPosition = v;
    }

    double getDistFromScene() {
        return distFromScene;
    }

    void setDistFromScene(double val) {
        distFromScene = val;
    }

    double getHorizontalSize() {
        return horizSize;
    }

    void setHorizontalSize(double val) {
        horizSize = val;
    }

    double getAspectRatio() {
        return aspectRatio;
    }

    void setAspectRatio(double val) {
        aspectRatio = val;
    }

    Vector3<double> getU() {
        return u;
    }

    Vector3<double> getV() {
        return v;
    }

    Vector3<double> getCenter() {
        return center;
    }

    // Generates a ray starting from the camera and maps to an area in the 3d
    // coordinate plane that points to a pixel
    // x and y are values in the U, V coordinate system related to the pixels
    // in [-1, 1]
    Ray createRayFromCamera(double x, double y) {
        Vector3<double> temp = center + (u * x);
        Vector3<double> screenCoord = temp + (v * y);

        return Ray{screenCoord - position};
    }

    void updateCameraGeometry() {
        principalAxis = lookAtPosition - position;
        principalAxis.normalize();

        u = Vector3<double>::cross(principalAxis, up);
        u.normalize();
        v = Vector3<double>::cross(u, principalAxis);
        v.normalize();

        center = position + (distFromScene * principalAxis);

        u = u * horizSize;
        v = v * (horizSize / aspectRatio);
    }

   private:
    Vector3<double> position;
    Vector3<double> up;
    Vector3<double> lookAtPosition;
    double distFromScene;
    double horizSize;
    double aspectRatio;

    Vector3<double> principalAxis;
    Vector3<double> u;
    Vector3<double> v;
    Vector3<double> center;
};

#endif