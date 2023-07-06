#include "denoiser.hpp"
#include <iostream>

Denoiser::Denoiser() : device(oidn::newDevice())
{
    device.commit();
    const char *errorMessage;
    if (device.getError(errorMessage) != oidn::Error::None)
        std::cout << errorMessage << std::endl;

    filter = device.newFilter("RT"); // generic ray tracing filter
    if (device.getError(errorMessage) != oidn::Error::None)
        std::cout << errorMessage << std::endl;
}

Denoiser::~Denoiser()
{
}

void Denoiser::denoise(const int width, const int height, const float *beauty, const float *albedo, const float *normals, float *outputData)
{
    const size_t size = width * height * 3 * sizeof(float);
    oidn::BufferRef colorBuf = device.newBuffer(size);
    const char *errorMessage;
    if (device.getError(errorMessage) != oidn::Error::None)
        std::cout << errorMessage << std::endl;

    std::memcpy(colorBuf.getData(), beauty, size);
    filter.setImage("color", colorBuf, oidn::Format::Float3, width, height); // beauty
    if (device.getError(errorMessage) != oidn::Error::None)
        std::cout << errorMessage << std::endl;

    oidn::BufferRef normalBuffer;
    if (normals != nullptr)
    {
        normalBuffer = device.newBuffer(size);
        std::memcpy(normalBuffer.getData(), beauty, size);

        filter.setImage("normal", normalBuffer, oidn::Format::Float3, width, height); // normals
    }

    oidn::BufferRef albedoBuffer;
    if (albedo != nullptr)
    {
        albedoBuffer = device.newBuffer(size);
        std::memcpy(albedoBuffer.getData(), beauty, size);

        filter.setImage("normal", albedoBuffer, oidn::Format::Float3, width, height); // normals
    }

    filter.setImage("output", colorBuf, oidn::Format::Float3, width, height); // denoised beauty
    if (device.getError(errorMessage) != oidn::Error::None)
        std::cout << errorMessage << std::endl;
    filter.set("hdr", true); // beauty image is HDR
    if (device.getError(errorMessage) != oidn::Error::None)
        std::cout << errorMessage << std::endl;

    filter.commit();
    if (device.getError(errorMessage) != oidn::Error::None)
        std::cout << errorMessage << std::endl;

    filter.execute();

    if (device.getError(errorMessage) != oidn::Error::None)
        std::cout << errorMessage << std::endl;

    device.sync();

    std::memcpy(outputData, colorBuf.getData(), size);
}
