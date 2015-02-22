#ifndef __COMMON_BLOB__
#define __COMMON_BLOB__

#define PPCAT_NX(A, B) A##B
#define PPCAT(A, B) PPCAT_NX(A, B)
#define STRINGIZE_NX(A) #A
#define STRINGIZE(A) STRINGIZE_NX(A)

#define N 162
//#define N 642
//#define N 2562
//#define N 10242
//#define N 40962
//#define N 163842
//#define N 655362

#define ICOSPHERE_FILE STRINGIZE(PPCAT(N, .ico))

struct State {

  // these are the basics. most AlloSphere Apps will want these.
  //
  double t;    // simulation time
  unsigned n;  // "frame" number
  Pose pose;   // for navigation

  // this is how you might control renderering settings.
  //
  double eyeSeparation, nearClip, farClip, focalLength;
  double audioGain;
  Color backgroundColor;
  bool wireFrame;  // alternative is shaded

  Vec3f lightPosition;

  unsigned pokedVertex;
  Vec3f pokedVertexRest;

  // all of the above data could be distributed using unicast OSC to each
  // renderering host (it's only about 60 bytes), but this scheme would suffer
  // from tearing due to out-of-sync sending and receiving of packets. the win
  // above is that all this data appears at each renderering host
  // simultaneously because we're using UDP broadcast, which does not use a
  // foreach to send N identical messages to N renderering hosts.
  //
  // below is another, different win. here we have an array of vertices that
  // could get as big as the network will bear (~10MB). this data is calculated
  // on the server/simulator, so it does not need to be calculated on the
  // renderer, only interpreted.
  //

  Vec3f p[N];
};

bool load(string fileName, Mesh& mesh, vector<vector<int> >& nn) {
  ifstream file(fileName);
  if (!file.is_open()) return false;

  string line;
  int state = 0;
  while (getline(file, line)) {
    if (line == "|") {
      state++;
      continue;
    }
    switch (state) {
      case 0: {
        vector<float> v;
        stringstream ss(line);
        float f;
        while (ss >> f) {
          v.push_back(f);
          if (ss.peek() == ',') ss.ignore();
        }
        mesh.vertex(v[0], v[1], v[2]);
        // cout << v[0] << "|" << v[1] << "|" << v[2] << endl;
      } break;

      case 1: {
        stringstream ss(line);
        int i;
        if (ss >> i)
          mesh.index(i);
        else
          return false;
        // cout << i << endl;
      } break;

      case 2: {
        vector<int> v;
        stringstream ss(line);
        int i;
        while (ss >> i) {
          v.push_back(i);
          if (ss.peek() == ',') ss.ignore();
        }
        if ((v.size() != 5) && (v.size() != 6)) return false;
        nn.push_back(v);
        // cout << nn[nn.size() - 1].size() << endl;
      } break;
    }
  }
  file.close();

  return true;
}

#endif
