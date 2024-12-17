#ifndef FLUIDBOX
#define FLUIDBOX

#include "raylib.h"
#include <vector>

class FluidBox {
public:
  FluidBox(int screenWidth, int screenHeight);

  void update_density(std::vector<float> &prevDensityField,
                      std::vector<float> &densityField,
                      std::vector<Vector2> &velocityField, float diffusionRate);

  void update_velocity(std::vector<Vector2> &velocityField,
                       std::vector<Vector2> &prevVelocityField,
                       float viscocity);

  const std::vector<float>
  get_source_from_UI(std::vector<float> prevDensityField,
                     std::vector<Vector2> prevVelocityField);

private:
  const int m_boxWidth;
  const int m_boxHeight;

  std::vector<Vector2> m_velocityField();
  std::vector<Vector2> m_prevVelocityField();

  std::vector<float> m_density();
  std::vector<float> m_prevDensity();

  const float m_timestep;

  int IX(const int x, const int y) { return (x + (y * m_boxHeight)); };

  // forces will either be added to velocity field(type Vector2), or density
  // field (type float). Hence, function overloading.
  void add_source(const std::vector<Vector2> &forceSource,
                  std::vector<Vector2> &targetVelocityField);
  void add_source(const std::vector<float> &forceSource,
                  std::vector<float> &targetDensityField);

  void diffuse(const std::vector<Vector2> &source,
               const std::vector<Vector2> &prevSource,
               const float diffusionRate);
  void diffuse(const std::vector<float> &source,
               const std::vector<float> &prevSource, const float diffusionRate);

  void advect(std::vector<float> &densityField,
              std::vector<float> &prevDensityField,
              std::vector<Vector2> &velocityField);

  // TODO: project
};
#endif
