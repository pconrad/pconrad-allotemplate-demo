#ifndef SIMPLER_SINE_ENV_APP_HPP
#define SIMPLER_SINE_ENV_APP_HPP

#include <cstdio> // for printing to stdout

#include "Gamma/Analysis.h"
#include "Gamma/Effects.h"
#include "Gamma/Envelope.h"
#include "Gamma/Oscillator.h"

#include "al/app/al_App.hpp"
#include "al/scene/al_PolySynth.hpp"
#include "al/scene/al_SynthSequencer.hpp"
#include "al/ui/al_ControlGUI.hpp"
#include "al/ui/al_Parameter.hpp"

using namespace al;

#include "SineEnv.hpp"

// We make an app.
class SimplerSineEnvApp : public App
{
public:
  // GUI manager for SineEnv voices
  // The name provided determines the name of the directory
  // where the presets and sequences are stored
  SynthGUIManager<SineEnv> synthManager{"SineEnv"};

  // This function is called right after the window is created
  // It provides a graphics context to initialize ParameterGUI
  // It's also a good place to put things that should
  // happen once at startup.
  void onCreate() override;
  

  // The audio callback function. Called when audio hardware requires data
  void onSound(AudioIOData &io) override;
 

  void onAnimate(double dt) override;
 
 
  // The graphics callback function.
  void onDraw(Graphics &g) override;
 

  // Whenever a key is pressed, this function is called
  bool onKeyDown(Keyboard const &k) override;
  

  // Whenever a key is released this function is called
  bool onKeyUp(Keyboard const &k) override;

  void onExit() override; 
};


#endif