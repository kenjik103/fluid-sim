#ifndef FLUIDBOX
#define FLUIDBOX

#include <cstddef>
#include <vector>

class FluidBox {
public:
  FluidBox(int size, int scale, float diffusion, float viscosity, float timestep);

  void addDensity(int x, int y, float ammount);

  void addVelocity(int x, int y, float ammountX, float ammountY);

  void step();

  void render();

private:
  // converts 2D coords into 1D
  size_t IX(int x, int y);

  void set_bnd(int b, std::vector<float> &x);

  void diffuse(int b, std::vector<float> &x, const std::vector<float> &x0,
               float diff, int gaussSeidelIterations);

  void project(std::vector<float> &velX, std::vector<float> &velY,
               std::vector<float> &p, std::vector<float> &divergence,
               int gaussSeidelIterations);

  void advect(int b, std::vector<float> &d, std::vector<float> &d0,
              const std::vector<float> &velX, const std::vector<float> &velY);

  const int m_size;
  const int m_scale;
  const float m_timestep;
  const float m_diffusion;
  const float m_viscosity;

  std::vector<float> m_density;
  std::vector<float> m_prevDensity;

  std::vector<float> m_Vx;
  std::vector<float> m_Vx0;
  std::vector<float> m_Vy;
  std::vector<float> m_Vy0;
};
#endif
