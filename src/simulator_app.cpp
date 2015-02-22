#include "allocore/io/al_App.hpp"
#include "Cuttlebone/Cuttlebone.hpp"
#include "alloutil/al_Simulator.hpp"
#include "alloutil/al_AlloSphereAudioSpatializer.hpp"
#include <Gamma/Noise.h>
#include <Gamma/Oscillator.h>

using namespace al;

#include <vector>    // vector
#include <iostream>  // cout
using namespace std;

// your state definition common to audio/simulation and graphics renderer
// programs.
//
#include "common.hpp"

// simulation parameters
//
#define SK (0.06f)  // spring constant for anchor points
#define NK (0.1f)   // spring constant between neighbors
#define D (0.08f)   // damping factor

struct Blob : App, AlloSphereAudioSpatializer, InterfaceServerClient {

  cuttlebone::Maker<State, 9000> maker;
  State* state;

  vector<vector<int>> nn;
  vector<Vec3f> velocity;
  vector<Vec3f> original;

  // a boolean value that is read and reset (false) by the simulation step and
  // written (true) by audio, keyboard and mouse callbacks.
  //
  bool shouldPoke;

  // a mesh we use to do graphics rendering in this app
  //
  Mesh mesh;

  // noise generator
  gam::NoisePink<> pinkNoise;
  gam::Sine<> sine;

  // sound source to represent a sound in space
  SoundSource tap;

  Blob() 
    : maker(Simulator::defaultBroadcastIP()),
      InterfaceServerClient(Simulator::defaultInterfaceServerIP()) {
    // create our state struct
    state = new State;
    memset(state, 0, sizeof(State));

    shouldPoke = true;

    SearchPaths searchPaths;
    searchPaths.addSearchPath(".", false);
    searchPaths.addSearchPath("/alloshare/blob", false);
    searchPaths.addAppPaths();

    // load a mesh from file
    mesh.primitive(Graphics::TRIANGLES);
    if (!load(searchPaths.find(ICOSPHERE_FILE).filepath(), mesh, nn)) {
      cout << "cannot find " << ICOSPHERE_FILE << endl;
      Main::get().stop();
    }

    // initialize simulator local state
    velocity.resize(mesh.vertices().size(), Vec3f(0, 0, 0));
    original.resize(mesh.vertices().size());
    for (int i = 0; i < mesh.vertices().size(); i++)
      original[i] = mesh.vertices()[i];

    sine.freq(440);

    // initialize lens parameters
    lens().near(0.1);
    lens().far(100);
    lens().focalLength(6);
    lens().eyeSepAuto();

    // initialize state to be broadcasted
    state->n = 0;
    for (int i = 0; i < N; i++) state->p[i] = original[i];
    state->audioGain = 0.97;
    state->backgroundColor = Color(0.1, 1);
    state->wireFrame = true;

    initWindow(Window::Dim(0, 0, 600, 400), "Blob Control Center", 60);

    // init audio and ambisonic spatialization
    AlloSphereAudioSpatializer::initAudio();
    AlloSphereAudioSpatializer::initSpatialization();
    gam::Sync::master().spu(AlloSphereAudioSpatializer::audioIO().fps());

    // add our sound source to the audio scene
    scene()->addSource(tap);

    // use this for smoother spatialization and dopler effect
    // good for fast moving sources or listener
    // computationally expensive!!
    scene()->usePerSampleProcessing(true);
    
    // set interface server nav/lens to App's nav/lens
    InterfaceServerClient::setNav(nav());
    InterfaceServerClient::setLens(lens());
   }

  virtual void poke() {
      shouldPoke = false;
      int n = rnd::uniform(N);
      state->pokedVertex = n;
      state->pokedVertexRest = original[n];
      Vec3f v = Vec3f(rnd::uniformS(), rnd::uniformS(), rnd::uniformS());
      for (unsigned k = 0; k < nn[n].size(); k++) state->p[nn[n][k]] += v * 0.5;
      state->p[n] += v;

      sine.freq(n*20. + 100.);

      // Illuminate the poke with this weird trick...
      state->lightPosition = (state->pokedVertexRest) * 2.3 + v * 4.567890;

      LOG("poke!");
  }


