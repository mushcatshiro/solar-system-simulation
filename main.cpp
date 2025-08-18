#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <cstdint>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <iostream>
#include <chrono> // For std::chrono::seconds and other time durations
#include <thread>

#include <cfenv>
#include <limits>
// #include <glm/gtx/compatibility.hpp>

/*
lunar distance 384,400 km
earth
  mean radius 6371 km
  mass 5.972 x 10^24 kg
  
moon
  mean radius 1737.4 km
  mass 7.346 x 10^22 kg

cpp float range [3.4e-38, 3.4e+38]
*/

constexpr float ViewportWidth = 800.f;
constexpr float ViewportHeight = 800.f;
constexpr float AstronomicalUnit = 1.4960e11;
constexpr float LunarDistance = 3.844e8; // in m
constexpr float SunRadius = 6.96e8;
constexpr float EarthRadius = 6.371e6;     // in m
constexpr float MoonRadius = 1.737e6;      // in m
constexpr float G = 6.67430e-11f;            // Gravitational constant in m^3 kg^-1 s^-2
constexpr float MassSun = 1.989e30;
constexpr float MassEarth = 5.972e24f;       // kg
constexpr float MassMoon = 7.342e22f;        // kg
constexpr float t = 86400.f;               // 1 tick = 1 hour (3600 seconds)
constexpr float m2vp = (ViewportWidth) / (440000000.f * 2); // pix/km
constexpr size_t ListLength = 32;
constexpr int HeadGreyValue = 200;

struct PlanetaryObj {
  sf::CircleShape shape;
  float x;
  float y;
  float radius;
  float mass;
  float vx;
  float vy;
  sf::Color color;
  std::vector<sf::CircleShape> trail;
  bool showTrail;

  PlanetaryObj(float x_m, float y_m, float r_m, float m_kg, sf::Color c, bool st)
    : x(x_m), y(y_m), radius(r_m), mass(m_kg), color(c), shape(r_m * m2vp), showTrail(st) {
    vx = 0.f;
    vy = 0.f;
  }

  void updatePos(glm::vec2 acceleration, bool debug) {
    sf::CircleShape trailPoint(radius * m2vp); // Adjust the size and color as needed
    // trailPoint.setFillColor(sf::Color(color.r, color.r, color.r, 127)); // Slightly transparent
    trailPoint.setOrigin({trailPoint.getRadius(), trailPoint.getRadius()});
    trailPoint.setPosition({ViewportWidth / 2.f + x * m2vp, ViewportHeight / 2.f + y * m2vp});
    trail.push_back(trailPoint);
    if (trail.size() >= ListLength) {
      trail.erase(trail.begin());
    }

    this->vx += acceleration.x * t;
    this->vy += acceleration.y * t;
    this->x += vx * t;
    this->y += vy * t;
    if (debug) {
      std::cout << "vx, vy:" << vx << "," << vy << std::endl;
      std::cout << "x, y:" << x << "," << y << std::endl;
    }
  };

  void render(sf::RenderWindow& window) {
    /*
    unsuccessful attempt to show trail with color spectrum indicating start/end
    */
    // for (size_t i = 0; i < trail.size(); i++) {
    //   sf::CircleShape t = trail[i];
    //   t.setFillColor(sf::Color{
    //     static_cast<int>(HeadGreyValue) - i * 2,
    //     static_cast<int>(HeadGreyValue) - i * 2,
    //     static_cast<int>(HeadGreyValue) - i * 2
    //   });
    //   window.draw(t);
    // }
    if (showTrail) {
      for (const auto& trailPoint : trail) {
        window.draw(trailPoint);
      }
    }
    
    shape.setOrigin({shape.getRadius(), shape.getRadius()});
    shape.setFillColor(color);
    shape.setPosition({ViewportWidth / 2.f + x * m2vp, ViewportHeight / 2.f + y * m2vp});
    window.draw(shape);
  };
};

PlanetaryObj earth{0., 0., EarthRadius, MassEarth, sf::Color(100, 250, 50), false};
PlanetaryObj moon{3.633e8, 0., MoonRadius, MassMoon, sf::Color(100, 50, 250), true};

glm::vec2 calculateGForce(const PlanetaryObj& earth, const PlanetaryObj& moon) {
  // calculates f earth exerts on moon, direction is towards earth
  glm::vec2 direction = glm::vec2{earth.x, earth.y} - glm::vec2{moon.x, moon.y};
  float r = glm::length(direction);  // sqrt((x^2) + (y^2))
  glm::vec2 unitDirection = direction / r;
  float mag = G * earth.mass * moon.mass / (r * r);
  std::cout << unitDirection.x << unitDirection.y << std::endl;
  return mag * unitDirection;
};

int main() {
  /*
  TODO
  - center of mass frame?
  - euler integration/velocity verlet/rk4
  */
  // create the window
  std::feclearexcept(FE_OVERFLOW);
  sf::RenderWindow window(sf::VideoMode({static_cast<unsigned int>(ViewportWidth), static_cast<unsigned int>(ViewportHeight)}), "My window");
  int ctr = 0;

  // Calculate initial orbital velocity for a stable circular orbit
  moon.vy = 1.076e3;
  earth.vy = -(moon.vy * moon.mass) / earth.mass; // conservation of momentum

  // run the program as long as the window is open
  while (window.isOpen()) {
    // check all the window's events that were triggered since the last iteration of the loop
    while (const std::optional event = window.pollEvent()) {
      // "close requested" event: we close the window
      if (event->is<sf::Event::Closed>())
        window.close();
    }

    glm::vec2 F = calculateGForce(earth, moon);
    glm::vec2 accelEarth = -F / earth.mass;
    glm::vec2 accelMoon = F / moon.mass;
    earth.updatePos(accelEarth, false);
    moon.updatePos(accelMoon, false);

    // clear the window with black color
    window.clear(sf::Color::Black);
    // draw everything here...
    earth.render(window);
    moon.render(window);

    // end the current frame
    window.display();
    ctr += 1;
    // std::cout << "ctr: " << ctr << std::endl;
    std::this_thread::sleep_for(std::chrono::microseconds(100));
    // std::this_thread::sleep_for(std::chrono::seconds(1)); 
  }
  if (std::fetestexcept(FE_OVERFLOW)) {
    std::cout << "Floating-point overflow detected!" << std::endl;
  } else {
    std::cout << "No floating-point overflow." << std::endl;
  }
  // while (true) {}
  return 0;
}