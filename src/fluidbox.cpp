#include "fluidBox.h"
#include "raylib.h"

#include <cassert>
#include <cmath>
#include <vector>

FluidBox::FluidBox(int size, int scale, float diffusion, float viscosity,
                   float timestep)
    : m_size{size}, m_scale{scale}, m_timestep{timestep},
      m_diffusion{diffusion}, m_viscosity{viscosity} {
  assert(size > 0);
  size_t N{static_cast<size_t>(size)};
  m_density = std::vector<float>(N * N);
  m_prevDensity = std::vector<float>(N * N);

  m_Vx = std::vector<float>(N * N);
  m_Vx0 = std::vector<float>(N * N);
  m_Vy = std::vector<float>(N * N);
  m_Vy0 = std::vector<float>(N * N);
}

void FluidBox::addDensity(int x, int y, float ammount) {
  m_density[IX(x / m_scale, y / m_scale)] += ammount;
}

void FluidBox::addVelocity(int x, int y, float ammountX, float ammountY) {
  m_Vx[IX(x / m_scale, y / m_scale)] += ammountX;
  m_Vy[IX(x / m_scale, y / m_scale)] += ammountY;
}

size_t FluidBox::IX(int x, int y) {
  if (x > m_size - 1){
    x = m_size - 1;
  }
  if (y > m_size - 1){
    y = m_size - 1;
  }  
  if (x < 0){
    x = 0;
  }
  if (y < 0){
    y = 0;
  }

  return static_cast<size_t>(x + (y * m_size));
}

void FluidBox::step() {
  diffuse(1, m_Vx0, m_Vx, m_viscosity, 4);
  diffuse(2, m_Vy0, m_Vy, m_viscosity, 4);

  project(m_Vx0, m_Vy0, m_Vx, m_Vy, 4);

  advect(1, m_Vx, m_Vx0, m_Vx0, m_Vy0);
  advect(2, m_Vy, m_Vy0, m_Vx0, m_Vy0);

  project(m_Vx, m_Vy, m_Vx0, m_Vy0, 4);

  diffuse(0, m_prevDensity, m_density, m_diffusion, 4);
  advect(0, m_density, m_prevDensity, m_Vx, m_Vy);
}

void FluidBox::render(float fadeSpeed, float dyeBrightness) {
  for (int y{1}; y < m_size - 1; ++y) {
    for (int x{1}; x < m_size - 1; ++x) {
      float density = m_density[IX(x,y)];
      //fade
      if (density > 0.f){
        m_density[IX(x,y)] -= fadeSpeed;
      }
      //clamps
      float alpha = dyeBrightness * density;
      if (alpha > 255.f){
        alpha = 255.f;
      }
      Color c = Color(255.f, 255.f, 255.f, alpha);
      DrawRectangle(x * m_scale, y * m_scale, m_scale, m_scale, c);
    }
  }
}

// TODO: get rid of the wierd b bounds detection thingy
// b==2 top and bottom
// b==1 left and right
void FluidBox::set_bnd(int b, std::vector<float> &x) {
  for (int i{1}; i < m_size - 1; ++i) {
    x[IX(i, 0)] = b == 2 ? -x[IX(i, 1)] : x[IX(i, 1)];
    x[IX(i, m_size - 1)] =
        b == 2 ? -x[IX(i, m_size - 2)] : x[IX(i, m_size - 2)];
  }
  for (int j{1}; j < m_size - 1; ++j) {
    x[IX(0, j)] = b == 1 ? -x[IX(1, j)] : x[IX(1, j)];
    x[IX(m_size - 1, j)] =
        b == 1 ? -x[IX(m_size - 2, j)] : x[IX(m_size - 2, j)];
  }

  x[IX(0, 0)] = 0.5f * (x[IX(1, 0)] + x[IX(0, 1)]);
  x[IX(0, m_size - 1)] = 0.5f * (x[IX(1, m_size - 1)] + x[IX(0, m_size - 2)]);
  x[IX(m_size - 1, 0)] = 0.5f * (x[IX(m_size - 1, 1)] + x[IX(m_size - 2, 0)]);
  x[IX(m_size - 1, m_size - 1)] =
      0.5f * (x[IX(m_size - 1, m_size - 2)] + x[IX(m_size - 2, m_size - 1)]);
}

