#include "Cuttlebone/Cuttlebone.hpp"
#include "alloutil/al_OmniStereoGraphicsRenderer.hpp"
#include <vector>    // vector
#include <string>    // memcpy
#include <iostream>  // cout
using namespace al;
using namespace std;

#include "common.hpp"

struct Blob : OmniStereoGraphicsRenderer {
  cuttlebone::Taker<State, 9000> taker;
  State* state;
  Mesh mesh;
  Light light;

  Blob() {
    state = new State;

    SearchPaths searchPaths;
    searchPaths.addSearchPath(".", false);
    searchPaths.addSearchPath("/alloshare/blob", false);
    searchPaths.addAppPaths();

    vector<vector<int>> nn;
    if (!load(searchPaths.find(ICOSPHERE_FILE).filepath(), mesh, nn)) {
      cout << "cannot find " << ICOSPHERE_FILE << endl;
      Main::get().stop();
    }

    mesh.primitive(Graphics::TRIANGLES);
  }

  virtual ~Blob() {}

  virtual void onAnimate(double dt) {
    // static cuttlebone::Stats fps("onAnimate()");
    // fps(dt);
    int popCount = taker.get(*state);
    if (popCount > 0) {
      memcpy(&mesh.vertices()[0], &state->p[0], sizeof(Vec3f) * N);
      if (!state->wireFrame) mesh.generateNormals();
      pose = state->pose;
    }
  }

  virtual void onDraw(Graphics& g) {
    shader().uniform("lighting", 0.5);
    light.pos(state->lightPosition);
    light();

    omni().clearColor() = state->backgroundColor;
    omni().sphereRadius(state->focalLength);
    lens().near(state->nearClip);
    lens().far(state->farClip);
    lens().eyeSep(state->eyeSeparation);
    if (state->wireFrame)
      g.polygonMode(Graphics::LINE);
    else
      g.polygonMode(Graphics::FILL);
    g.color(1,1,1);
    g.draw(mesh);
  }

  virtual void start() {
    taker.start();                        // non-blocking
    OmniStereoGraphicsRenderer::start();  // blocks
  }
};

int main() { Blob().start(); }
