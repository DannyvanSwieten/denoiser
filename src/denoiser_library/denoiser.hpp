#include <OpenImageDenoise/oidn.hpp>

class Denoiser
{
public:
    Denoiser();
    ~Denoiser();

    void denoise(const int width, const int height, const float *beauty, const float *albedo, const float *normals, float *outputData);

private:
    void denoiseAlbedo(const int width, const int height, oidn::BufferRef &albedoBuffer);
    void denoiseNormals(const int width, const int height, oidn::BufferRef &albedoBuffer);

private:
    oidn::DeviceRef device;
};