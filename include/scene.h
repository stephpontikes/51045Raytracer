#ifndef SCENE_H
#define SCENE_H

#include <iostream>
#include <memory>
#include <vector>

#include "camera.h"
#include "hit.h"
#include "image.h"
#include "mesh.h"
#include "random.h"
#include "ray.h"

using namespace mpcs51045;

namespace mpcs51045 {

using std::cout;
using std::endl;
using std::make_shared;
using std::make_unique;
using std::shared_ptr;
using std::unique_ptr;
using std::vector;

class Scene {
   public:
    Scene() {
        camera.setPosition(Vector3<double>{0.0, -10.0, 0.0});
        camera.setLookAtPosition(Vector3<double>{0.0, 0.0, 0.0});
        camera.setUp(Vector3<double>{0.0, 0.0, 1.0});
        camera.setHorizontalSize(0.25);
        camera.setAspectRatio(16.0 / 9.0);
        camera.updateCameraGeometry();

        // unique_ptr<AbstractGeometryFactory> gFactory(make_unique<GeometryFactory>());
        // unique_ptr<AbstractMaterialFactory> mFactory(make_unique<MaterialFactory>());
        // auto s{gFactory->create<Sphere>()};
        // auto glossy{mFactory->create<Glossy>()};
        Sphere s{Vector3<double>{1.25, 0.0, 0.0}, 1.0};
        Light g{Vector3<double>{255.0, 0.0, 255.0}};
        Sphere sl{Vector3<double>{-1.25, 0.0, 0.0}, 1.0};
        Light l{Vector3<double>{255.0, 255.0, 255.0}};
        Mesh<Geometry, Light> mesh{s, g};
        Mesh<Geometry, Light> meshl{sl, l};
        // unique_ptr<GlossyMeshFactory> meshFactory(make_unique<GlossyMeshFactory>());
        // auto mesh(meshFactory->create<Sphere, Glossy>());
        objects.push_back(make_shared<Mesh<Geometry, Light>>(mesh));
        objects.push_back(make_shared<Mesh<Geometry, Light>>(meshl));
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
        Vector3<double> totalIncomingLight{0.0, 0.0, 0.0};

        double xFact = 1.0 / (static_cast<double>(imgWidth / 2.0));
        double yFact = 1.0 / (static_cast<double>(imgHeight / 2.0));
        double minDist = 1e6;
        double maxDist = 0.0;
        for (int x = 0; x < imgWidth; x++) {
            for (int y = 0; y < imgHeight; y++) {
                double normX = static_cast<double>(x * xFact - 1.0);
                double normY = static_cast<double>(y * yFact - 1.0);

                camera.createRay(normX, normY, cameraRay);
                totalIncomingLight = Vector3<double>{0.0, 0.0, 0.0};
                int maxBounceCount = 2;
                int numRaysPerPixel = 10;

                // Test for intersections with all objects (replace with visitor in future)
                for (auto current : objects) {
                    for (int j = 0; j < numRaysPerPixel; j++) {
                        for (int i = 0; i < maxBounceCount; i++) {
                            try {
                                // auto m = static_cast<Mesh<Geometry, Light>>(*current.get());
                                auto sphere = static_cast<Sphere>(current->geometry);
                                // auto sphere = Sphere{Vector3<double>{0.0, 0.0, 0.0}, 1.0};
                                hitData = sphereIntersect(cameraRay, sphere);
                            } catch (std::bad_cast const& e) {
                                cout << "Failed cast" << endl;
                                continue;
                            }

                            if (hitData.didHit) {
                                // cout << "Before: " << cameraRay << endl;
                                totalIncomingLight += handleHit(cameraRay, current, hitData);
                                // cout << "After: " << cameraRay << endl;
                                // cout << "New Light: " << totalIncomingLight << endl;

                            } else {
                                break;
                            }
                        }
                    }
                    // incomingLight *= 255.0;
                    totalIncomingLight /= numRaysPerPixel;
                    // if (incomingLight.x != 0.0 && incomingLight.y != 0.0 && incomingLight.z != 0.0) {
                    //     cout << "Setting pixel " << x << ", " << y << " to " << incomingLight << endl;
                    // }
                    outputImage.setPixel(x, y, totalIncomingLight.x, totalIncomingLight.y,
                                         totalIncomingLight.z);
                }
            }
        }
        cout << "Finished looping" << endl;

        return true;
    }

   private:
    Camera camera;
    std::vector<shared_ptr<Mesh<Geometry, Light>>> objects;
    // Sphere sphere{Vector3<double>{0.0, 0.0, 0.0}, 1.0};
};

}  // namespace mpcs51045

#endif