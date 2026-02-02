#include "src/flags.h"
#include "src/utils.h"
#include "src/worker.h"
#include <filesystem>
#include <iostream>

using namespace std;
namespace fs = std::filesystem;

int main(int argc, char *argv[]) {
      try {
            if (argc < 2) {
                  cout << "Uso: meuapp [-out <dir>] [-g "
                          "\"[\"a.mp4\",\"b.mov\"]\"] [-a <dir>] <arquivo?> "
                          "<preset> [-- args ffmpeg]\n";
                  return 1;
            }

            // exe dir
            fs::path exePath = fs::canonical(argv[0]).parent_path();
            fs::path ffmpegPath = exePath / "ffmpeg" / "ffmpeg";
            util::ensure_executable(ffmpegPath);

            Options opt = parseFlags(argc, argv);

            if (opt.presetName.empty())
                  throw runtime_error("Preset não informado");

            if (opt.inputs.empty())
                  throw runtime_error("Nenhum arquivo de entrada informado");

            WorkerOptions wopt;
            wopt.exeDir = exePath;
            wopt.ffmpegPath = ffmpegPath;
            wopt.inputs = opt.inputs;
            wopt.presetName = opt.presetName;
            wopt.outDir = opt.outDir;
            wopt.extraArgs = opt.extraArgs;

            run_workers(wopt);

      } catch (const exception &e) {
            cerr << "Erro: " << e.what() << '\n';
            return 1;
      }

      return 0;
}