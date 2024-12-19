#include "fluidbox.h"
#include "raylib.h"

#include <cassert>
#include <cstddef>
#include <iostream>
#include <vector>

using std::vector;

FluidBox::FluidBox(int screenWidth, int screenHeight, float timestep)
    : m_boxWidth{screenWidth}, m_boxHeight{screenHeight}, m_timestep{timestep} {
  std::size_t size = static_cast<std::size_t>(screenWidth * screenHeight);
  assert(size > 0);
  m_velocityField = vector<Vector2>(size);
  m_prevVelocityField = vector<Vector2>(size);
  m_density = vector<float>(size);
  m_prevDensity = vector<float>(size);
}

void FluidBox::add_source(const std::vector<Vector2> &forceSource,
                          std::vector<Vector2> &targetVelocityField) {
  size_t size = static_cast<size_t>(m_boxWidth * m_boxHeight);
  assert(size == forceSource.size());
  assert(size == targetVelocityField.size());
  for (size_t i{}; i < size; ++i) {
    targetVelocityField[i].x += forceSource[i].x;
    targetVelocityField[i].y += forceSource[i].y;
  }
}

void FluidBox::add_source(const std::vector<float> &forceSource,
                          std::vector<float> &targetDensityField) {
  size_t size = static_cast<size_t>(m_boxWidth * m_boxHeight);
  assert(size == forceSource.size());
  assert(size == targetDensityField.size());
  for (size_t i{}; i < size; ++i) {
    targetDensityField[i] += forceSource[i];
  }
}

void FluidBox::diffuse(std::vector<Vector2> &source,
                       const std::vector<Vector2> &prevSource,
                       const float diffusionRate) {}
void FluidBox::diffuse(std::vector<float> &source,
                       const std::vector<float> &prevSource,
                       const float diffusionRate) {
  float k{m_timestep * diffusionRate *
          static_cast<float>(m_boxWidth * m_boxHeight)};
  for (int GSIteration{}; GSIteration < 20;
       ++GSIteration) { // run 20 iterations of Gauss Seidel to solve our system
                        // of equations for density.
    for (int i{}; i < m_boxHeight; ++i) {
      for (int j{}; j < m_boxWidth; ++j) {
        source[IX(i, j)] =
            (prevSource[IX(i, j)] +
             k * (prevSource[IX(i - 1, j)] + prevSource[IX(i + 1, j)] +
                  prevSource[IX(i, j - 1)] + prevSource[IX(i, j + 1)])) /
            (1 + 4 * k);
      }
    }
  }
}
