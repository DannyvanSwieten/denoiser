#include "denoiser.hpp"
#include <memory>

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#endif

static std::unique_ptr<Denoiser> denoiser;

extern "C" EXPORT void denoiser_init()
{
    denoiser = std::make_unique<Denoiser>();
}

extern "C" EXPORT void denoiser_denoise(const int width, const int height, const float *beauty, const float *albedo, const float *normals, float *outputData)
{
    denoiser->denoise(width, height, beauty, albedo, normals, outputData);
}