#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <cmath>

using namespace std;

const double PI = 3.14159265358979323846;

// Flange
void applyFlange(vector<pair<int, double>> &waveform, double flangeRate, int maxDelay)
{
    int numSamples = waveform.size();
    vector<double> lfo(numSamples, 0.0); // Low Frequency Oscillator

    for (int i = 0; i < numSamples; ++i)
    {
        double lfoValue = sin(2.0 * PI * flangeRate * i / numSamples);
        lfo[i] = (lfoValue + 1) / 2.0 * maxDelay; // Biến đổi giá trị LFO vào khoảng 0-maxDelay
    }

    for (int i = 0; i < numSamples; ++i)
    {
        int delaySamples = static_cast<int>(lfo[i]); // Độ trễ được xác định bởi LFO
        int delayedIndex = i - delaySamples;

        if (delayedIndex < 0)
            delayedIndex = 0; // Xử lý ranh giới

        double delayedValue = (delaySamples > 0) ? waveform[delayedIndex].second : 0.0;

        // Pha trộn mẫu gốc và mẫu bị trễ
        waveform[i].second = (waveform[i].second + delayedValue) / 2;
    }
}

// Viết ra file
void writeWaveformToFile(const vector<pair<int, double>> &waveform)
{
    ofstream outfile("output_waveform_flange.txt");
    for (const auto &sample : waveform)
    {
        outfile << sample.first << " " << sample.second << endl;
    }
    outfile.close();
}

// Vẽ biểu đồ trên miền thời gian của tín hiệu.
void drawPlotFlange()
{
    FILE *gnuplotPipe = popen("gnuplot -persist", "w");
    if (gnuplotPipe)
    {
        fprintf(gnuplotPipe, "set title 'Flange Effect'\n");
        fprintf(gnuplotPipe, "set xlabel 'Thoi gian N'\n");
        fprintf(gnuplotPipe, "set ylabel 'x(N)'\n");
        fprintf(gnuplotPipe, "plot 'output_waveform_flange.txt' with lines title 'Flange Effect'\n");
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

    // Khởi tạo waveform với tín hiệu sin
    for (int i = 0; i <= 50; ++i)
    {
        waveform.push_back({i, sin(i / 20.0 * 2 * PI)});
    }

    double flangeRate = 0.25; // Tốc độ dao động của Flanger LFO
    int maxDelay = 20;        // Độ trễ tối đa

    // Áp dụng hiệu ứng flange
    applyFlange(waveform, flangeRate, maxDelay);

    // Xuất waveform đã được xử lý ra file
    writeWaveformToFile(waveform);

    // Vẽ biểu đồ sử dụng gnuplot hoặc công cụ khác
    drawPlotFlange();

    return 0;
}