#ifndef SCENE_H
#define SCENE_H

#include "image.h"
#include "camera.h"
#include "mesh.h"
#include "hit.h"

namespace mpcs51045 {

class Scene {
   public:
    Scene() {
        // camera lookAt = <0, 0, 0>, up = <0, 1, 0> by default
        camera.setPosition(Vector3<double>{0.0, 0.0, -10.0});
        camera.setHorizontalSize(0.25);
        camera.setAspectRatio(16.0 / 9.0);
        camera.updateCameraGeometry();
    }

    bool render(Image& outputImage) {
        int imgWidth = outputImage.getWidth();
        int imgHeight = outputImage.getHeight();

        // for (int x = 0; x < imgWidth; x++) {
        //     for (int y = 0; y < imgHeight; y++) {
        //         double red = (static_cast<double>(x) / double(imgWidth)) * 255.0;
        //         double green = (static_cast<double>(y) / double(imgHeight)) * 255.0;
        //         outputImage.setPixel(x, y, red, green, 0.0);
        //     }
        // }

        // Loop over each pixel in the image
        Ray cameraRay;
        Vector3<double> intersectPoint;
        Vector3<double> localNormal;
        Vector3<double> localColor;

        double xFact = 1.0 / (static_cast<double>(imgWidth / 2.0));
        double yFact = 1.0 / (static_cast<double>(imgHeight / 2.0));
        double minDist = 1e6;
        double maxDist = 0.0;
        for (int x = 0; x < imgWidth; x++) {
            for (int y =  0; y < imgHeight; y++) {
                double normX = static_cast<double>(x * xFact - 1.0);
                double normY = static_cast<double>(y * yFact - 1.0);

                camera.createRay(normX, normY, cameraRay);

                HitData hitData = sphereIntersect(cameraRay, sphere);
                intersectPoint = hitData.hitPoint;
                localNormal = hitData.hitNormal;

                if (hitData.didHit) {
                    
                    outputImage.setPixel(x, y, 255.0, 0.0, 0.0);
                } else {
                    outputImage.setPixel(x, y, 0.0, 0.0, 0.0);
                }
            }
        }


        return true;
    }

    private:
        Camera camera;
        std::vector<Mesh> objects;
};

}  // namespace mpcs51045

#endif