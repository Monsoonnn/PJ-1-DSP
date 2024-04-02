#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstdlib>
using namespace std;

//DLF

vector<double> design_low_pass_filter(double frequency, double cutOffFrequency, int order) {
    double wc = tan(3.14 * frequency / cutOffFrequency);

    vector<double> low_pass_coeffs(order + 1);

    double theta = 3.14 * (2.0 * order - 1.0) / (2.0 * order);
    for (int i = 0; i <= order; ++i) {
        double angle = theta * i;
        double real = -1.0 * cos(angle);
        double imag = sin(angle);
        low_pass_coeffs[i] = 1.0 / (pow((real * real + imag * imag), 0.5));
    }

    for (int i = 0; i <= order; ++i) {
        low_pass_coeffs[i] *= pow(2.0 / wc, i);
    }
    double sum = 0.0;
    for (int i = 0; i <= order; ++i) {
        sum += low_pass_coeffs[i];
    }
    for (int i = 0; i <= order; ++i) {
        low_pass_coeffs[i] /= sum;
    }

    return low_pass_coeffs;
}

vector<pair<int, double>> apply_low_pass_filter(const vector<pair<int, double>>& data, double frequency, double cutOffFrequency, int order) {
    vector<double> coefficients = design_low_pass_filter(frequency, cutOffFrequency, order);

    vector<pair<int, double>> filtered_data;

    for (const pair<int, double>& sample : data) {
        int index = sample.first;
        double value = sample.second;

        double filtered_value = 0.0;
        for (size_t i = 0; i < coefficients.size(); ++i) {
            int delay_index = index - static_cast<int>(i);
            if (delay_index >= 0 && delay_index < data.size()) {
                filtered_value += coefficients[i] * data[delay_index].second;
            }
        }

        filtered_data.push_back({ index, filtered_value });
    }

    return filtered_data;
}


void writeToFile( const string& filename, vector<pair<int, double>>& waveform) {
  ofstream outfile(filename);
  for (auto& a : waveform) {
    outfile << a.first << " " << a.second << endl;
  }
  outfile.close();
}

void openGnuplot() {
    FILE *gnuplotPipe = popen("gnuplot -persist", "w");
    if (gnuplotPipe) {
        fprintf(gnuplotPipe, "set title 'Low Pass Filter Output'\n");
        fprintf(gnuplotPipe, "set xlabel 'Frequency(Hz)'\n");
        fprintf(gnuplotPipe, "set ylabel 'Amplitude(Db)'\n");
        fprintf(gnuplotPipe, "plot './LPF/LPF_output.txt' using 1:2 with lines title ''\n");
        fflush(gnuplotPipe);
    } else {
        std::cerr << "Error: Could not open gnuplot pipe" << std::endl;
    }
}

int main() {
    double frequency = 6.0;
    double cutOffFrequency = 0.5;
    int order = 4;
    double start_freq = 0.1;
    double end_freq = 100.0;
     int num_freq_steps = 100;
     int num_samples = 10;
     double bandwidth = 2.0;

    

  ofstream outfile("l_data.txt");
  if (!outfile.is_open()) {
    cerr << "Error opening file!" << endl;
    return 1;
  }
  for (double frequency = start_freq; frequency <= end_freq; frequency += (end_freq - start_freq) / (num_freq_steps - 1)) {
    vector<pair<int, double>> data;
    for (int i = 0; i < num_samples; ++i) {
      data.push_back({i, sin(2 * 3.14 * frequency * i)});
    }
	writeToFile("./LPF/LPF_input.txt", data);
    vector<pair<int, double>> filtered_data1 = apply_low_pass_filter(data,frequency,cutOffFrequency,order); 
    writeToFile("./LPF/LPF_output.txt",filtered_data1);

    double average_amplitude = 0.0;
    
    average_amplitude /= num_samples;
    outfile << frequency << " " << average_amplitude << std::endl;
  }
    outfile.close();

  openGnuplot();
    return 0;

}
