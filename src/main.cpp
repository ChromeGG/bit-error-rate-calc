#include <iostream>

struct berResults {
    float ber;
    double tot;
    double err;
    clock_t startTime;
    clock_t endTime;
};

uint8_t hammingDistance(uint8_t n1, uint8_t n2);
void createFile1(const std::string name, const int count, const char value);
berResults calculateBer(std::string fpath1, std::string fpath2);
void printResult(berResults results);

int main(int argc, char *argv[]) {
    std::string fpath1; // path to file 1
    std::string fpath2; // path to file 2
    berResults results;

    createLogger("log.log");
    fpath1 = argv[1];
    fpath2 = argv[2];

    persisLog("Processing test files");
    results = calculateBer(fpath1, fpath2);
    printResult(results)

    closeLog();
    return 0;
}

uint8_t hammingDistance(uint8_t n1, uint8_t n2) {
    uint8_t x = n1 ^ n2; // XOR
    uint8_t setBits = 0;
    while (x > 0) {
        setBits += x & 1;
        x >>= 1;
    }
    return setBits;
}

void createFile1(const std::string name, const int count, const char value) {
    std::fstream f;
    f.open(name.c_str(), std::ios::binary | std::ios::out);
    for (int i = 0; i < count; i++) {
        f.write((char *)&value, 1);
    }
    f.close();
}

berResults calculateBer(std::string fpath1, std::string fpath2) {
    std::fstream f1, f2; // handlers to files
    berResults results;
    results.startTime = 0;
    results.endTime = 0;
    results.ber = 0;
    results.err = 0;
    results.tot = 0;

    persisLog("Calculating BER...");
    f1.open(fpath1.c_str(), std::ios::binary | std::ios::in);
    f2.open(fpath2.c_str(), std::ios::binary | std::ios::in);
    char a = 0x00;
    char b = 0x00;
    results.startTime = clock();

    while (!f1.eof()) {
        f1 >> a;       // read 1 char from file 1
        f2 >> b;       // read 1 char from file 2
        if (!f1.eof()) // till the end of the 1st file
        {
            results.err += hammingDistance(a, b); // add to the .err the number of different bits
            results.tot += 8;                     // add to the .tot the number of compared bits
        }
    }

    results.ber = (float)results.err / results.tot; // calculate ber
    results.endTime = clock();
    persisLog("BER calculations are done");
    return results; // return structure with all results
}

void printResult(berResults results) {
    std::stringstream message;
    message << "Results are: " << std::endl;
    message << "BER: " << results.ber << std::endl;
    message << "Tot: " << results.tot << std::endl;
    message << "Err: " << results.err << std::endl;
    message << "Calc time: " << ((float)results.endTime - results.startTime) / CLOCKS_PER_SEC << " sec " << std::endl;
    persisLog(message.str());
}