#include "worker.h"
#include "presets.h"
#include <future>
#include <iostream>
#include <thread>

using namespace std;
namespace fs = std::filesystem;

static void process_one(const WorkerOptions &opt, const string &input) {
      static std::mutex cout_m;
      static std::mutex cerr_m;

      try {
            fs::path p(input);
            string inputName = p.stem().string();
            string extension = p.extension().string();
            if (!extension.empty() && extension.front() == '.')
                  extension.erase(0, 1);

            string presetCmd = searchPreset(opt.presetName, opt.exeDir);
            string finalCmd =
                applyVars(presetCmd, inputName, extension, opt.outDir);

            for (const auto &a : opt.extraArgs)
                  finalCmd += " " + a;

            string fullCommand =
                "\"" + opt.ffmpegPath.string() + "\" " + finalCmd;

            {
                  std::lock_guard<std::mutex> lk(cout_m);
                  cout << "Executando: " << fullCommand << "\n";
            }

            int rc = system(fullCommand.c_str());
            if (rc != 0) {
                  std::lock_guard<std::mutex> lk(cerr_m);
                  cerr << "Comando retornou código " << rc << " para arquivo "
                       << input << "\n";
            }
      } catch (const std::exception &e) {
            std::lock_guard<std::mutex> lk(cerr_m);
            cerr << "Erro processando " << input << ": " << e.what() << "\n";
      }
}

void run_workers(const WorkerOptions &opt) {
      size_t concurrency = std::thread::hardware_concurrency();
      if (concurrency == 0)
            concurrency = 2;
      size_t maxThreads = std::min(concurrency, opt.inputs.size());

      vector<future<void>> futures;
      futures.reserve(maxThreads + 1);

      for (const auto &in : opt.inputs) {
            while (futures.size() >= maxThreads) {
                  // wait for first
                  futures.front().get();
                  futures.erase(futures.begin());
            }

            futures.push_back(std::async(
                std::launch::async, [opt, in]() { process_one(opt, in); }));
      }

      // wait remaining
      for (auto &f : futures)
            f.get();
}