void FluidBox::diffuse(int b, std::vector<float> &x,
                       const std::vector<float> &x0, float diff,
                       int gaussSeidelIterations) {
  float a = m_timestep * diff * (m_size - 2) * (m_size - 2);
  float k = 1.f / (a * 4.0f + 1.f);
  for (int gs{}; gs < gaussSeidelIterations; gs++) {
    for (int i{1}; i < m_size - 1; i++) {
      for (int j{1}; j < m_size - 1; j++) {
        x[IX(j, i)] = (x0[IX(j, i)] + a * (x[IX(j, i - 1)] + x[IX(j, i + 1)] +
                                           x[IX(j - 1, i)] + x[IX(j + 1, i)])) *
                      k;
      }
    }
  }
  set_bnd(b, x);
}

void FluidBox::project(std::vector<float> &velX, std::vector<float> &velY,
                       std::vector<float> &p, std::vector<float> &divergence,
                       int gaussSeidelIterations) {
  for (int y{1}; y < m_size - 1; ++y) {
    for (int x{1}; x < m_size - 1; ++x) {
      divergence[IX(x, y)] = -0.5f *
                             (velX[IX(x + 1, y)] - velX[IX(x - 1, y)] +
                              velY[IX(x, y + 1)] - velY[IX(x, y - 1)]) /
                             m_size;
      p[IX(x, y)] = 0.f;
    }
  }
  set_bnd(0, divergence);
  set_bnd(0, p);
  for (int gs{}; gs < gaussSeidelIterations; gs++) {
    for (int i{1}; i < m_size - 1; i++) {
      for (int j{1}; j < m_size - 1; j++) {
        p[IX(j, i)] =
            (divergence[IX(j, i)] + (p[IX(j, i - 1)] + p[IX(j, i + 1)] +
                                     p[IX(j - 1, i)] + p[IX(j + 1, i)])) *
            (1.f / 4.f);
      }
    }
  }
  set_bnd(0, p);

  for (int y{1}; y < m_size - 1; ++y) {
    for (int x{1}; x < m_size - 1; ++x) {
      velX[IX(x, y)] -= 0.5f * (p[IX(x + 1, y)] - p[IX(x - 1, y)]) * m_size;
      velY[IX(x, y)] -= 0.5f * (p[IX(x, y + 1)] - p[IX(x, y - 1)]) * m_size;
    }
  }
  set_bnd(1, velX);
  set_bnd(2, velY);
}
void FluidBox::advect(int b, std::vector<float> &d, std::vector<float> &d0,
                      const std::vector<float> &velX,
                      const std::vector<float> &velY) {
  float i0{};
  float i1{};
  float j0{};
  float j1{};

  float dtScaled{m_timestep * (m_size - 2)};

  float s1{};
  float t1{};
  float x{};
  float y{};

  float Nfloat{static_cast<float>(m_size)};
  for (int j{1}; j < m_size - 1; j++) {
    for (int i{1}; i < m_size - 1; i++) {
      x = static_cast<float>(i) - dtScaled * velX[IX(i, j)];
      y = static_cast<float>(j) - dtScaled * velY[IX(i, j)];

      // set bounds
      if (x < 0.5f) {
        x = 0.5f;
      }
      if (x > Nfloat + 0.5f) {
        x = Nfloat + 0.5f;
      }
      if (y < 0.5f) {
        y = 0.5f;
      }
      if (y > Nfloat + 0.5f) {
        y = Nfloat + 0.5f;
      }

      // floor to find index
      i0 = std::floor(x);
      i1 = i0 + 1.0f;
      j0 = std::floor(y);
      j1 = j0 + 1.0f;

      // get fractional bit for "real" position
      s1 = x - i0;
      t1 = y - j0;

      int i0i{static_cast<int>(i0)};
      int i1i{static_cast<int>(i1)};
      int j0i{static_cast<int>(j0)};
      int j1i{static_cast<int>(j1)};

      d[IX(i, j)] =
          std::lerp(std::lerp(d0[IX(i0i, j0i)], d0[IX(i1i, j0i)], s1),
                    std::lerp(d0[IX(i0i, j1i)], d0[IX(i1i, j1i)], s1), t1);
    }
  }
  set_bnd(b, d);
}
