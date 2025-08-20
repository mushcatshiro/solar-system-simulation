#ifndef CELESTIALCONST
#define CELESTIALCONST

namespace CelestialConst {
  constexpr float AU = 1.496e11;
  constexpr float GravitationalConstant = 6.67430e-11f;  // Gravitational constant in m^3 kg^-1 s^-2 
  // Sun
  constexpr float SunRadius = 6.96e8;  // m
  constexpr float SunMass = 1.988e30;  // kg

  // Mecury
  constexpr float MecuryRadius = 2.439e6;  // m
  constexpr float MecuryDistanceFromSun = 0.32;  // AU
  constexpr float MecuryMass = 3.3e23;  // 0.055 earth
  constexpr float MecuryOrbitalSpeed = 4.736e4; // m/s
  constexpr float MecurySemiMajorAxis = 0.387; // AU
  constexpr float MecuryAphelion = 0.467;  // AU
  constexpr float MecuryPeriphelion = 0.31;  // AU

  // Venus
  constexpr float VenusRadius = 6.051e6;
  constexpr float VenusDistanceFromSun = 0.72;
  constexpr float VenusMass = 4.87e24;  // 0.82
  constexpr float VenusOrbitalSpeed = 3.502e4;
  constexpr float VenusSemiMajorAxis = 0.723;
  constexpr float VenusAphelion = 0.728;
  constexpr float VenusPeriphelion = 0.718;

  // Earth
  constexpr float EarthRadius = 6.371e6;
  constexpr float EarthDistanceFromSun = 1.0;
  constexpr float EarthMass = 5.972e24;
  constexpr float EarthOrbitalSpeed = 2.978e4;
  constexpr float EarthSemiMajorAxis = 1.496e11;
  constexpr float EarthAphelion = 1.02;
  constexpr float EarthPeriphelion = 0.98;

  // Moon
  constexpr float MoonRadius = 1.737e6;
  constexpr float MoondDistanceFromEarth = 0.0026;
  constexpr float MoonMass = 7.342e22;  // 0.013
  constexpr float MoonOrbitalSpeed = 1.022e3;
  constexpr float MoonSemiMajorAxis = 0.00257;
  constexpr float MoonApogee = 0.0027;
  constexpr float MoonPerigee = 0.0024;

  // Mars
  constexpr float MarsRadius = 3.398e6;
  constexpr float MarsDistanceFromSun = 1.5;
  constexpr float MarsMass = 6.42e23;  // 0.107
  constexpr float MarsOrbitalSpeed = 2.407e4;
  constexpr float MarsSemiMajorAxis = 1.52;
  constexpr float MarsAphelion = 1.67;
  constexpr float MarsPeriphelion = 1.38;

  // Jupiter
  constexpr float JupiterRadius = 6.99e7;
  constexpr float JupiterDistanceFromSun = 5.2;
  constexpr float JupiterMass = 1.89e27;  // 317.8
  constexpr float JupiterOrbitalSpeed = 1.306e4;
  constexpr float JupiterSemiMajorAxis = 5.2;
  constexpr float JupiterAphelion = 5.46;
  constexpr float JupiterPeriphelion = 4.95;

  // Saturn
  constexpr float SaturnRadius = 5.82e7;
  constexpr float SaturnDistanceFromSun = 9.5;
  constexpr float SaturnMass = 5.68e26;  // 95.16
  constexpr float SaturnOrbitalSpeed = 9.68e3;
  constexpr float SaturnSemiMajorAxis = 9.58;
  constexpr float SaturnAphelion = 10.12;
  constexpr float SaturnPeriphelion = 9.04;

  // Uranus
  constexpr float UranusRadius = 2.53e7;
  constexpr float UranusDistanceFromSun = 19.0;
  constexpr float UranusMass = 8.68e25;  // 14.54
  constexpr float UranusOrbitalSpeed = 6.8e3;
  constexpr float UranusSemiMajorAxis = 19.19;
  constexpr float UranusAphelion = 20.1;
  constexpr float UranusPeriphelion = 18.29;

  // Neptune
  constexpr float NeptuneRadius = 2.46e7;
  constexpr float NeptuneDistanceFromSun = 30.0;
  constexpr float NeptuneMass = 1.024e26;  // 17.15
  constexpr float NeptuneOrbitalSpeed = 5.43e3;
  constexpr float NeptuneSemiMajorAxis = 30.07;
  constexpr float NeptuneAphelion = 30.33;
  constexpr float NeptunePeriphelion = 29.81;

  // Pluto
  constexpr float PlutoRadius = 1.188e6;
  constexpr float PlutoDistanceFromSun = 39.5;
  constexpr float PlutoMass = 1.3e22;  // 0.00218
  constexpr float PlutoOrbitalSpeed = 4.74e3;
  constexpr float PlutoSemiMajorAxis = 39.48;
  constexpr float PlutoAphelion = 49.31;
  constexpr float PlutoPeriphelion = 29.66;
}

#endif