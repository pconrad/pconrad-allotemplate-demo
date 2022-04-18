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

#include "SineEnv.hpp"

using namespace al;

void SineEnv::init() 
{
  // Intialize envelope
  mAmpEnv.curve(0); // make segments lines
  mAmpEnv.levels(0, 1, 1, 0);
  mAmpEnv.sustainPoint(2); // Make point 2 sustain until a release is issued

  // This is a quick way to create parameters for the voice. Trigger
  // parameters are meant to be set only when the voice starts, i.e. they
  // are expected to be constant within a voice instance. (You can actually
  // change them while you are prototyping, but their changes will only be
  // stored and aplied when a note is triggered.)

  createInternalTriggerParameter("amplitude", 0.1, 0.0, 1.0);
  createInternalTriggerParameter("frequency", 60, 20, 5000);
  createInternalTriggerParameter("attackTime", 0.1, 0.01, 3.0);
  createInternalTriggerParameter("decayTime", 0.1, 0.1, 10.0);
  createInternalTriggerParameter("releaseTime", 0.1, 0.1, 10.0);
  createInternalTriggerParameter("pan", 0.0, -1.0, 1.0);
}

// The audio processing function
void SineEnv::onProcess(AudioIOData &io) 
{
  // Get the values from the parameters and apply them to the corresponding
  // unit generators. You could place these lines in the onTrigger() function,
  // but placing them here allows for realtime prototyping on a running
  // voice, rather than having to trigger a new voice to hear the changes.
  // Parameters will update values once per audio callback because they
  // are outside the sample processing loop.
  mOsc.freq(getInternalParameterValue("frequency"));
  mAmpEnv.lengths()[0] = getInternalParameterValue("attackTime");
  mAmpEnv.lengths()[1] = getInternalParameterValue("decayTime");
  mAmpEnv.lengths()[2] = getInternalParameterValue("releaseTime");
  mPan.pos(getInternalParameterValue("pan"));
  while (io())
  {
    float s1 = mOsc() * mAmpEnv() * getInternalParameterValue("amplitude");
    float s2;
    mPan(s1, s1, s2);
    io.out(0) += s1;
    io.out(1) += s2;
  }
  // We need to let the synth know that this voice is done
  // by calling the free(). This takes the voice out of the
  // rendering chain
  if (mAmpEnv.done())
    free();
}

// The graphics processing function
void SineEnv::onProcess(Graphics &g) 
{
  // empty if there are no graphics to draw
}

// The triggering functions just need to tell the envelope to start or release
// The audio processing function checks when the envelope is done to remove
// the voice from the processing chain.
void SineEnv::onTriggerOn()  { mAmpEnv.reset(); }

void SineEnv::onTriggerOff() { mAmpEnv.release(); }
