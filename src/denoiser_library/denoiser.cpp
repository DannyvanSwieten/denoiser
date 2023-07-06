#include "denoiser.hpp"
#include <iostream>

Denoiser::Denoiser() : device(oidn::newDevice())
{
    device.commit();
    const char *errorMessage;
    if (device.getError(errorMessage) != oidn::Error::None)
        std::cout << errorMessage << std::endl;
}

Denoiser::~Denoiser()
{
}

void Denoiser::denoise(const int width, const int height, const float *beauty, const float *albedo, const float *normals, float *outputData)
{
    const char *errorMessage;
    auto filter = device.newFilter("RT"); // generic ray tracing filter
    if (device.getError(errorMessage) != oidn::Error::None)
        std::cout << errorMessage << std::endl;

    const size_t size = width * height * 3 * sizeof(float);
    oidn::BufferRef colorBuf = device.newBuffer(size);

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

        denoiseNormals(width, height, normalBuffer);

        filter.setImage("normal", normalBuffer, oidn::Format::Float3, width, height); // normals
    }

    oidn::BufferRef albedoBuffer;
    if (albedo != nullptr)
    {
        albedoBuffer = device.newBuffer(size);
        std::memcpy(albedoBuffer.getData(), beauty, size);

        denoiseAlbedo(width, height, albedoBuffer);

        filter.setImage("albedo", albedoBuffer, oidn::Format::Float3, width, height); // albedo
    }

    filter.setImage("output", colorBuf, oidn::Format::Float3, width, height); // denoised beauty
    filter.set("cleanAux", true);
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

void Denoiser::denoiseAlbedo(const int width, const int height, oidn::BufferRef &albedoBuffer)
{
    auto albedoFilter = device.newFilter("RT");                                         // generic ray tracing filter
    albedoFilter.setImage("albedo", albedoBuffer, oidn::Format::Float3, width, height); // albedo
    albedoFilter.setImage("output", albedoBuffer, oidn::Format::Float3, width, height); // denoised albedo
    albedoFilter.commit();
    albedoFilter.execute();
}

void Denoiser::denoiseNormals(const int width, const int height, oidn::BufferRef &albedoBuffer)
{
    auto normalFilter = device.newFilter("RT");                                         // generic ray tracing filter
    normalFilter.setImage("normal", albedoBuffer, oidn::Format::Float3, width, height); // normals
    normalFilter.setImage("output", albedoBuffer, oidn::Format::Float3, width, height); // denoised normals
    normalFilter.commit();
    normalFilter.execute();
}
