#include <assert.h>

#include <chrono>
#include <iostream>
#include <memory>

#include "camera.h"
#include "factory.h"
#include "graphics.h"
#include "mesh.h"
#include "opencl.cpp"
#include "opencl_struct.h"
#include "ray.h"
#include "vector3.h"

using std::cout;
using std::endl;
using std::make_unique;
using std::unique_ptr;

using namespace mpcs51045;
using namespace std::chrono;

void getOpenCLInfo() {
    //     std::vector<cl::Platform> platforms;
    //     cl::Platform::get(&platforms);

    //     assert(platforms.size() > 0);

    //     auto platform = platforms.front();

    //     std::vector<cl::Device> devices;
    //     platforms.front().getDevices(CL_DEVICE_TYPE_GPU, &devices);

    //     assert(devices.size() > 0);

    //     auto device = devices.front();
    //     auto vendor = device.getInfo<CL_DEVICE_VENDOR>();
    //     auto version = device.getInfo<CL_DEVICE_VERSION>();

    //     std::cout << "Device Vendor: " << vendor << std::endl;
    //     std::cout << "Device Version: " << version << std::endl;

    //     // Test
    //     cl::Context context(device);
    //     cl::Program::Sources sources;

    //     std::string kernelCode =
    //         "   void kernel squareArray(global int* input, global int* output) {"
    //         "       size_t gid = get_global_id(0);"
    //         "       output[gid] = input[gid] * input[gid];"
    //         "   }";
    //     sources.push_back({kernelCode.c_str(), kernelCode.length()});

    //     cl_int exitcode = 0;

    //     cl::Program program(context, sources, &exitcode);
    //     program.build();
    //     assert(exitcode == CL_SUCCESS);

    //     cl::Kernel kernel(program, "squareArray", &exitcode);
    //     assert(exitcode == CL_SUCCESS);

    //     auto workGroupSize = kernel.getWorkGroupInfo<CL_KERNEL_WORK_GROUP_SIZE>(device);
    //     std::cout << "Kernel Work Group Size: " << workGroupSize << std::endl;

    //     std::vector<int> outVec(1024);
    //     std::vector<int> inVec(1024);
    //     std::iota(inVec.begin(), inVec.end(), 1);

    //     cl::Buffer inBuf(context,
    //                      CL_MEM_READ_ONLY | CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR,
    //                      sizeof(int) * inVec.size(),
    //                      inVec.data());
    //     cl::Buffer outBuf(context,
    //                       CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY,
    //                       sizeof(int) * outVec.size());
    //     kernel.setArg(0, inBuf);
    //     kernel.setArg(1, outBuf);

    //     cl::CommandQueue queue(context, device);

    //     queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(inVec.size()));
    //     queue.enqueueReadBuffer(outBuf, CL_TRUE, 0, sizeof(int) * outVec.size(), outVec.data());

    //     for (std::vector<int>::const_iterator i = outVec.begin(); i != outVec.end(); ++i)
    //         std::cout << *i << std::endl;
}

int main() {
    int windowWidth = 1280;
    int windowHeight = 720;

    // unique_ptr<GlossyMeshFactory> gmf = make_unique<GlossyMeshFactory>();
    // auto gs = create<Mesh, Sphere, Glossy>(Vector3<double>(255.0, 0.0, 255.0), Vector3<double>(0.0, 5.0, 0.0), 1.0);
    // std::cout << gs->geometry->coordinates << std::endl;

    // std::cout << gs->material->color() << std::endl;
    // std::cout << gs->material->reflectivity() << std::endl;
    // auto cast = dynamic_cast<Sphere*>(gs->geometry.get());
    // if (cast != nullptr) {
    //     std::cout << cast->radius << std::endl;
    // }

    // get the default device
    // compute::device device = compute::system::default_device();

    // print the device's name
    // std::cout << "hello from " << device.name() << std::endl;
    // getOpenCLInfo();

    // return 0;

    // auto start = high_resolution_clock::now();
    // unique_ptr<Graphics> graphics = make_unique<Graphics>(windowWidth,
    //                                                       windowHeight);
    // cout << "Render Time: " << duration_cast<milliseconds>(high_resolution_clock::now() - start).count() << "ms" << endl;

    // graphics->run();

    cl_float3 vec{1.0, 2.0, 3.0};
    cout << vec.s[0] << endl;
    cout << vec.s[1] << endl;
    cout << vec.s[2] << endl;

    // runGL();

    return 0;
}
