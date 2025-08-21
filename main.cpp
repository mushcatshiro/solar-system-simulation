#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <cstdint>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <iostream>
#include <chrono> // For std::chrono::seconds and other time durations
#include <thread>
#include <string>

// check for float overflow
#include <cfenv>
#include <limits>

// constants
#include <celestialconst.hpp>
using namespace CelestialConst;

/*
cpp float range [3.4e-38, 3.4e+38]
*/

constexpr float ViewportWidth = 800.f;
constexpr float ViewportHeight = 800.f;
constexpr float t = 86400.f;  // 1 tick = 1 hour (3600 seconds)
constexpr float m2vp = (ViewportWidth) / (440000000.f * 2); // pix/m
constexpr float au2vp = (ViewportWidth) / (55.f * 2) ;  // pix/au
constexpr size_t ListLength = 32;
constexpr int HeadGreyValue = 200;

struct CelestialObj {
  std::string name;
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

  CelestialObj(std::string n, float x_m, float y_m, float r_m, float m_kg, sf::Color c, bool st)
    : name(n), x(x_m), y(y_m), radius(r_m), mass(m_kg), color(c), shape(r_m * m2vp), showTrail(st) {
    vx = 0.f;
    vy = 0.f;
  }

  glm::vec2 calculateGForce(std::vector<CelestialObj> celestialBodies) {
    glm::vec2 acceleration = glm::vec2{0., 0.};
    for (auto& c : celestialBodies) {
      if (c.name != this->name) {
        glm::vec2 direction = glm::vec2{c.x, c.y} - glm::vec2{this->x, this->y};
        float r = glm::length(direction);  // sqrt((x^2) + (y^2))
        glm::vec2 unitDirection = direction / r;
        acceleration += (this->mass / (r * r)) * unitDirection;
      }
    }
    acceleration *= GravitationalConstant;
    std::cout << acceleration.x << " " << acceleration.y << std::endl;
    return acceleration;
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

CelestialObj sun{"sun", 0., 0., SunRadius, SunMass, sf::Color{200, 200, 10}, false};
CelestialObj mecury{"mecury", MecuryDistanceFromSun, 0., MecuryRadius, MecuryMass, sf::Color(116, 123, 129), true};
CelestialObj venus{"venus", VenusDistanceFromSun, 0.,  VenusRadius, VenusMass, sf::Color(255, 198, 73), true};
CelestialObj earth{"earth", 0., 0., EarthRadius, EarthMass, sf::Color(159, 193, 100), false};
CelestialObj moon{"moon", 3.633e8, 0., MoonRadius, MoonMass, sf::Color(201, 201, 201), true};
CelestialObj mars{"mars", MarsDistanceFromSun, 0., MarsRadius, MarsMass, sf::Color(129, 37, 27), true};
CelestialObj jupiter{"jupiter", JupiterDistanceFromSun, 0., JupiterRadius, JupiterMass, sf::Color(188, 175, 178), true};
CelestialObj saturn{"saturn", SaturnDistanceFromSun, 0., SaturnRadius, SaturnMass, sf::Color(196, 188, 170), true};
CelestialObj uranus{"uranus", UranusDistanceFromSun, 0., UranusRadius, UranusMass, sf::Color(172, 229, 238), true};
CelestialObj neptune{"neptune", NeptuneDistanceFromSun, 0., NeptuneRadius, NeptuneMass, sf::Color(124, 183, 187), true};
CelestialObj pluto{"pluto", PlutoDistanceFromSun, 0., PlutoRadius, PlutoMass, sf::Color(146, 168, 164), true};

std::vector<CelestialObj> solarSystem = {earth, moon};

glm::vec2 calculateGForce(const CelestialObj& p1, const CelestialObj& p2) {
  // calculates f `p1` exerts on `p2`, direction is towards `p1`
  glm::vec2 direction = glm::vec2{p1.x, p1.y} - glm::vec2{p2.x, p2.y};
  float r = glm::length(direction);  // sqrt((x^2) + (y^2))
  glm::vec2 unitDirection = direction / r;
  float mag = GravitationalConstant * p1.mass * p2.mass / (r * r);
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
  sf::RenderWindow window(sf::VideoMode({static_cast<unsigned int>(ViewportWidth), static_cast<unsigned int>(ViewportHeight)}), "Solar System Simulation");
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
    // for (auto& c: solarSystem) {
    //   glm::vec2 a = c.calculateGForce(solarSystem);
    //   c.updatePos(a, false);
    // }

    // clear the window with black color
    window.clear(sf::Color::Black);
    // draw everything here...
    earth.render(window);
    moon.render(window);

    // end the current frame
    window.display();
    ctr += 1;
    // std::cout << "ctr: " << ctr << std::endl;
    // std::this_thread::sleep_for(std::chrono::microseconds(100));
    // std::this_thread::sleep_for(std::chrono::seconds(1)); 
  }
  if (std::fetestexcept(FE_OVERFLOW)) {
    std::cout << "Floating-point overflow detected!" << std::endl;
  } else {
    std::cout << "No floating-point overflow." << std::endl;
  }
  while (true) {}
  return 0;
}