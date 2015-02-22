#include "Cuttlebone/Cuttlebone.hpp"
#include "alloutil/al_AudioRenderer.hpp"

#include <string>  // memset
#include <csound/csound.hpp>

using namespace al;
using namespace std;

#include "common.hpp"

struct Blob : AudioRenderer {
  cuttlebone::Taker<State, 9000> taker;
  State* state;

  SoundSource tap;

  Blob() {
    state = new State;
    memset(state, 0, sizeof(State));
    // initAudio();

    char csd[] = "granularReverb.csd";
    csound.Compile(csd);
    mAudioIO.print();

//    initAudio("HDA Intel PCH: ALC269VC Analog", csound.GetSr(), csound.GetKsmps(), csound.GetNchnls(), csound.GetNchnlsInput());
    printf("Sampling Rate: %f, Ksmps size: %i, Out channels: %i, In Channels: %i.\n",
    csound.GetSr(), csound.GetKsmps(), csound.GetNchnls(), csound.GetNchnlsInput());

    initSpatialization();
    scene()->addSource(tap);
  }

  virtual ~Blob() {csound.Cleanup();}

  virtual void onSound(AudioIOData& io) {
//    static cuttlebone::Stats fps("onSound()");
//    fps(io.secondsPerBuffer());

//    tap.pose(Pose(state->p[state->pokedVertex], Quatd(1, 0, 0, 0)));

//    int popCount = taker.get(*state);
//    float f = (state->p[state->pokedVertex] - state->pokedVertexRest).mag() - 0.304134 /* mean */;
//    if (f > 0.8) f = 0.8;
//    if (f < 0) f = 0;
//    while (io()) {
//      // XXX rnd::uniformS() should be gamma noise
//      // XXX use interpolation on gain
//      tap.writeSample(rnd::uniformS() * f);
//      // io.out(0) = rnd::uniformS() * f;
//    }
//    listener()->pose(state->pose);
//    scene()->render(io);


    csound.PerformKsmps();
    MYFLT *buffer = csound.GetSpout();
    // This is the sample loop
    while(io()){
        //float in = io.in(0);
        for (int i = 0; i < csound.GetNchnls(); i++) {
            io.out(i) = *buffer++;
        }
    }

  }

  virtual void start() {
    taker.start();           // non-blocking
    AudioRenderer::start();  // blocks
  }

  Csound csound;
};

int main() { Blob().start(); }
