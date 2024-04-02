#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <cstdio>
#include <cmath>

using namespace std;

const double PI = 3.14159265358979323846;

// Function to apply a simple reverb effect using a feedback delay line
void applyReverb(vector<pair<int, double>> &waveform, int delaySamples, double decay)
{
    vector<double> delayLine(delaySamples, 0.0);

    for (size_t i = 0; i < waveform.size(); ++i)
    {
        double delayedValue = (i < delaySamples) ? 0.0 : delayLine[i % delaySamples];
        double inputValue = waveform[i].second;

        // Mixing the delayed value with the current one and applying the decay
        waveform[i].second = inputValue + delayedValue * decay;

        // Storing the mixed value for future feedback
        delayLine[i % delaySamples] = waveform[i].second;
    }
}

// Function to write the waveform to a file
void writeWaveformToFile(const vector<pair<int, double>> &waveform, const string &filename)
{
    ofstream outfile(filename);
    for (const auto &sample : waveform)
    {
        outfile << sample.first << " " << sample.second << endl;
    }
    outfile.close();
}

void drawPlot(const string &filename, const string &title)
{
    FILE *gnuplotPipe = popen("gnuplot -persist", "w");
    if (gnuplotPipe)
    {
        fprintf(gnuplotPipe, "set title '%s'n", title.c_str());
        fprintf(gnuplotPipe, "set xlabel 'TIME N'\n");
        fprintf(gnuplotPipe, "set ylabel 'x(N)'\n");
        fprintf(gnuplotPipe, "plot '%s' with lines title '%s'\n", filename.c_str(), title.c_str());
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

    // Add sample data to the waveform
    for (int i = 0; i <= 1000; ++i)
    {
        waveform.push_back({i, i < 200 ? 10.0 * sin(i / 20.0 * 2 * PI) : 0.0});
    }

    // Reverb parameters
    int delaySamples = 50; // The delay in samples
    double decay = 0.5;    // The decay rate of the reverb

    // Apply reverb to the waveform
    applyReverb(waveform, delaySamples, decay);

    // Write the waveform data after the reverb effect to a file
    writeWaveformToFile(waveform, "output_waveform_reverb.txt");

    // Plot the waveform with reverb effect
    drawPlot("output_waveform_reverb.txt", "Reverb Effect");

    return 0;
}
