#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <complex>

using namespace std;

const double PI = 3.14159265358979323846;

// Tính phản hồi biên độ của Band Stop Filter
vector<double> BSFMagnitudeResponse(int sampleRate, double centerFreq, double bandwidth, int numPoints)
{
    vector<double> magnitudeResponse(numPoints);
    double dt = 1.0 / sampleRate;

    for (int i = 0; i < numPoints; ++i)
    {
        double freq = (double)i / numPoints * sampleRate / 2; // Tần số Nyquist
        double omega = 2 * PI * freq;
        double magnitude = 1.0 - exp(-0.5 * pow((freq - centerFreq) / (bandwidth / 2.0), 2)); // Phản hồi biên độ của Band Stop Filter
        magnitudeResponse[i] = magnitude;
    }

    return magnitudeResponse;
}

// Tính phản hồi pha của Band Stop Filter
vector<double> BSFPhaseResponse(int sampleRate, double centerFreq, double bandwidth, int numPoints)
{
    vector<double> phaseResponse(numPoints);
    double dt = 1.0 / sampleRate;

    for (int i = 0; i < numPoints; ++i)
    {
        double freq = (double)i / numPoints * sampleRate / 2;                        // Tần số Nyquist
        double phase = atan(-2 * PI * (freq - centerFreq) / bandwidth) * (180 / PI); // Phản hồi pha của Band Stop Filter (đơn vị là độ)
        phaseResponse[i] = phase;
    }

    return phaseResponse;
}

void writeFileBSF(const vector<double> &data, const string &filename)
{
    ofstream outfile(filename);
    for (size_t i = 0; i < data.size(); ++i)
    {
        outfile << i << "\t" << data[i] << "\n";
    }
    outfile.close();
}

void drawBSFPlot(const string &magnitudeFile, const string &phaseFile)
{
    FILE *gnuplotPipe = popen("gnuplot -persist", "w");
    if (gnuplotPipe)
    {
        fprintf(gnuplotPipe, "set multiplot layout 2,1 title 'Band Stop Filter'\n");
        fprintf(gnuplotPipe, "set xlabel 'Tan so (Hz)'\n");
        fprintf(gnuplotPipe, "set ylabel 'Bien do'\n");
        fprintf(gnuplotPipe, "plot '%s' with lines title 'Bien do'\n", magnitudeFile.c_str());
        fprintf(gnuplotPipe, "set ylabel 'Pha (do)'\n");
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
    int sampleRate = 500; // Tần số lấy mẫu
     double centerFreq = 50; // Tần số trung tâm của dải chặn
     double bandwidth = 20;  // Băng thông của dải chặn
    int N = 20; // Số điểm
//    double centerFreq, bandwidth;
    // cout << "Nhap tan so lay mau: ";
    // cin >> sampleRate;
//    cout << "Nhap tan so trung tam: ";
//    cin >> centerFreq;
//    cout << "Nhap bang thong: ";
//    cin >> bandwidth;
    // cout << "Nhap so diem lay mau: ";
    // cin >> N;

    vector<double> bsfMagnitudeResponse = BSFMagnitudeResponse(sampleRate, centerFreq, bandwidth, N);
    writeFileBSF(bsfMagnitudeResponse, "BSF/bsf_magnitude_data.txt");

    vector<double> bsfPhaseResponse = BSFPhaseResponse(sampleRate, centerFreq, bandwidth, N);
    writeFileBSF(bsfPhaseResponse, "BSF/bsf_phase_data.txt");

    drawBSFPlot("BSF/bsf_magnitude_data.txt", "BSF/bsf_phase_data.txt");

    return 0;
}
