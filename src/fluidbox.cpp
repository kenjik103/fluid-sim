#include "fluidbox.h"
#include "raylib.h"

#include <cassert>
#include <cstddef>
#include <vector>

using std::vector;

FluidBox::FluidBox(int screenWidth, int screenHeight, float timestep)
    : m_boxWidth{screenWidth}, m_boxHeight{screenHeight}, m_timestep{timestep} {
  std::size_t size = static_cast<std::size_t>((screenWidth + 2) * (screenHeight + 2));
  assert(size > 0);
  m_velocityField = vector<Vector2>(size);
  m_prevVelocityField = vector<Vector2>(size);
  m_density = vector<float>(size, 1.f);
  m_prevDensity = vector<float>(size, 1.f);
}

void FluidBox::add_source(const std::vector<Vector2> &forceSource,
                          std::vector<Vector2> &targetVelocityField) {
  size_t size = static_cast<size_t>((m_boxWidth + 2) * (m_boxHeight + 2));
  assert(size == forceSource.size());
  assert(size == targetVelocityField.size());
  for (size_t i{}; i < size; ++i) {
    targetVelocityField[i].x += forceSource[i].x;
    targetVelocityField[i].y += forceSource[i].y;
  }
}

void FluidBox::add_source(const std::vector<float> &forceSource,
                          std::vector<float> &targetDensityField) {
  size_t size = static_cast<size_t>((m_boxWidth + 2) * (m_boxHeight + 2));
  assert(size == forceSource.size());
  assert(size == targetDensityField.size());
  for (size_t i{}; i < size; ++i) {
    targetDensityField[i] += forceSource[i];
  }
}

void FluidBox::diffuse(std::vector<Vector2> &source,
                       const std::vector<Vector2> &prevSource,
                       const float diffusionRate) {
  float k{m_timestep * diffusionRate *
          static_cast<float>(m_boxWidth * m_boxHeight)};
  for (int GSIteration{}; GSIteration < 20;
       ++GSIteration) { // run 20 iterations of Gauss Seidel to solve our system
                        // of equations for density.
    for (int i{1}; i <= m_boxHeight; ++i) {
      for (int j{1}; j <= m_boxWidth; ++j) {
         source[IX(i, j)].x =
            (prevSource[IX(i, j)].x +
             k * (prevSource[IX(i - 1, j)].x + prevSource[IX(i + 1, j)].x +
                  prevSource[IX(i, j - 1)].x + prevSource[IX(i, j + 1)].x)) /
            (1 + 4 * k);
         source[IX(i, j)].y =
            (prevSource[IX(i, j)].y +
             k * (prevSource[IX(i - 1, j)].y + prevSource[IX(i + 1, j)].y +
                  prevSource[IX(i, j - 1)].y + prevSource[IX(i, j + 1)].y)) /
            (1 + 4 * k);
        }
    }
  }
}
void FluidBox::diffuse(std::vector<float> &source,
                       const std::vector<float> &prevSource,
                       const float diffusionRate) {
  float k{m_timestep * diffusionRate *
          static_cast<float>(m_boxWidth * m_boxHeight)};
  for (int GSIteration{}; GSIteration < 20;
       ++GSIteration) { // run 20 iterations of Gauss Seidel to solve our system
                        // of equations for density.
    for (int i{1}; i <= m_boxHeight; ++i) {
      for (int j{1}; j <= m_boxWidth; ++j) {
         source[IX(i, j)] =
            (prevSource[IX(i, j)] +
             k * (prevSource[IX(i - 1, j)] + prevSource[IX(i + 1, j)] +
                  prevSource[IX(i, j - 1)] + prevSource[IX(i, j + 1)])) /
            (1 + 4 * k);
        }
    }
  }
}
