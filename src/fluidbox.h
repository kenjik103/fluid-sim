#ifndef FLUIDBOX
#define FLUIDBOX

#include "raylib.h"
#include <iostream>
#include <vector>

// comment/uncomment to enable testing
#define TESTS

class FluidBox {
public:
  FluidBox(int screenWidth, int screenHeight, float timestep);

  void update_density(float diffusionRate);

  void update_velocity(std::vector<Vector2> &velocityField,
                       std::vector<Vector2> &prevVelocityField,
                       float viscocity);

  const std::vector<float>
  get_source_from_UI(std::vector<float> prevDensityField,
                     std::vector<Vector2> prevVelocityField);

  const std::vector<Vector2> &getVelocityField() { return m_velocityField; }
  const std::vector<Vector2> &getPrevVelocityField() {
    return m_prevVelocityField;
  }
  const std::vector<float> &getDensityField() { return m_density; }
  std::vector<float> getPrevDensityField() { return m_prevDensity; }

#ifdef TESTS
  void printVector(const std::vector<float> vector) {
    for (int i{1}; i <= m_boxHeight; ++i) {
      for (int j{1}; j <= m_boxWidth; ++j) {
        std::cout << vector[IX(j, i)] << " ";
      }
      std::cout << "\n";
    }
  }

  void printVector(const std::vector<Vector2> vector) {
    for (int i{1}; i <= m_boxHeight; ++i) {
      for (int j{1}; j <= m_boxWidth; ++j) {
        std::cout << "(" << (vector[IX(j, i)].x) << "," << (vector[IX(j, i)].y)
                  << ")";
      }
      std::cout << "\n";
    }
  }

  void testAddDensitySource() {
    // m_prevDensity[IX(1,1)] = 3.f;
    m_prevDensity[IX(m_boxWidth / 2 + 1, m_boxHeight / 2 + 1)] = 5.f;
    m_prevDensity[IX(m_boxWidth / 2 , m_boxHeight / 2 )] = 5.f;
    // m_prevDensity[IX(m_boxWidth, m_boxHeight)] = 3.f;
    // add_source(m_prevDensity, m_density);
  }

  void testAddVelocitySource() {
    m_velocityField[IX(m_boxWidth / 2 + 1, m_boxHeight / 2 + 1)] =
        Vector2{2, 1};
    m_velocityField[IX(m_boxWidth / 2 , m_boxHeight / 2 + 1)] =
        Vector2{2, 1};
    m_velocityField[IX(m_boxWidth / 2 + 1, m_boxHeight / 2 )] =
        Vector2{2, 1};
    m_velocityField[IX(m_boxWidth / 2, m_boxHeight / 2)] =
        Vector2{2, 1};
    m_velocityField[IX(m_boxWidth / 2 + 2, m_boxHeight / 2 + 2)] =
        Vector2{1, 1};
    // add_source(m_prevVelocityField, m_velocityField);
  }

  void testDiffuseDensity() { diffuse(m_density, m_prevDensity, 0.005f); }
  void testDiffuseVelocity() {
    diffuse(m_velocityField, m_prevVelocityField, 0.005f);
  }
#endif

private:
  const int m_boxWidth;
  const int m_boxHeight;

  std::vector<Vector2> m_velocityField;
  std::vector<Vector2> m_prevVelocityField;

  std::vector<float> m_density;
  std::vector<float> m_prevDensity;

  const float m_timestep;

  // helper converts (x,y) coords into 1D array coords
  size_t IX(const int x, const int y) {
    return static_cast<size_t>(x + (y * m_boxHeight));
  };

  // forces will either be added to velocity field(type Vector2), or density
  // field (type float). Hence, function overloading.
  void add_source(const std::vector<Vector2> &forceSource,
                  std::vector<Vector2> &targetVelocityField);
  void add_source(const std::vector<float> &forceSource,
                  std::vector<float> &targetDensityField);

  void diffuse(std::vector<Vector2> &source,
               const std::vector<Vector2> &prevSource,
               const float diffusionRate);
  void diffuse(std::vector<float> &source, const std::vector<float> &prevSource,
               const float diffusionRate);

  void advect(std::vector<float> &densityField,
              std::vector<float> &prevDensityField,
              std::vector<Vector2> &velocityField);

  void clear_divergence(std::vector<Vector2> velocityField,
                        std::vector<Vector2> prevVelocityField);
};
#endif
