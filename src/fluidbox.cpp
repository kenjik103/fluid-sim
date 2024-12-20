#include "fluidbox.h"
#include "raylib.h"

#include <cassert>
#include <cmath>
#include <cstddef>
#include <vector>
/**
 * PUBLIC METHODS
 *
 * */
void FluidBox::update_density(float diffusionRate) {
  add_source(m_prevDensity, m_density);
  std::swap(m_prevDensity, m_density);
  diffuse(m_density, m_prevDensity, diffusionRate);
  std::swap(m_prevDensity, m_density);
  advect(m_density, m_prevDensity, m_velocityField);
}

/**
 * PRIVATE METHODS
 * */
FluidBox::FluidBox(int screenWidth, int screenHeight, float timestep)
    : m_boxWidth{screenWidth}, m_boxHeight{screenHeight}, m_timestep{timestep} {
  std::size_t size =
      static_cast<std::size_t>((screenWidth + 2) * (screenHeight + 2));
  assert(size > 0);
  m_velocityField = std::vector<Vector2>(size);
  m_prevVelocityField = std::vector<Vector2>(size);
  m_density = std::vector<float>(size, 0.f);
  m_prevDensity = std::vector<float>(size, 0.f);
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
        source[IX(j, i)].x =
            (prevSource[IX(j, i)].x +
             k * (prevSource[IX(j - 1, i)].x + prevSource[IX(j + 1, i)].x +
                  prevSource[IX(j, i - 1)].x + prevSource[IX(j, i + 1)].x)) /
            (1 + 4 * k);
        source[IX(j, i)].y =
            (prevSource[IX(j, i)].y +
             k * (prevSource[IX(j - 1, i)].y + prevSource[IX(j + 1, i)].y +
                  prevSource[IX(j, i - 1)].y + prevSource[IX(j, i + 1)].y)) /
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
        source[IX(j, i)] =
            (prevSource[IX(j, i)] +
             k * (prevSource[IX(j - 1, i)] + prevSource[IX(j + 1, i)] +
                  prevSource[IX(j, i - 1)] + prevSource[IX(j, i + 1)])) /
            (1 + 4 * k);
      }
    }
  }
}
void FluidBox::advect(std::vector<float> &densityField,
                      std::vector<float> &prevDensityField,
                      std::vector<Vector2> &velocityField) {
  assert(m_boxWidth > 0 && m_boxHeight > 0);
  float deltaTime{m_timestep *
                  static_cast<float>(std::sqrt(m_boxWidth * m_boxHeight))};

  assert(deltaTime != 0);
  float densitySourceX{};
  float densitySourceY{};
  int x_0{};
  int x_1{};
  int y_0{};
  int y_1{};

  float fract_x{};
  float fract_y{};

  for (int i{1}; i <= m_boxHeight; ++i) {
    for (int j{1}; j <= m_boxWidth; ++j) {
      densitySourceX =
          static_cast<float>(j) - deltaTime * velocityField[IX(j, i)].x;
      densitySourceY =
          static_cast<float>(i) - deltaTime * velocityField[IX(j, i)].y;

      // out of bounds clamps
      if (densitySourceX < 0.5) {
        densitySourceX = 0.5;
      }
      if (densitySourceY < 0.5) {
        densitySourceY = 0.5;
      }
      if (densitySourceX > (static_cast<float>(m_boxWidth) + 0.5f)) {
        densitySourceX = static_cast<float>(m_boxWidth) + 0.5f;
      }
      if (densitySourceY > (static_cast<float>(m_boxHeight) + 0.5f)) {
        densitySourceY = static_cast<float>(m_boxHeight) + 0.5f;
      }

      // get coordinates of desired densities
      x_0 = static_cast<int>(std::floor(densitySourceX));
      x_1 = x_0 + 1;
      y_0 = static_cast<int>(std::floor(densitySourceY));
      y_1 = y_0 + 1;

      // get fractional bits between coordinates and actual densitySource
      fract_x = densitySourceX - static_cast<float>(x_0);
      fract_y = densitySourceY - static_cast<float>(y_0);

      // lerp between our 4 desired densities, update target density to reflect

      densityField[IX(j, i)] =
          std::lerp(std::lerp(prevDensityField[IX(x_0, y_0)],
                              prevDensityField[IX(x_1, y_0)], fract_x),
                    std::lerp(prevDensityField[IX(x_0, y_1)],
                              prevDensityField[IX(x_1, y_1)], fract_x),
                    fract_y);
    }
  }
}