  virtual void onAnimate(double dt) {
    static cuttlebone::Stats fps("Blob::step");
    fps(dt);

    // handle messages from interface server
    while (InterfaceServerClient::oscRecv().recv()) {}

    // simulate
    if (shouldPoke) poke();

    for (int i = 0; i < N; i++) {
      Vec3f& v = state->p[i];
      Vec3f force = (v - original[i]) * -SK;

      for (int k = 0; k < nn[i].size(); k++) {
        Vec3f& n = state->p[nn[i][k]];
        force += (v - n) * -NK;
      }

      force -= velocity[i] * D;
      velocity[i] += force;
    }

    // update each point from simulation
    for (int i = 0; i < N; i++) state->p[i] += velocity[i];

    // update basic state parameters
    //
    state->t += dt;
    state->n++;
    state->pose = nav();
    state->nearClip = lens().near();
    state->farClip = lens().far();
    state->focalLength = lens().focalLength();
    state->eyeSeparation = lens().eyeSep();

    // set the new state (it'll get broadcast to each renderer)
    //
    maker.set(*state);

    // copy the vertex positions into the mesh
    //
    memcpy(&mesh.vertices()[0], &state->p[0], sizeof(Vec3f) * N);
  }

  virtual void onDraw(Graphics& g, const Viewpoint& v) {
    g.clearColor(state->backgroundColor);
    if (state->wireFrame)
      g.polygonMode(Graphics::LINE);
    else
      g.polygonMode(Graphics::FILL);
    g.draw(mesh);
  }

  virtual void onSound(AudioIOData& io) {
    static cuttlebone::Stats fps("onSound()");
    static float currentNoiseAmplitude = 0;

    fps(io.secondsPerBuffer());

    float maxInputAmplitude = 0.0f;
    
    // set the pose of our audio source
    tap.pose(Pose(state->p[state->pokedVertex], Quatf()));

    // "f" is the desired noise amplitude based on the state
    float f =
      (state->p[state->pokedVertex] - state->pokedVertexRest).mag() - 0.45;
    
    if (f > 0.99) f = 0.99;
    if (f < 0) f = 0;



    while (io()) {
      // find largest input amplitude of block
      //
      float in = fabs(io.in(0));
      if (in > maxInputAmplitude) maxInputAmplitude = in;

      // Make this 0.0001 so it will sound good
      float ProportionOfErrorToEliminateOnThisAudioSample = 0.0001;
      currentNoiseAmplitude += (f-currentNoiseAmplitude) * 
        ProportionOfErrorToEliminateOnThisAudioSample;

      // output sample directly or write sample to output through our audio source
      // io.out(0) = io.out(1) = pinkNoise() * f * state->audioGain;
      // tap.writeSample( pinkNoise() * currentNoiseAmplitude * state->audioGain );
      tap.writeSample( sine() * currentNoiseAmplitude * state->audioGain );
    }

    // poke the blob if the largest amplitude is above some threshold
    //
    if (maxInputAmplitude > 0.707f) shouldPoke = true;

    // set listener pose and render audio sources
    listener()->pose(state->pose);
    scene()->render(io);
  }

  virtual void onKeyDown(const ViewpointWindow& w, const Keyboard& k) {
    shouldPoke = true;

    if (k.key() == ' ') {
      state->wireFrame = !state->wireFrame;
      if (state->wireFrame) {
        LOG("wireframe on");
      } else {
        LOG("wireframe off");
      }
    }
  }

  virtual void onMouseDown(const ViewpointWindow& w, const Mouse& m) {
    shouldPoke = true;
  }
};

int main() {
  Blob blob;
  blob.AlloSphereAudioSpatializer::audioIO().start(); // start audio
  blob.InterfaceServerClient::connect(); // handshake with interface server
  blob.maker.start(); // it's important to call this.
  blob.start();
}
