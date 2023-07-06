#include <OpenImageDenoise/oidn.hpp>

class Denoiser
{
public:
    Denoiser();
    ~Denoiser();

    void denoise(const int width, const int height, const float *beauty, const float *albedo, const float *normals, float *outputData);

private:
    oidn::DeviceRef device;
    oidn::FilterRef filter;
};