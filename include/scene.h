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

Sphere s{Vector3<double>(-1.0, 4.5, 0.5), 0.5};
Sphere sl2{Vector3<double>(2.0, 0.0, 0.0), 1.0};
Light l{Vector3<double>(0.0, 255.0, 255.0)};
std::vector<SphereCL> clSpheres{toCL(sl2, l), toCL(s, l)};

namespace mpcs51045 {

using std::cout;
using std::endl;
using std::make_shared;
using std::make_unique;
using std::shared_ptr;
using std::unique_ptr;
using std::vector;

constexpr int MAX_BOUNCE_COUNT = 5;
constexpr auto NUM_RAYS_PER_PIXEL = 10;

Vector3<double> bounceRay(Ray cameraRay,
                          std::vector<unique_ptr<Mesh<Geometry, Material>>> const& objects) {
    Vector3<double> incomingLight{0.0, 0.0, 0.0};
    for (int i = 0; i < MAX_BOUNCE_COUNT; i++) {
        auto hitTuple = getClosestHit(cameraRay, objects);
        HitData hitData = hitTuple.first;
        auto idx = hitTuple.second;

        if (hitData.didHit && idx >= 0) {
            auto& current = objects.at(idx);
            incomingLight += handleHit(cameraRay, current->material, hitData);
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

        outputImage.setPixel(x, y, incomingLight.x, incomingLight.y,
                             incomingLight.z);
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

        // unique_ptr<GlossyMeshFactory> factory = make_unique<LightMeshFactory>();
        // unique_ptr<MatteMeshFactory> mFactory = make_unique<MatteMeshFactory>();
        // unique_ptr<LightMeshFactory> lFactory = make_unique<LightMeshFactory>();

        // auto teal = lFactory->create<Sphere>(Vector3<double>(0.0, 255.0, 255.0), Vector3<double>(0.0, 0.0, 0.0), 2.0);
        // cout << teal->geometry->coordinates << endl;
        // objects.emplace_back(std::move(teal));

        // auto lightTest = create<Mesh, Sphere, Light>(Vector3<double>(255.0, 255.0, 255.0),
        //  Vector3<double>(-2.0, 0.0, 0.0), 1.0);
        // auto teal = create<Mesh, Sphere, Light>(Vector3<double>(0.0, 255.0, 255.0),
        //                                         Vector3<double>(2.0, 0.0, 0.0), 1.0);
        // objects.emplace_back(std::move(lightTest));
        // objects.emplace_back(std::move(teal));

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
        objects.emplace_back(std::move(base));
        objects.emplace_back(std::move(green));
        objects.emplace_back(std::move(blue));
        objects.emplace_back(std::move(light));
        objects.emplace_back(std::move(light2));
        // objects.emplace_back(std::move(triangle));
        // objects.emplace_back(std::move(triangle2));
    }

    bool render(Image& outputImage, int const& renderCount) {
        int imgWidth = outputImage.getWidth();
        int imgHeight = outputImage.getHeight();
#if 0
        cl::Platform platform;
        cl::Device device;
        cl::Context context;
        cl::CommandQueue commands;

        cl::Program program;
        // Kernels
        cl::Kernel rendererKernel;
        cl::Kernel presentKernel;
        // Buffers
        cl::Buffer screen_buffer;
        cl::Buffer spheres_buffer;
        cl::Buffer seed_buffer;
        cl::Buffer screen;

        std::vector<cl::Platform> platforms;

        cl_uint length;
        cl_int err;

        cl::Platform::get(&platforms);
        platform = platforms[0];

        std::vector<cl::Device> devices;
        platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);

        if (devices.size() > 0) {
            device = devices[0];
        }

        context = cl::Context(device);

        std::ifstream in("../cl_src/gpu.cl");
        std::ifstream inRandom("../cl_src/random.cl");
        std::stringstream ss;
        std::string temp;

        while (std::getline(inRandom, temp)) {
            ss << temp << std::endl;
        }
        inRandom.close();

        while (std::getline(in, temp)) {
            ss << temp << std::endl;
        }
        in.close();

        std::string src = ss.str();
        const char* c_src = src.c_str();

        cl_uint size = imgWidth * imgHeight;

        cout << "Starting" << endl;

        auto imageColors = toCL(outputImage);

        // get program using ifstream
        // here, c_src is just the result of streaming the source file into a string
        program = cl::Program(context, c_src);

        err = program.build({device}, "");

        if (err != CL_SUCCESS) {
            char buf[2048];
            std::cout << "Error: Failed to build program" << std::endl;
            size_t len = static_cast<size_t>(length);
            std::cout << buf << std::endl;
            std::getchar();
            exit(1);
        }

        rendererKernel = cl::Kernel(program, "renderer");

        CameraCL cameracl = toCL(camera);

        cl_uint2 seed;
        seed.s[0] = 3;
        seed.s[1] = 4;

        size_t len = clSpheres.size();

        ColorCL* output = (ColorCL*)malloc(sizeof(ColorCL) * size);

        screen_buffer = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, size * sizeof(ColorCL), output);
        spheres_buffer = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, clSpheres.size() * sizeof(SphereCL), clSpheres.data());
        seed_buffer = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(cl_uint2), &seed);
        cl::Buffer cam_buffer = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(CameraCL), &cameracl);

        err |= rendererKernel.setArg(0, screen_buffer);
        err |= rendererKernel.setArg(1, spheres_buffer);
        err |= rendererKernel.setArg(2, len);
        err |= rendererKernel.setArg(3, cam_buffer);
        err |= rendererKernel.setArg(4, seed_buffer);

        if (err != CL_SUCCESS) {
            cout << "Failed assignment" << endl;
        }
        cl::NDRange globalWorkSize = {
            static_cast<size_t>(imgWidth),
            static_cast<size_t>(imgHeight)};

        cl::NDRange localWorkSize = {
            16, 16};

        commands = cl::CommandQueue(context, device);

        err |= commands.enqueueNDRangeKernel(rendererKernel, NULL, globalWorkSize, localWorkSize);
        cout << err << endl;

        std::vector<ColorCL> colors(size);
        err |= commands.enqueueReadBuffer(screen_buffer, CL_TRUE, 0, colors.size() * sizeof(ColorCL), colors.data());
        cout << err << endl;

        if (err != CL_SUCCESS) {
            cout << "Failed enqueueing" << endl;
        }
        cout << "Rendering..." << endl;

        err = cl::flush();
        err = cl::finish();

        outputImage.updateColorChannels(colors);
        free(output);
#else

        auto numBins = std::thread::hardware_concurrency();
        auto numPixels = imgWidth * imgHeight;
        std::vector<std::pair<unsigned int, unsigned int>> ranges;
        std::vector<std::thread> threads;

        // Segments pixel indices into worker bins
        for (unsigned int worker = 0; worker < numBins; worker++) {
            ranges.push_back(std::pair<unsigned int, unsigned int>(
                numPixels * worker / numBins,
                numPixels * (worker + 1) / numBins));
        }

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
#endif
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
        cout << camera.getPosition() << endl;
        camera.updateCameraGeometry();

        return true;
    }

   private:
    Camera camera;
    std::vector<unique_ptr<Mesh<Geometry, Material>>> objects;
};

}  // namespace mpcs51045

#endif