#ifndef SCENE_H
#define SCENE_H

#include <algorithm>
#include <execution>
#include <iostream>
#include <memory>
#include <thread>
#include <tuple>
#include <vector>

#include "camera.h"
#include "hit.h"
#include "image.h"
#include "mesh.h"
#include "random.h"
#include "ray.h"

using namespace mpcs51045;
using std::for_each;

namespace mpcs51045 {

using std::cout;
using std::endl;
using std::make_shared;
using std::make_unique;
using std::shared_ptr;
using std::unique_ptr;
using std::vector;

// Increasing bounce count does not seem to impact performance
constexpr int MAX_BOUNCE_COUNT = 3;
// Increase in num rays per pixel has linear impact on performance
constexpr auto NUM_RAYS_PER_PIXEL = 10;

Vector3<double> bounceRay(Ray cameraRay,
                          std::vector<unique_ptr<Mesh<Geometry, Material>>> const& objects) {
    Vector3<double> incomingLight{0.0, 0.0, 0.0};
    for (int i = 0; i < MAX_BOUNCE_COUNT; i++) {
        auto hitTuple = getClosestHit(cameraRay, objects);
        HitData hitData = hitTuple.first;
        auto idx = hitTuple.second;
        // cout << "index: " << idx << endl;

        if (hitData.didHit && idx >= 0) {
            auto& current = objects.at(idx);
            // cout << current->geometry->coordinates << endl;
            // cout << "Before: " << cameraRay << endl;
            incomingLight += handleHit(cameraRay, current->material, hitData);
            // cout << "After: " << cameraRay << endl;
        } else {
            break;
        }
    }

    return incomingLight;
}

void trace(int const& imgWidth, int const& imgHeight, int const& startIdx,
           int const& endIdx, Camera& camera,
           std::vector<unique_ptr<Mesh<Geometry, Material>>> const& objects,
           Image& outputImage, int const& renderCount) {
    double xFact = 1.0 / (static_cast<double>(imgWidth / 2.0));
    double yFact = 1.0 / (static_cast<double>(imgHeight / 2.0));
    for (int i = startIdx; i < endIdx; i++) {
        auto x = i % imgWidth;
        auto y = i / imgWidth;

        Ray cameraRay;
        double normX = static_cast<double>(x * xFact - 1.0);
        double normY = static_cast<double>(y * yFact - 1.0);

        Vector3<double> incomingLight;

        for (int j = 0; j < NUM_RAYS_PER_PIXEL; j++) {
            camera.createRay(normX, normY, cameraRay);
            incomingLight += bounceRay(cameraRay, objects);
        }

        incomingLight /= NUM_RAYS_PER_PIXEL;
        // cout << "Light at " << x << ", " << y << ": " << incomingLight << endl;

        if (!renderCount) {
            outputImage.setPixel(x, y, incomingLight.x, incomingLight.y,
                                 incomingLight.z);
        } else {
            auto currentPixel = outputImage.getPixel(x, y);
            if (x == 0 && y == 0)
                cout << "render count: " << renderCount << endl;
            currentPixel = ((currentPixel * renderCount) +
                            (1 / renderCount) * incomingLight) /
                           (renderCount + 1);
            outputImage.setPixel(x, y, currentPixel.x, currentPixel.y,
                                 currentPixel.z);
        }
    }
}

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
        auto pink = create<Mesh, Sphere, Glossy>(Vector3<double>(255.0, 0.0, 255.0),
                                                 Vector3<double>(-1.0, 4.5, 0.5), 0.5);
        auto base = create<Mesh, Sphere, Matte>(Vector3<double>(255.0, 0.0, 0.0),
                                                Vector3<double>(0.0, 6.0, 11.0), 10.0);
        auto green = create<Mesh, Sphere, Mirror>(Vector3<double>(0.0, 255.0, 0.0),
                                                  Vector3<double>(0.5, 4.5, 0.5), 0.5);
        auto blue = create<Mesh, Sphere, Matte>(Vector3<double>(0.0, 0.0, 255.0),
                                                Vector3<double>(2.0, 4.5, 0.5), 0.5);
        auto light = create<Mesh, Sphere, Light>(Vector3<double>(255.0, 255.0, 255.0),
                                                 Vector3<double>(-9.0, 10.0, -9.0), 10.0);
        auto light2 = create<Mesh, Sphere, Light>(Vector3<double>(255.0, 255.0, 255.0),
                                                  Vector3<double>(6.5, 10.0, -6.5), 5.0);
        auto triangle = create<Mesh, Triangle, Matte>(Vector3<double>(255.0, 255.0, 0.0),
                                                      Vector3<double>(0.0, 0.0, 0.0),
                                                      Vector3<double>(-0.5, -1.0, 0.0),
                                                      Vector3<double>(-1.0, 1.0, 0.0),
                                                      Vector3<double>(0.0, 0.0, -0.5));
        auto triangle2 = create<Mesh, Triangle, Matte>(Vector3<double>(255.0, 255.0, 0.0),
                                                       Vector3<double>(0.0, 0.0, 0.0),
                                                       Vector3<double>(-1.0, 0.0, 0.0),
                                                       Vector3<double>(0.0, -1.0, 0.0),
                                                       Vector3<double>(1.0, 0.0, 0.0));

