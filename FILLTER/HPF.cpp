#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <complex>

using namespace std;

const double PI = 3.14159265358979323846;

// Tính phản hồi pha của HPF
vector<double> HPFPhaseResponse(int sampleRate, double cutoffFreq, int numPoints)
{
    vector<double> phaseResponse(numPoints);
    double rc = 1.0 / (cutoffFreq * 2 * PI);
    double dt = 1.0 / sampleRate;

    for (int i = 0; i < numPoints; ++i)
    {
        double freq = (double)i / numPoints * sampleRate / 2; // tan so Nyquist
        double omega = 2 * PI * freq;
        double phase = (freq > cutoffFreq) ? 1 : 0.0; // Điều kiện HPF
        phaseResponse[i] = phase;
    }

    return phaseResponse;
}

// Viết dữ liệu phản hồi pha của HPF ra file
void writeFileHPFPhase(const vector<double> &phaseResponse, const string &filename)
{
    ofstream outfile(filename);
    for (size_t i = 0; i < phaseResponse.size(); ++i)
    {
        outfile << i << "\t" << phaseResponse[i] << "\n";
    }
    outfile.close();
}

// Vẽ Bode Plot cho HPF
void drawHPFPlot(const string &magnitudeFile, const string &phaseFile)
{
    FILE *gnuplotPipe = popen("gnuplot -persist", "w");
    if (gnuplotPipe)
    {
        fprintf(gnuplotPipe, "set multiplot layout 2,1 title 'HPF'\n");
        fprintf(gnuplotPipe, "set xlabel 'Truc tan so (Hz)'\n");
        fprintf(gnuplotPipe, "set ylabel 'Bien do'\n");
        fprintf(gnuplotPipe, "plot '%s' with lines title 'Bien do'\n", magnitudeFile.c_str());
        fprintf(gnuplotPipe, "set ylabel 'Pha'\n");
        fprintf(gnuplotPipe, "plot '%s' with lines title 'Pha'\n", phaseFile.c_str());
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
    int sampleRate = 500;   // Tan so lay mau
    double cutoffFreq = 50; // Tan so cat
    int N = 20;             // So diem

    // Tính và viết dữ liệu phản hồi pha của HPF ra file
    vector<double> hpfPhaseResponse = HPFPhaseResponse(sampleRate, cutoffFreq, N);
    writeFileHPFPhase(hpfPhaseResponse, "HPF/hpf_phase_data.txt");

    // Vẽ Bode Plot
    drawHPFPlot("HPF/hpf_data.txt", "HPF/hpf_phase_data.txt");

    return 0;
}
