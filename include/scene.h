#ifndef SCENE_H
#define SCENE_H

#include <iostream>

#include "camera.h"
#include "hit.h"
#include "image.h"
#include "mesh.h"

namespace mpcs51045 {

using std::cout;
using std::endl;

class Scene {
   public:
    Scene() {
        // sphere = Sphere{Vector3<double>{0.0, 0.0, 0.0}, 1.0};
        camera.setPosition(Vector3<double>{0.0, -10.0, 0.0});
        camera.setLookAtPosition(Vector3<double>{0.0, 0.0, 0.0});
        camera.setUp(Vector3<double>{0.0, 0.0, 1.0});
        camera.setHorizontalSize(0.25);
        camera.setAspectRatio(16.0 / 9.0);
        camera.updateCameraGeometry();
    }

    bool render(Image& outputImage) {
        int imgWidth = outputImage.getWidth();
        int imgHeight = outputImage.getHeight();

        // Loop over each pixel in the image
        Ray cameraRay;
        Vector3<double> intersectPoint;
        Vector3<double> localNormal;
        Vector3<double> localColor;
        HitData hitData;

        double xFact = 1.0 / (static_cast<double>(imgWidth / 2.0));
        double yFact = 1.0 / (static_cast<double>(imgHeight / 2.0));
        double minDist = 1e6;
        double maxDist = 0.0;
        for (int x = 0; x < imgWidth; x++) {
            for (int y = 0; y < imgHeight; y++) {
                double normX = static_cast<double>(x * xFact - 1.0);
                double normY = static_cast<double>(y * yFact - 1.0);

                camera.createRay(normX, normY, cameraRay);

                hitData = sphereIntersect(cameraRay, sphere);
                intersectPoint = hitData.hitPoint;

                if (hitData.didHit) {
                    double dist = (intersectPoint - cameraRay.position).norm();
                    if (dist > maxDist) {
                        maxDist = dist;
                    }

                    if (dist < minDist) {
                        minDist = dist;
                    }

                    outputImage.setPixel(x, y, 255.0 - ((dist - 9.0) / 0.94605) * 255.0, 0.0, 0.0);
                } else {
                    outputImage.setPixel(x, y, 0.0, 0.0, 0.0);
                }
            }
        }

        return true;
    }

   private:
    Camera camera;
    Sphere sphere{Vector3<double>{0.0, 0.0, 0.0}, 1.0};
    // std::vector<Mesh> objects;
};

}  // namespace mpcs51045

#endif