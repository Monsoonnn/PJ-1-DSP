#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <complex>

using namespace std;

const double PI = 3.14159265358979323846;

vector<pair<int, double>> designBandPassFilter(const vector<pair<int, double>>& signal, double frequencyThreshold) {
    vector<pair<int, double>> filteredSignal;
    filteredSignal.reserve(signal.size());

    
    for (const auto& point : signal) {
        double y = 0.0;

      
        if (point.second > frequencyThreshold) {
          
            y = point.second;
        }

       
        filteredSignal.emplace_back(point.first, y);
    }

    return filteredSignal;
}

void plotSignals(const vector<pair<int, double>>& originalSignal, const vector<pair<int, double>>& filledSignal) {
    
    FILE *gnuplotPipe = popen("gnuplot -persist", "w");
    if (!gnuplotPipe) {
        cerr << "Error: Could not open gnuplot pipe" << endl;
        return;
    }

  
    fprintf(gnuplotPipe, "set title 'Original Signal and Filled Signal'\n");
    fprintf(gnuplotPipe, "set xlabel 'X'\n");
    fprintf(gnuplotPipe, "set ylabel 'Y'\n");
    fprintf(gnuplotPipe, "plot '-' with lines lw 2 title 'Original Signal', '-' with lines lw 2 title 'Filled Signal'\n");

    
    for (const auto& point : originalSignal) {
        fprintf(gnuplotPipe, "%d %lf\n", point.first, point.second);
    }
    fprintf(gnuplotPipe, "e\n");
 
    for (const auto& point : filledSignal) {
        fprintf(gnuplotPipe, "%d %lf\n", point.first, point.second);
    }
    fprintf(gnuplotPipe, "e\n");

    
    pclose(gnuplotPipe);

    
    ofstream originalFile("./BPF/original_BPF.txt");
    if (originalFile.is_open()) {
        for (const auto& point : originalSignal) {
            originalFile << point.first << " " << point.second << endl;
        }
        originalFile.close();
    } else {
        cerr << "Error: Could not open original_BPF.txt for writing" << endl;
    }

    ofstream filledFile("./BPF/filled_BPF.txt");
    if (filledFile.is_open()) {
        for (const auto& point : filledSignal) {
            filledFile << point.first << " " << point.second << endl;
        }
        filledFile.close();
    } else {
        cerr << "Error: Could not open filled.txt for writing" << endl;
    }
}
int main() {
    
    vector<pair<int, double>> signal = {
        {1, 0.0}, {2, 1.0}, {3, 3.0}, {4, 5.0}, {5, 6.0}, 
        {6, 6.0}, {7, 6.0}, {8, 6.0}, {9, 6.0}, {10, 6.0}, 
		{11, 6.0}, {12, 6.0},{13, 6.0}, {14, 6.0}, {15, 6.0},
		{16, 5.0}, {17, 4.0}, {18, 3.0},
        {19, 2.0}, {20, 1.0},{21, 0.0}
    };

    
    double frequencyThreshold = 5.0;
    vector<pair<int, double>> filteredSignal = designBandPassFilter(signal, frequencyThreshold);

    
    plotSignals(signal, filteredSignal);

    return 0;
}

