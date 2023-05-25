#ifndef SCENE_H
#define SCENE_H

#include <algorithm>
#include <execution>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <thread>
#include <tuple>
#include <vector>

#include "camera.h"
#include "hit.h"
#include "image.h"
#include "mesh.h"
#include "opencl_struct.h"
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
        cl_platform_id platform;
        cl_device_id device;
        cl_context context;
        cl_command_queue commands;

        cl_program program;
        // Kernels
        cl_kernel rendererKernel;
        cl_kernel presentKernel;
        // Buffers
        cl_mem screen_buffer;
        cl_mem spheres_buffer;
        cl_mem screen;

        std::vector<cl_platform_id> platforms;

        cl_uint length;
        cl_int err;

        clGetPlatformIDs(0, 0, &length);
        platforms.resize(length);
        clGetPlatformIDs(platforms.size(), platforms.data(), 0);

        for (int i = 0; i < platforms.size(); i++) {
            std::vector<cl_device_id> devices;

            clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, 0, 0, &length);
            devices.resize(length);
            clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, devices.size(), devices.data(), 0);

            if (devices.size() > 0) {
                device = devices[0];
                platform = platforms[i];
                break;
            }
        }

        context = clCreateContext(0, 1, &device, nullptr, nullptr, &err);
        commands = clCreateCommandQueue(context, device, 0, &err);

        std::ifstream in("../cl_src/gpu.cl");
        std::stringstream ss;
        std::string temp;

        while (std::getline(in, temp)) {
            ss << temp << std::endl;
        }

        in.close();

        std::string src = ss.str();
        const char* c_src = src.c_str();

        int imgWidth = outputImage.getWidth();
        int imgHeight = outputImage.getHeight();

        cl_uint size = imgWidth * imgHeight;
        screen_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE, size * sizeof(ColorCL), nullptr, &err);
        spheres_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, clSpheres.size() * sizeof(SphereCL), clSpheres.data(), &err);

        // get program using ifstream
        // here, c_src is just the result of streaming the source file into a string
        program = clCreateProgramWithSource(context, 1, &c_src, nullptr, &err);

        err = clBuildProgram(program, 0, nullptr, nullptr, nullptr, nullptr);

        rendererKernel = clCreateKernel(program, "renderer", &err);

        size_t len = clSpheres.size();
        CameraCL cameracl = toCL(camera);

        err = clSetKernelArg(rendererKernel, 0, sizeof(cl_mem), (void*)&screen_buffer);
        err |= clSetKernelArg(rendererKernel, 1, sizeof(cl_mem), (void*)&spheres_buffer);
        err |= clSetKernelArg(rendererKernel, 2, sizeof(size_t), (void*)&len);
        err != clSetKernelArg(rendererKernel, 3, sizeof(CameraCL), (void*)&cameracl);

        size_t globalWorkSize[2] = {
            imgWidth,
            imgHeight};

        size_t localWorkSize[2] = {
            16, 16};

        err = clEnqueueNDRangeKernel(commands, rendererKernel, 2, nullptr, globalWorkSize, localWorkSize, 0, nullptr, nullptr);

        cout << "Rendering..." << endl;

        err = clFinish(commands);

        // auto numBins = std::thread::hardware_concurrency();
        // // auto numBins = 1;
        // auto numPixels = imgWidth * imgHeight;
        // std::vector<std::pair<unsigned int, unsigned int>> ranges;
        // std::vector<std::thread> threads;

        // // Segments pixel indices into worker bins
        // for (unsigned int worker = 0; worker < numBins; worker++) {
        //     ranges.push_back(std::pair<unsigned int, unsigned int>(
        //         numPixels * worker / numBins,
        //         numPixels * (worker + 1) / numBins));
        // }

        // // auto resultIdx = 0;
        // // for (auto& i : ranges) {
        // //     auto sw = i.first / imgWidth;
        // //     auto sh = i.first % imgWidth;
        // //     auto ew = i.second / imgWidth;
        // //     auto eh = i.second % imgWidth;
        // //     cout << "[(" << sh << ", " << sw << "), (" << eh << ", " << ew << ")"
        // //          << ")" << endl;
        // // }

        // // For each range of indices, run the trace for each
        // for (auto& r : ranges) {
        //     auto si = r.first;
        //     auto ei = r.second;
        //     threads.push_back(
        //         std::thread([this, si, ei, &imgWidth, &imgHeight, &outputImage, renderCount]() {
        //             trace(imgWidth, imgHeight, si, ei, camera, objects, outputImage, renderCount);
        //         }));
        // }

        // for (auto& t : threads) {
        //     t.join();
        // }
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

Sphere s{Vector3<double>(-1.0, 4.5, 0.5), 0.5};
Glossy g{Vector3<double>(255.0, 0.0, 255.0)};
SphereCL scl{toCL(s, g)};
std::vector<SphereCL> clSpheres{scl, scl, scl};

}  // namespace mpcs51045

#endif