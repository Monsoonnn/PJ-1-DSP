#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <cstdio>

using namespace std;

// Fade in
void applyFadeIn(vector<pair<int, double>>& waveform, double fadeInRatio) {
    int numSamples = waveform.size();
    int fadeInSamples = static_cast<int>(numSamples * fadeInRatio);
    
    for (int i = 0; i < fadeInSamples; ++i) {
        double volume = static_cast<double>(i) / fadeInSamples; // Linear increase of volume
        waveform[i].second *= volume;
    }
}

// Fade out
void applyFadeOut(vector<pair<int, double>>& waveform, double fadeOutRatio) {
    int numSamples = waveform.size();
    int fadeOutSamples = static_cast<int>(numSamples * fadeOutRatio);
    
    for (int i = numSamples - 1; i >= numSamples - fadeOutSamples; --i) {
        double volume = static_cast<double>(numSamples - i) / fadeOutSamples; // Linear decrease of volume
        waveform[i].second *= volume;
    }
}

// Viet ra file
void writeWaveformToFile(const vector<pair<int, double>>& waveform, const string& filename) {
    ofstream outfile(filename);
    for (const auto& sample : waveform) {
        outfile << sample.first << " " << sample.second << endl;
    }
    outfile.close();
}
//Ve bieu do tren mien thoi gian cua tin hieu.
void drawPlot(){
	FILE *gnuplotPipe = popen("gnuplot -persist", "w");
    if (gnuplotPipe) {
        fprintf(gnuplotPipe, "set multiplot layout 2,1 title 'Fade In / Fade Out'\n");
        fprintf(gnuplotPipe, "set xlabel 'THOI GIAN N'\n");
        fprintf(gnuplotPipe, "set ylabel 'x(N)'\n");
        fprintf(gnuplotPipe, "plot 'output_waveform_fade_in.txt' with lines title 'Fade In'\n");
        fprintf(gnuplotPipe, "set xlabel 'THOI GIAN N'\n");
        fprintf(gnuplotPipe, "set ylabel 'x(N)'\n");
        fprintf(gnuplotPipe, "plot 'output_waveform_fade_out.txt' with lines title 'Fade Out'\n");
        fprintf(gnuplotPipe, "unset multiplot\n");
        fflush(gnuplotPipe);
        pclose(gnuplotPipe);
    } else {
        cerr << "Error: Could not open gnuplot pipe" << endl;
    }
}
int main() {
    vector<pair<int, double>> waveform;
    
    // Thêm các c?p giá tr? (index, amplitude) vào vector
    for (int i = 0; i <= 20; ++i) {
        waveform.push_back({i, 10.0}); // Gi? s? amplitude c? d?nh là 10.0
    }
    
    double fadeInRatio = 0.2; // 20% fade in
    double fadeOutRatio = 0.2; // 20% fade out
    
    //Ap dung fade in va fade out vao
    vector<pair<int, double>> waveformFadeIn = waveform;
    applyFadeIn(waveformFadeIn, fadeInRatio);
    vector<pair<int, double>> waveformFadeOut = waveform;
    applyFadeOut(waveformFadeOut, fadeOutRatio);
    
    // Viet ra file du lieu fade in va fade out
    writeWaveformToFile(waveformFadeIn, "output_waveform_fade_in.txt");
    writeWaveformToFile(waveformFadeOut, "output_waveform_fade_out.txt");
    
    // GNUPLOT
    drawPlot();

    return 0;
}

