#include <iostream>

// for master branch
// #include "al/core.hpp"

// for devel branch
#include "al/app/al_App.hpp"
#include "al/graphics/al_Shapes.hpp"

using namespace al;

#include "SimplerSineEnvApp.hpp"
#include "SineEnv.hpp"

int main()
{
  // Create app instance
  SimplerSineEnvApp app;

  // Set up audio
  app.configureAudio(48000., 512, 2, 0);

  app.start();

  return 0;
}
