
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstdlib>

using namespace std;

// cau hinh cac chi so
int startIndex = 0;
int endIndex = 12;
const int length = 50;
int startIndexF = 0;

vector<double> generateSquareWave( double amplitude, int period) {
        vector<double> waveform(abs(endIndex - startIndex) + 1, 0); 

        for (int i = startIndex; i < endIndex; ++i) {
	        if (abs(i) % period < period / 2) {
	            waveform[i] = amplitude;
	        } else {
	            waveform[i] = -amplitude;
	        }
    	}

        return waveform;
    }

vector<double> generateUnitImpulseFunction() {
    vector<double> impulse(abs(endIndex - startIndex) + 1,0);
    impulse[0] = 1; 
    return impulse;
}

void writeToFile(const string& filename,vector<double> waveform) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Not Found " << filename << endl;
        return;
    }
	if(startIndexF == 1){
		for (int i = endIndex; i >= startIndex; --i) {
            file << -i << " "<< waveform[waveform.size() - i - 1]<<endl;
        }
	}else{
		for (int i = startIndex; i <= endIndex; ++i) {
            file << i << " "<< waveform[i]<<endl;
        }
	}
    	

    file.close();
}

vector<double> generateUnitStepFunction() {
    vector<double> step(abs(endIndex - startIndex) + 1,0);
    for (int i = startIndex; i <= endIndex; ++i) {
        if (i >= 0) {
            step[i] = 1;
        }
    }
    return step;
}

vector<double> timeShift(vector<double> signal, int shift) {
    vector<double> shiftedSignal(signal.size(), 0); 
	shift = -shift;
    if (shift >= 0) { 
        for (int i = startIndex; i <= endIndex - shift; ++i) {
            shiftedSignal[i + shift] = signal[i]; 
        }

    } else { 
        for (int i = startIndex - shift; i <= endIndex; ++i) {
            shiftedSignal[i + shift] = signal[i]; 
        }
    }
	
    return shiftedSignal;
}

vector<double> timeReversal(vector<double> signal) {
  
    
   	reverse(signal.begin(), signal.end());
   	
	startIndexF = 1;
	
    return signal;
}

vector<double> downsample(vector<double> signal, int M) {
	int newSize = signal.size() / M;
	cout << newSize;
    vector<double> downsampledSignal(newSize,0);


    for (int i = 0; i < downsampledSignal.size() ; i++) {
        downsampledSignal[i] = signal[i + M];
        cout << signal[i+M] << endl;
    }
    endIndex = endIndex / M -1 ;
    return downsampledSignal;
}

vector<double> upsample(vector<double>& signal, int L) {
    vector<double> upsampledSignal;
    
    upsampledSignal.reserve(signal.size() * L);

    for (int i = 0; i < signal.size(); ++i) {
        upsampledSignal.push_back(signal[i]); 

       
        for (int j = 1; j < L; ++j) {
            upsampledSignal.push_back(0); 
        }
    }

	endIndex = endIndex * L;
    return upsampledSignal;
}
int main() {
    double amplitude = 1.0; 
    int halfPeriod = 6; 
    double frequency = 1.0;
    string filename = "output.txt";
    vector<double> finalSignal;
	int shiftAmount = -2; // Gia tri dich thoi gian // >0 delayed <0 advanced

// 	vector<double> waveform = generateSquareWave(amplitude, halfPeriod);// sinh day chu nhat

//	vector<double> waveform = generateUnitImpulseFunction(); // sinh day xung don vi

	vector<double> waveform = generateUnitStepFunction();//ham sinh day nhay don vi
	
//	vector <double> waveform = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	
//	finalSignal = timeShift(waveform, shiftAmount);//phep dich thoi gian
	
//	finalSignal = timeReversal(finalSignal);//phep dao nguoc thoi gian
	
//	finalSignal = downsample(waveform, 2); // giam tan so lay mau
	
//	finalSignal = upsample(waveform, 2);
	
  	writeToFile(filename, waveform); // viet ra file
    cout << "Updated " << filename << endl;
    
    //Neu gap loi "'gnuplot' is not recognized as an internal or external command" 
    //Mo Control Panel trên Windows và di d?n System and Security > System > Advanced system settings.
	//Trong hop thoai System Properties, chon tab Advanced, và nhap vào nút Environment Variables.
	//Trong ph?n System Variables, tìm bi?n môi tru?ng có tên PATH và chon Edit.
	//Thêm duung dan den thu muc cài dat Gnuplot vào cuoi bien PATH (C:\Program Files\gnuplot\bin).
	//Nhap OK de luu thay doi 
	
    // Dong lenh nay dung de hien plot sau khi chay ct ma khong can mo gnuplot
	string command = "gnuplot gnu.plt";
    system("gnuplot -persist gnu.plt");	
    
    return 0;
}

