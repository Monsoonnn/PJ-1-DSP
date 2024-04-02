#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <complex>

using namespace std;

const double PI = 3.14159265358979323846;

// TINH DFT
vector<complex<double>> DFT(const vector<pair<int, double>>& waveform) {
    int N = waveform.size();
    vector<complex<double>> result(N);

    for (int k = 0; k < N; ++k) {
        complex<double> sum(0.0, 0.0);
        for (int n = 0; n < N; ++n) {
            double angle = -2.0 * PI * k * n / N;
            sum += complex<double>(waveform[n].second * cos(angle), waveform[n].second * sin(angle));
        }
        result[k] = sum;
    }

    return result;
}

// TINH IDFT
vector<pair<int, double>> IDFT(const vector<complex<double>>& spectrum) {
    double N = spectrum.size();
    vector<pair<int, double>> result(N);

    for (int n = 0; n < N; ++n) {
        complex<double> sum(0.0, 0.0);
        for (int k = 0; k < N; ++k) {
            double angle = 2.0 * PI * k * n / N;
            sum += spectrum[k] * complex<double>(cos(angle), sin(angle));
        }
        result[n] = {n, abs(sum) / N};
    }

    return result;
}
// TINH GOC CUA SO PHUC
double angle(complex<double> z) {
    return atan2(z.imag(), z.real());
}
// VE DFT
void drawDFTplot(){
	FILE *gnuplotPipe = popen("gnuplot -persist", "w");
    if (gnuplotPipe) {
        fprintf(gnuplotPipe, "set multiplot layout 2,1 title 'DFT / PHO BIEN DO / PHO PHA'\n");
        fprintf(gnuplotPipe, "set xlabel 'Truc tan so W'\n");
        fprintf(gnuplotPipe, "set ylabel '|X(W)|'\n");
        fprintf(gnuplotPipe, "plot './DFT_IDFT/dft_data.txt' with lines title ''\n");
        fprintf(gnuplotPipe, "set xlabel 'Truc tan so W'\n");
        fprintf(gnuplotPipe, "set ylabel 'Angle(X(w))'\n");
        fprintf(gnuplotPipe, "plot './DFT_IDFT/phase_data.txt' with lines title ''\n");
        fprintf(gnuplotPipe, "unset multiplot\n");
        fflush(gnuplotPipe);
        // Dong pipe sau khi ve xong
        pclose(gnuplotPipe);
    } else {
        cerr << "Error: Could not open gnuplot pipe" << endl;
    }
    
}

//Viet DFT data ra file
void writeFileDFT(vector<complex<double>> spectrum){
	ofstream dft_file("./DFT_IDFT/dft_data.txt");
    ofstream phase_file("./DFT_IDFT/phase_data.txt");
    for (size_t i = 0; i < spectrum.size(); ++i) {
        dft_file << i << "\t" << abs(spectrum[i]) << endl;
        phase_file << i << "\t" << angle(spectrum[i]) << endl;
    }
    dft_file.close();
    phase_file.close();
}

// VE IDFT
void drawIDFTplot(){
	FILE *gnuplotPipe = popen("gnuplot -persist", "w");
    if (gnuplotPipe) {
    	fprintf(gnuplotPipe, "set title 'IDFT / BIEU DIEN MIEN THOI GIAN'\n");
        fprintf(gnuplotPipe, "set xlabel 'TRUC THOI GIAN N'\n");
        fprintf(gnuplotPipe, "set ylabel 'x(N)'\n");
        fprintf(gnuplotPipe, "plot './DFT_IDFT/idft_data.txt' with lines title ''\n");
        fprintf(gnuplotPipe, "unset multiplot\n");
        fflush(gnuplotPipe);
        
        pclose(gnuplotPipe);
    } else {
        cerr << "Error: Could not open gnuplot pipe" << endl;
    }
    
}

//Viet IDFT data ra file
void writeFileIDFT(vector<pair<int, double>> reconstructed_waveform, string filename){
	ofstream idft_file(filename);
    for (size_t i = 0; i < reconstructed_waveform.size(); ++i) {
        idft_file << reconstructed_waveform[i].first << "\t" << reconstructed_waveform[i].second << endl;
    }
    idft_file.close();
}



int main() {
	
    vector<pair<int, double>> waveform = {
        {0, 1.0}, {1, 4.0}, {2, 9.0}, {3, 16.0}, {4, 25.0},
        {5, 36.0}, {6, 49.0}, {7, 64.0}, {8, 81.0}
    };
	writeFileIDFT(waveform,"./DFT_IDFT/DFT_input.txt");
	//DFT
    vector<complex<double>> spectrum = DFT(waveform);

	//Viet DFT data ra file
    writeFileDFT(spectrum);
	
	//IDFT
	vector<pair<int, double>> reconstructed_waveform = IDFT(spectrum);
	
	//Viet IDFT data ra file
	writeFileIDFT(reconstructed_waveform,"./DFT_IDFT/idft_data.txt");
    
    
    // GNUPLOT
    drawDFTplot();
    drawIDFTplot();
    
    
	cout << "Updated " << "DFT data and IDFT data" << endl;
    return 0;
}

