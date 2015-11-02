#include "Cuttlebone/Cuttlebone.hpp"
#include "alloutil/al_AudioRenderer.hpp"
#include <string>  // memset
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
    initSpatialization();
    scene()->addSource(tap);
  }

  virtual ~Blob() {}

  virtual void onSound(AudioIOData& io) {
    static cuttlebone::Stats fps("onSound()");
    fps(io.secondsPerBuffer());

    tap.pose(Pose(state->p[state->pokedVertex], Quatd(1, 0, 0, 0)));

    int popCount = taker.get(*state);
    float f = (state->p[state->pokedVertex] - state->pokedVertexRest).mag() - 0.304134 /* mean */;
    if (f > 0.8) f = 0.8;
    if (f < 0) f = 0;
    while (io()) {
      // XXX rnd::uniformS() should be gamma noise
      // XXX use interpolation on gain
      tap.writeSample(rnd::uniformS() * f);
      // io.out(0) = rnd::uniformS() * f;
    }
    listener()->pose(state->pose);
    scene()->render(io);
  }

  virtual void start() {
    taker.start();           // non-blocking
    AudioRenderer::start();  // blocks
  }
};

int main() { Blob().start(); }
