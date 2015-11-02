#include "allocore/math/al_Vec.hpp"
#include "allocore/graphics/al_Mesh.hpp"
#include "allocore/spatial/al_Pose.hpp"
#include "allocore/io/al_File.hpp"
#include "allocore/math/al_Random.hpp"
#include "Cuttlebone/Cuttlebone.hpp"
#include "alloutil/al_Simulator.hpp"
#include <vector>    // vector
#include <iostream>  // cout
using namespace al;
using namespace std;

// your state definition and any code that audio, simulation, and graphics uses.
//
#include "common.hpp"

// simulation parameters
//
#define P (200)     // how often to "poke" the blob
#define SK (0.06f)  // spring constant for anchor points
#define NK (0.1f)   // spring constant between neighbors
#define D (0.08f)   // damping factor

struct Blob : Simulator {

  cuttlebone::Maker<State, 9000> maker;
  State* state;

  vector<vector<int>> nn;
  vector<Vec3f> velocity;
  vector<Vec3f> original;

  Blob() : Simulator(defaultInterfaceServerIP()), maker(defaultBroadcastIP()) {
 
    state = new State;

    SearchPaths searchPaths;
    searchPaths.addSearchPath(".", false);
    searchPaths.addSearchPath("/alloshare/blob", false);
    searchPaths.addAppPaths();

    Mesh mesh;
    if (!load(searchPaths.find(ICOSPHERE_FILE).filepath(), mesh, nn)) {
      cout << "cannot find " << ICOSPHERE_FILE << endl;
      stop();
    }

    velocity.resize(mesh.vertices().size(), Vec3f(0, 0, 0));
    original.resize(mesh.vertices().size());
    for (int i = 0; i < mesh.vertices().size(); i++)
      original[i] = mesh.vertices()[i];
  }

  virtual ~Blob() {}

  virtual void init() {
    state->n = 0;
    for (int i = 0; i < N; i++) state->p[i] = original[i];
    state->audioGain = 0.1;
    state->backgroundColor = Color(0.1, 1);
    state->wireFrame = true;
  }

  virtual void poke() {
      int n = rnd::uniform(N);
      state->pokedVertex = n;
      state->pokedVertexRest = original[n];
      Vec3f v = Vec3f(rnd::uniformS(), rnd::uniformS(), rnd::uniformS());
      for (unsigned k = 0; k < nn[n].size(); k++) state->p[nn[n][k]] += v * 0.5;
      state->p[n] += v;

      // Illuminate the poke with this weird trick...
      state->lightPosition = (state->pokedVertexRest) * 2.3 + v * 4.567890;
      LOG("poke!");
  }


  virtual void step(double dt) {

    static cuttlebone::Stats fps("Blob::step");
    fps(dt);

    // i sense a greate disturbance in the ...
    if ((state->n % P) == 0) {
      poke();
    }

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
  }

  virtual void exit() { cout << "goodbye!" << endl; }

  // virtual void onMessage(osc::Message& m) {
  //   m.print();
  //   Simulator::onMessage(m);
  // }

  virtual void start() {
    maker.start();       // non-blocking
    Simulator::start();  // blocks
  }
};

int main() { Blob().start(); }
