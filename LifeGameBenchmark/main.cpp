#include "../LifeGame/Universe.h"

#include <string>
#include <iostream>
#include <fstream>
#include <random>
#include <sstream>
#include <boost/filesystem.hpp>
#include <chrono>

using namespace std;

const string PREFIX("LifeGame_");
const string EXT("dat");

void usage() {
  cout << "LifeGameBenchmark.exe <mode> ..." << endl
       << "\t --generate, -g: Generation mode" << endl
       << "\t\t Path to save test cases" << endl
       << "\t\t N cases" << endl
       << endl
       << "\t --test, -t: Test mode" << endl
       << "\t\t Path to load test cases" << endl
       << "\t\t N generations"
       << endl;
}

string testdata(const string& path, int i) {
  stringstream sstr;
  sstr << path << "/" << PREFIX << i << "." << EXT;
  return sstr.str();
}

void generate(const string& path, int ncases) {
  if (!boost::filesystem::exists(path))
    boost::filesystem::create_directories(path);

  auto engine = mt19937{ random_device()() };
  auto rand = [=](int range) mutable {
    auto idist = uniform_int_distribution<int>(0, range);
    return idist(engine);
  };

  for (int i = 0; i < ncases; ++i) {
    auto file = testdata(path, i);
    if (boost::filesystem::exists(file)) {
      cout << "Error: test data file " << file << " already exists!" << endl;
      exit(1);
    }
    ofstream ofs(file);
    ofs << Universe::bigBang(1000, 1000, 0.3, rand);
    cout << ".";
  }

  cout << endl << "Test data saved to: " << path << endl;
}

void run(Universe u, int n) {
  Universe v;
  for (int i = 0; i < n; ++i) {
    u.nextGeneration(v);
    u.swap(v);
  }
}

chrono::steady_clock::duration timeit(function<void()> f) {
  auto begin = chrono::steady_clock::now();
  f();
  auto now = chrono::steady_clock::now();
  return now - begin;
}

string report_path(const string& path) {
  auto report = path + "/Benchmark.txt";
  int i = 0;
  while (boost::filesystem::exists(report)) {
    stringstream sstr;
    sstr << path << "/Benchmark_" << i++ << ".txt";
    report = sstr.str();
  }
  return report;
}

void test(const string& path, int ngen) {
  int idata = 0;

  auto report = report_path(path);
  ofstream ofs(report);
  ofs << "Run each universe of " << ngen << " generations." << endl
      << "idata,tick,milliseconds" << endl;

  while (true) {
    if (!boost::filesystem::exists(testdata(path, idata)))
      break;
    Universe u;
    ifstream ifs(testdata(path, idata));
    ifs >> u;
    auto duration = timeit([&]() { run(u, ngen); });
    cout << testdata(path, idata) << "\t"
         << chrono::duration_cast<chrono::milliseconds>(duration).count()
         << "ms" << endl;
    ofs << idata << "," << duration.count() << ","
        << chrono::duration_cast<chrono::milliseconds>(duration).count()
        << endl;
    ++idata;
  }
  cout << endl << "Report saved to: " << report << endl;
}

int main(int argc, char** argv) {
  if (argc != 4) {
    usage();
    return 0;
  }

  if (argv[1] == string("--generate") || argv[1] == string("-g")) {
    int ncases = atoi(argv[3]);
    generate(argv[2], ncases);
  } else if (argv[1] == string("--test") || argv[1] == string("-t")) {
    int ngen = atoi(argv[3]);
    test(argv[2], ngen);
  } else {
    usage();
  }

  return 0;
}
