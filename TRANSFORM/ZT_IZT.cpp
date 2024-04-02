#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <complex>

using namespace std;

const double PI = 3.14159265358979323846;

// Z-transform
vector<complex<double>> ZTransform(const vector<pair<int, double>>& waveform) {
    int N = waveform.size();
    vector<complex<double>> result(N);

    for (int k = 0; k < N; ++k) {
        complex<double> sum(0.0, 0.0);
        for (int n = 0; n < N; ++n) {
            double angle = -2.0 * PI * k * n / N;
            sum += waveform[n].second * exp(complex<double>(0.0, angle));
        }
        result[k] = sum;
    }

    return result;
}

// Inverse Z-transform
vector<pair<int, double>> InverseZTransform(const vector<complex<double>>& Z) {
    int N = Z.size();
    vector<pair<int, double>> result(N);

    for (int n = 0; n < N; ++n) {
        complex<double> sum(0.0, 0.0);
        for (int k = 0; k < N; ++k) {
            double angle = 2.0 * PI * k * n / N;
            sum += Z[k] * exp(complex<double>(0.0, angle));
        }
        result[n] = {n, abs(sum) / N};
    }

    return result;
}

// write ZT to file
void WriteZTToFile(const vector<complex<double>>& Z) {
    ofstream zt_file("./ZT_IZT/ZT_data.txt");
    for (size_t i = 0; i < Z.size(); ++i) {
        zt_file << Z[i].real() << "\t" << Z[i].imag() << endl;
    }
    zt_file.close();
}

// write IZT to file
void WriteIZTToFile(const vector<pair<int, double>>& IZ) {
    ofstream izt_file("./ZT_IZT/IZT_data.txt");
    for (size_t i = 0; i < IZ.size(); ++i) {
        izt_file << IZ[i].first << "\t" << IZ[i].second << endl;
    }
    izt_file.close();
}


// ve bieu do cho ZT 
void plotZT(string filename) {

    
    FILE *gnuplotPipe = popen("gnuplot -persist", "w");
    if (gnuplotPipe) {
        
        fprintf(gnuplotPipe, "set title 'ZT / BIEU DIEN TOA DO DIEM'\n");
        fprintf(gnuplotPipe, "set xlabel 'PHAN THUC'\n");
        fprintf(gnuplotPipe, "set ylabel 'PHAN AO'\n");
        fprintf(gnuplotPipe, "set xrange [-300:300]\n");
        fprintf(gnuplotPipe, "set yrange [-300:300]\n");
        fprintf(gnuplotPipe, "set grid\n");
        fprintf(gnuplotPipe, "set arrow 1 from 0,-300 to 0,300 nohead lt 8 lw 1\n");
		fprintf(gnuplotPipe, "set arrow 2 from -300,0 to 300,0 nohead lt 8 lw 1\n");
        fprintf(gnuplotPipe, "plot '%s' using 1:2 with points title 'SO PHUC\n", filename.c_str());
       
        fflush(gnuplotPipe);
        
        pclose(gnuplotPipe);
    } else {
        cerr << "Error: Could not open gnuplot pipe" << endl;
    }
}

// ve bieu do cho IZT 
void plotIZT(const string& filename) {
    FILE *gnuplotPipe = popen("gnuplot -persist", "w");
    if (gnuplotPipe) {
        fprintf(gnuplotPipe, "set title 'IZT / BIEU DIEN MIEN THOI GIAN'\n");
        fprintf(gnuplotPipe, "set xlabel 'TRUC THOI GIAN N'\n");
        fprintf(gnuplotPipe, "set ylabel 'x(N)'\n");
        fprintf(gnuplotPipe, "plot '%s' with lines title ''\n", filename.c_str());
        fflush(gnuplotPipe);
        pclose(gnuplotPipe);
    } else {
        cerr << "Error: Could not open gnuplot pipe" << endl;
    }
}


int main() {
	
	// KHOI TAO DU LIEU
    vector<pair<int, double>> waveform = {
        {0, 1.0}, {1, 4.0}, {2, 9.0}, {3, 16.0}, {4, 25.0},
        {5, 36.0}, {6, 49.0}, {7, 64.0}, {8, 81.0}
    };

    // TINH TOAN ZT
    vector<complex<double>> ZT = ZTransform(waveform);

    // GHI DU LIEU ZT VAO FILE
    WriteZTToFile(ZT);

    // TINH TOAN IZT
    vector<pair<int, double>> IZT = InverseZTransform(ZT);

    // GHI DU LIEU IZT VAO FILE
    WriteIZTToFile(IZT);

	//TAO DUONG DAN FILE .txt cho ZT va IZT
	string ZTfile = "./ZT_IZT/ZT_data.txt";
    string IZTfile = "./ZT_IZT/IZT_data.txt";
	
	//VE BIEU DO ZT VA IZT
    plotZT(ZTfile);
    plotIZT(IZTfile);
	
    cout << "Z-transform and Inverse Z-transform data written to files." << endl;

    return 0;
}
