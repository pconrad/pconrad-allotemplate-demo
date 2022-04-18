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
#include "SimplerSineEnvApp.hpp"

// This example shows how to use SynthVoice and SynthManager to create an audio
// visual synthesizer. In a class that inherits from SynthVoice you will
// define the synth's voice parameters and the sound and graphic generation
// processes in the onProcess() functions.

// This function is called right after the window is created
// It provides a graphics context to initialize ParameterGUI
// It's also a good place to put things that should
// happen once at startup.
void SimplerSineEnvApp::onCreate()
{
  navControl().active(false); // Disable navigation via keyboard, since we
                              // will be using keyboard for note triggering

  // Set sampling rate for Gamma objects from app's audio
  gam::sampleRate(audioIO().framesPerSecond());

  imguiInit();

  // Play example sequence. Comment this line to start from scratch
  // synthManager.synthSequencer().playSequence("synth1.synthSequence");
  synthManager.synthRecorder().verbose(true);
}

// The audio callback function. Called when audio hardware requires data
void SimplerSineEnvApp::onSound(AudioIOData &io)
{
  synthManager.render(io); // Render audio
}

void SimplerSineEnvApp::onAnimate(double dt)
{
  // The GUI is prepared here
  imguiBeginFrame();
  // Draw a window that contains the synth control panel
  // synthManager.drawSynthControlPanel();
  imguiEndFrame();
}

// The graphics callback function.
void SimplerSineEnvApp::onDraw(Graphics &g) 
{
  g.clear();
  // Render the synth's graphics
  // synthManager.render(g);

  // GUI is drawn here
  imguiDraw();
}

// Whenever a key is pressed, this function is called
bool SimplerSineEnvApp::onKeyDown(Keyboard const &k) 
{
  // const float A4 = 432.f;
  const float A4 = 440.f;
  if (ParameterGUI::usingKeyboard())
  { // Ignore keys if GUI is using
    // keyboard
    return true;
  }
  if (k.shift())
  {
    // If shift pressed then keyboard sets preset
    int presetNumber = asciiToIndex(k.key());
    synthManager.recallPreset(presetNumber);
  }
  else
  {
    // Otherwise trigger note for polyphonic synth
    int midiNote = asciiToMIDI(k.key());
    if (midiNote > 0)
    {
      synthManager.voice()->setInternalParameterValue(
          "frequency", ::pow(2.f, (midiNote - 69.f) / 12.f) * A4);
      synthManager.triggerOn(midiNote);
    }
  }
  return true;
}

// Whenever a key is released this function is called
bool SimplerSineEnvApp::onKeyUp(Keyboard const &k) 
{
  int midiNote = asciiToMIDI(k.key());
  if (midiNote > 0)
  {
    synthManager.triggerOff(midiNote);
  }
  return true;
}

void SimplerSineEnvApp::onExit()  { imguiShutdown(); }
