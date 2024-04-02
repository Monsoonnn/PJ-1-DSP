#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <cstdio>
#include <cmath>

using namespace std;

const double PI = 3.14159265358979323846;

// Reverb
void applyReverb(vector<pair<int, double>> &waveform, int delaySamples, double decay)
{
    vector<double> delayLine(delaySamples, 0.0);

    for (size_t i = 0; i < waveform.size(); ++i)
    {
        double delayedValue = (i < delaySamples) ? 0.0 : delayLine[i % delaySamples];
        double inputValue = waveform[i].second;

        // Mix gia tri delayed va input trong thoi gian tren 1 giay
        waveform[i].second = inputValue + delayedValue * decay;

        // Luu gia tri mixed
        delayLine[i % delaySamples] = waveform[i].second;
    }
}

// Viet ra file
void writeWaveformToFile(const vector<pair<int, double>> &waveform)
{
    ofstream outfile("output_waveform_reverb.txt");
    for (const auto &sample : waveform)
    {
        outfile << sample.first << " " << sample.second << endl;
    }
    outfile.close();
}

// Ve bieu do tren mien thoi gian cua tin hieu.
void drawPlotReverb()
{
    FILE *gnuplotPipe = popen("gnuplot -persist", "w");
    if (gnuplotPipe)
    {
        fprintf(gnuplotPipe, "set title 'Reverb'\n");
        fprintf(gnuplotPipe, "set xlabel 'THOI GIAN N'\n");
        fprintf(gnuplotPipe, "set ylabel 'x(N)'\n");
        fprintf(gnuplotPipe, "plot 'output_waveform_reverb.txt' with lines title 'Reverb Effect'\n");
        fflush(gnuplotPipe);
        pclose(gnuplotPipe);
    }
    else
    {
        cerr << "Error: Could not open gnuplot pipe" << endl;
    }
}

int main()
{
    vector<pair<int, double>> waveform;

    // Them data mau vao waveform
    for (int i = 0; i <= 1000; ++i)
    {
        waveform.push_back({i, i < 200 ? 10.0 * sin(i / 20.0 * 2 * PI) : 0.0});
    }

    int delaySamples = 50; // do tre
    double decay = 0.5;    // Ti le Decay cua Reverb

    // Ap dung reverb vao waveform
    applyReverb(waveform, delaySamples, decay);

    // Viet du lieu waveform cua hieu ung reverb ra file
    writeWaveformToFile(waveform);
    drawPlotReverb();

    return 0;
}