        // unique_ptr<GlossyMeshFactory> factory = make_unique<GlossyMeshFactory>();
        // auto sphere = factory->create<Sphere>(Vector3<double>(255.0, 255.0, 0.0),
        //                                       Vector3<double>(), 1.0);
        // cout << "is null: " << sphere->geometry->coordinates << endl;

        // Triangle t1{Vector3<double>(0.0, 0.0, 0.0),
        //             Vector3<double>(-0.5, -1.0, 0.0),
        //             Vector3<double>(-1.0, 1.0, 0.0),
        //             Vector3<double>(0.0, 0.0, -0.5)};
        // Triangle t2{Vector3<double>(0.0, 0.0, 0.0),
        //             Vector3<double>(-0.5, -1.0, 0.0),
        //             Vector3<double>(1.0, 1.0, 0.0),
        //             Vector3<double>(0.0, 0.0, -0.5)};
        // vector<Triangle> prismVec{t1, t2};
        // ComplexGeometry prism(Vector3<double>(0.0, 0.0, 0.0), prismVec);

        objects.emplace_back(std::move(pink));
        // objects.emplace_back(std::move(base));
        // objects.emplace_back(std::move(green));
        // objects.emplace_back(std::move(blue));
        objects.emplace_back(std::move(light));
        // objects.emplace_back(std::move(light2));
        // objects.emplace_back(std::move(triangle));
        // objects.emplace_back(std::move(triangle2));
    }

    bool render(Image& outputImage, int const& renderCount) {
        cout << "Rendering..." << endl;
        int imgWidth = outputImage.getWidth();
        int imgHeight = outputImage.getHeight();

        auto numBins = std::thread::hardware_concurrency();
        // auto numBins = 1;
        auto numPixels = imgWidth * imgHeight;
        std::vector<std::pair<unsigned int, unsigned int>> ranges;
        std::vector<std::thread> threads;

        // Segments pixel indices into worker bins
        for (unsigned int worker = 0; worker < numBins; worker++) {
            ranges.push_back(std::pair<unsigned int, unsigned int>(
                numPixels * worker / numBins,
                numPixels * (worker + 1) / numBins));
        }

        // auto resultIdx = 0;
        // for (auto& i : ranges) {
        //     auto sw = i.first / imgWidth;
        //     auto sh = i.first % imgWidth;
        //     auto ew = i.second / imgWidth;
        //     auto eh = i.second % imgWidth;
        //     cout << "[(" << sh << ", " << sw << "), (" << eh << ", " << ew << ")"
        //          << ")" << endl;
        // }

        // For each range of indices, run the trace for each
        for (auto& r : ranges) {
            auto si = r.first;
            auto ei = r.second;
            threads.push_back(
                std::thread([this, si, ei, &imgWidth, &imgHeight, &outputImage, renderCount]() {
                    trace(imgWidth, imgHeight, si, ei, camera, objects, outputImage, renderCount);
                }));
        }

        for (auto& t : threads) {
            t.join();
        }
        cout << "Finished." << endl;
        return true;
    }

    bool updateCameraPosition(SDL_Event* event) {
        auto key = event->key.keysym.sym;
        auto cameraPos = camera.getPosition();

        switch (key) {
            case SDLK_LEFT:
                cameraPos.x += 1.0;
                break;
            case SDLK_RIGHT:
                cameraPos.x -= 1.0;
                break;
            case SDLK_UP:
                cameraPos.z += 1.0;
                break;
            case SDLK_DOWN:
                cameraPos.z -= 1.0;
                break;
            default:
                return false;
        }

        camera.setPosition(cameraPos);
        camera.updateCameraGeometry();

        return true;
    }

   private:
    Camera camera;
    std::vector<unique_ptr<Mesh<Geometry, Material>>> objects;
};

}  // namespace mpcs51045

#endif