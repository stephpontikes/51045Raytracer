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

        // Note: +x: right, +y: into screen, +z: down
        Sphere s{Vector3<double>{-1.5, 4.5, 0.5}, 0.5};
        Glossy g{Vector3<double>{255.0, 0.0, 255.0}};

        Sphere sl{Vector3<double>{-9.0, 10.0, -9.0}, 10.0};
        Light whiteLight{Vector3<double>{255.0, 255.0, 255.0}};

        Sphere s2{Vector3<double>{-0.25, 4.5, 0.5}, 0.5};
        Glossy green{Vector3<double>{0.0, 255.0, 0.0}};

        Sphere base{Vector3<double>{0.0, 6.0, 11.0}, 10.0};
        Glossy gr{Vector3<double>{255.0, 0.0, 0.0}};

        unique_ptr<GlossyMeshFactory> meshFactory(make_unique<GlossyMeshFactory>());
        // auto mesh(meshFactory->create<Sphere, Glossy>());
        auto ptr = meshFactory->create<Sphere>(Vector3<double>{255.0, 255.0, 255.0}, Vector3<double>{1.25, 0.0, 0.0}, 1.0);
        // shared_ptr<Mesh<Geometry, Material>> shared = ptr;
        unique_ptr<Mesh<Geometry, Material>> ms = make_unique<Mesh<Geometry, Material>>(s, g);
        unique_ptr<Mesh<Geometry, Material>> msl = make_unique<Mesh<Geometry, Material>>(sl, whiteLight);
        unique_ptr<Mesh<Geometry, Material>> msbase = make_unique<Mesh<Geometry, Material>>(base, gr);
        unique_ptr<Mesh<Geometry, Material>> ms2 = make_unique<Mesh<Geometry, Material>>(s2, green);

        objects.emplace_back(std::move(ms));
        objects.emplace_back(std::move(msbase));
        objects.emplace_back(std::move(msl));
        objects.emplace_back(std::move(ms2));

        // Color first, then position, then radius
        // meshFactory->create<Sphere>(Vector3<double>{255.0, 255.0, 255.0}, Vector3<double>{1.25, 0.0, 0.0}, 1.0));
        // objects.push_back(make_shared<Mesh<Geometry, Material>>(mesh));
        // objects.push_back(make_shared<Mesh<Geometry, Material>>(meshl));
    }

    bool render(Image& outputImage) {
        int imgWidth = outputImage.getWidth();
        int imgHeight = outputImage.getHeight();

        // Loop over each pixel in the image
        Ray cameraRay;
        HitData hitData;
        Vector3<double> totalIncomingLight{0.0, 0.0, 0.0};

        double xFact = 1.0 / (static_cast<double>(imgWidth / 2.0));
        double yFact = 1.0 / (static_cast<double>(imgHeight / 2.0));
        for (int x = 0; x < imgWidth; x++) {
            for (int y = 0; y < imgHeight; y++) {
                double normX = static_cast<double>(x * xFact - 1.0);
                double normY = static_cast<double>(y * yFact - 1.0);

                totalIncomingLight = Vector3<double>{0.0, 0.0, 0.0};
                int maxBounceCount = 3;
                int numRaysPerPixel = 10;

                for (int j = 0; j < numRaysPerPixel; j++) {
                    camera.createRay(normX, normY, cameraRay);
                    for (int i = 0; i < maxBounceCount; i++) {
                        auto hitTuple = getClosestHit(cameraRay, objects);
                        hitData = hitTuple.first;
                        auto idx = hitTuple.second;
                        // cout << "index: " << idx << endl;

                        if (hitData.didHit && idx >= 0) {
                            auto& current = objects.at(idx);
                            // cout << current->geometry->coordinates << endl;
                            // cout << "Before: " << cameraRay << endl;
                            Vector3<double> partialLight = handleHit(cameraRay, current->material, hitData);
                            // cout << "After: " << cameraRay << endl;
                            // cout << "New Light: " << partialLight << endl;

                            totalIncomingLight += partialLight;

                        } else {
                            break;
                        }
                    }
                }

                totalIncomingLight /= numRaysPerPixel;

                outputImage.setPixel(x, y, totalIncomingLight.x, totalIncomingLight.y,
                                     totalIncomingLight.z);
            }
        }

        return true;
    }

   private:
    Camera camera;
    std::vector<unique_ptr<Mesh<Geometry, Material>>> objects;
};

}  // namespace mpcs51045

#endif