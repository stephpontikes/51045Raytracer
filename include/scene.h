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
        Sphere s{Vector3<double>{0.0, 6.0, 1.5}, 0.5};
        Glossy g{Vector3<double>{255.0, 0.0, 255.0}};

        Sphere sl{Vector3<double>{-5.0, 5.0, -5.0}, 5.0};
        Light whiteLight{Vector3<double>{255.0, 255.0, 255.0}};

        Sphere sl2{Vector3<double>{1.5, 0.0, 0.5}, 0.5};
        Light greenLight{Vector3<double>{0.0, 255.0, 0.0}};

        Sphere base{Vector3<double>{0.0, 6.0, 11.0}, 10.0};
        Glossy gr{Vector3<double>{255.0, 0.0, 0.0}};
        // Mesh<Geometry, Material> mesh{make_unique<Geometry>(s), make_unique<Material>(g)};
        // Mesh<Geometry, Material> meshl{make_unique<Geometry>(sl), make_unique<Material>(l)};
        unique_ptr<GlossyMeshFactory> meshFactory(make_unique<GlossyMeshFactory>());
        // auto mesh(meshFactory->create<Sphere, Glossy>());
        auto ptr = meshFactory->create<Sphere>(Vector3<double>{255.0, 255.0, 255.0}, Vector3<double>{1.25, 0.0, 0.0}, 1.0);
        // shared_ptr<Mesh<Geometry, Material>> shared = ptr;
        unique_ptr<Mesh<Geometry, Material>> ms = make_unique<Mesh<Geometry, Material>>(s, g);
        unique_ptr<Mesh<Geometry, Material>> msl = make_unique<Mesh<Geometry, Material>>(sl, whiteLight);
        unique_ptr<Mesh<Geometry, Material>> msbase = make_unique<Mesh<Geometry, Material>>(base, gr);
        unique_ptr<Mesh<Geometry, Material>> msl2 = make_unique<Mesh<Geometry, Material>>(sl2, greenLight);
        // NOTE: Have to add all of the objects first, then all light sources
        objects.emplace_back(std::move(ms));
        objects.emplace_back(std::move(msbase));
        objects.emplace_back(std::move(msl));
        // objects.emplace_back(std::move(msl2));
        // make_shared<Mesh<Geometry, Material>>(
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

                totalIncomingLight = Vector3<double>{0.0, 0.0, 0.0};
                int maxBounceCount = 5;
                int numRaysPerPixel = 10;

                for (int j = 0; j < numRaysPerPixel; j++) {
                    camera.createRay(normX, normY, cameraRay);
                    // Test for intersections with all objects (replace with visitor in future)
                    for (auto& current : objects) {
                        for (int i = 0; i < maxBounceCount; i++) {
                            try {
                                // auto m = static_cast<Mesh<Geometry, Light>>(*current.get());
                                // auto sphere = static_cast<Sphere>(*current->geometry.get());
                                // auto sphere = Sphere{Vector3<double>{0.0, 0.0, 0.0}, 1.0};
                                hitData = sphereIntersect(cameraRay, current->geometry);
                            } catch (std::bad_cast const& e) {
                                cout << "Failed cast" << endl;
                                continue;
                            }

                            if (hitData.didHit) {
                                // cout << "Before: " << cameraRay << endl;
                                Vector3<double> partialLight = handleHit(cameraRay, current, hitData);
                                // cout << "After: " << cameraRay << endl;
                                // cout << "New Light: " << partialLight << endl;

                                totalIncomingLight += partialLight;

                            } else {
                                break;
                            }
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
    // Sphere sphere{Vector3<double>{0.0, 0.0, 0.0}, 1.0};
};

}  // namespace mpcs51045

#endif