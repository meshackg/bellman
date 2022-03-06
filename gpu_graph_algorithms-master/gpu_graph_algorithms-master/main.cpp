#include "main.h"

int main(int argc, char **argv) {
    if (argc < 2 ){
        cout << "Usage : ./bellman MODE FILE BLOCK_SIZE DEBUG" << endl;
        cout << "MODE - seq / cuda \n"
                "FILE - Input file \n"
                "BLOCKS - Number of blocks per grid for cuda\n"
                "BLOCK_SIZE - Number of threads per block for cuda \n"
                "DEBUG - 1 or 0 to enable/disable extended debug messages on console\n"
                "Program expects these CSV files based on FILE thats passed in the argument\n"
                "    FILE_V.csv\n"
                "    FILE_I.csv\n"
                "    FILE_E.csv\n"
                "    FILE_W.csv"
                << endl;
        return -1;
    }
    std::string mode = argv[1];
    std::string file;
    if(argv[2] != NULL){
        file = argv[2];
        //Check if all CSR files are present
        if(!isValidFile(file + "_V.csv") ||
           !isValidFile(file + "_I.csv") ||
           !isValidFile(file + "_E.csv") ||
           !isValidFile(file + "_W.csv")){
            cout << "One or more CSR files missing" << endl;
            return -1;
        }

    }

    int BLOCKS = argc > 3 ? atoi(argv[3]) : 4;
    int BLOCK_SIZE = argc > 4 ? atoi(argv[4]) : 512;
    int debug = argc > 5 ? atoi(argv[5]) : 0;

    if(mode == "seq") {
        // Reference https://www.geeksforgeeks.org/measure-execution-time-function-cpp/
        auto start = high_resolution_clock::now();
        runBellmanFordSequential(file, debug);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);
        cout << "Elapsed time : " << duration.count() << " milli seconds " << endl;
    }
    if(mode == "cuda"){
        runBellmanFordOnGPU(file.c_str(), BLOCK_SIZE,debug);
    }
    if(mode == "cuda-stride"){
        runBellmanFordOnGPUWithGridStride(file.c_str(), BLOCKS, BLOCK_SIZE, debug);
    }
    if(mode == "cuda-v3") {
        runBellmanFordOnGPUV3(file.c_str(), BLOCKS, BLOCK_SIZE, debug);

    }

}