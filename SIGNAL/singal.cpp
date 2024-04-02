#include <iostream>
#include <algorithm>
#include <fstream>
#include <complex>
#include <vector>
#include <cstdio>
#include <cstdlib>


using namespace std;

// cau hinh cac chi so
int startIndex = 0;
int endIndex = 12;
int startIndexF = 0;
string filename = "output.txt";
constexpr double PI = 3.14159265358979323846;

struct Complex {
  double real;
  double imag;
};
// H�m nm?
vector<pair<int, Complex>> generateComplexExponential(double base) {
  vector<pair<int, Complex>> result;

  for (int x = startIndex; x <= endIndex; ++x) {
    double angle = x;  // Combined exponent for complex case
    Complex y;
    y.real = pow(base, cos(angle));
    y.imag = pow(base, sin(angle));
    // * (base < 0 ? -1 : 1); // Handle negative base for imaginary part
    result.push_back(make_pair(x, y));
  }

  return result;
}
// sinh day chu nhat
vector<pair<int, double>> generateSquareWave(double amplitude, int period) {
    vector<pair<int, double>> waveform;

    
    int length = abs(endIndex - startIndex) + 1;

  
    waveform.reserve(length);

  
    for (int i = startIndex; i <= endIndex; ++i) {
        double value = (abs(i) % period < period / 2) ? amplitude : -amplitude;
        waveform.emplace_back(i, value);
    }

    return waveform;
}

// day xung don vi
vector<pair<int, double>> generateUnitImpulseFunction() {
    
    int length = endIndex - startIndex + 1;

	
    vector<pair<int, double>> impulse;

  		for (int i = startIndex; i <= endIndex; ++i) {
        	if (i == 0) {
            impulse.push_back({i, 1.0});
        	}else{
        	impulse.push_back({i, 0});
			}
    }
  

    return impulse;
}

// day nhay don vi

vector<pair<int, double> > generateUnitStepFunction() {
    
    int length = endIndex - startIndex + 1;

    vector<pair<int, double>> step;

    for (int i = startIndex; i <= endIndex; ++i) {
        if (i >= 0) {
            step.push_back({i, 1.0});
        }else{
        	step.push_back({i, 0});
		}
    }

    return step;
}

// Phep dich thoi gian


vector<pair<int, double> > timeShift(const vector<pair<int, double>>& signal, int shift) {
   
    int length = endIndex - startIndex + 1;

    vector<pair<int, double>> shiftedSignal;
        
        for (const auto& pair : signal) {
            int newIndex = pair.first + shift - startIndex;
            shiftedSignal.push_back({newIndex,pair.second});

        }
    

    return shiftedSignal;
}

// sinh day doc

vector<pair<int, double> > generateUnitRampFunction()
{
    vector<pair<int, double>> ramp;
    for (int i = startIndex; i <= endIndex; ++i)
    {
        if (i >= 0)
        {
            ramp.push_back({i,i});
        }
    }
    return ramp;
}

//day mu~ thuc

vector<pair<int, double> > generateRealExponentialFunction(double base) {
    
    vector<pair<int, double>> RealExpWaveform; 
    
	for (int i = startIndex; i <= endIndex; ++i) {
         if(i >= 0){
	        	RealExpWaveform.push_back({i,pow(base, i)});  
			}else{
				RealExpWaveform.push_back({i,0});  
			}
    }
   

    return RealExpWaveform;
}

//sinh day hinh Sin

vector<pair<int, double> > generateSineWave(int numSamples, double frequency, double amplitude) {
  const double PI = 3.14; 

  vector<pair<int, double>> sineWave;
  
  for (int i = startIndex; i < endIndex; ++i) {
  
    double angle = 2.0 * PI * frequency * (static_cast<double>(i) / numSamples);

    sineWave.push_back({i,amplitude * sin(angle)});
  }

  return sineWave;
}

//sinh day ham mu~

vector<pair<int, double> > generateExponentialFunction(double base) {
    
    vector<pair<int, double>> exponentialValues; 
    base = abs(base);
    
	for (int i = startIndex; i <= endIndex; ++i) {
         if(i >= 0){
	        	exponentialValues.push_back({i,pow(base, i)});  
			}else{
				exponentialValues.push_back({i,0});  
			}
    }
   

    return exponentialValues;
}

// dao nguoc thoi gian 

vector<pair<int, double> > timeReversal(const vector<pair<int, double>>& signal) {
   
    vector<pair<int, double>> reversedSignal;

    for (const auto& pair : signal) {
        
        int newIndex = -pair.first;

        reversedSignal.push_back({newIndex,pair.second}) ;
    }

    return reversedSignal;
}

//giam tan so lay mau 

vector<pair<int, double>> downsample(vector<pair<int, double>> signal, int M) {
	int newSize = signal.size() / M;
    vector<pair<int, double>> downsampledSignal;
	int j = startIndex;
    for (int i = startIndex; i <= endIndex ; i=i+M) {
    	
        downsampledSignal.push_back({j,signal[i + M].second});
        j++;
        
    }
    return downsampledSignal;
}


//tang tan so lay mau

vector<pair<int, double>> upsample(vector<pair<int, double>> signal, int L) {
    vector<pair<int, double>> upsampledSignal;
    int j = 0;
   	int x = startIndex;
    for (auto pair: signal) {	
    	double y = pair.second;
        upsampledSignal.push_back({x,y}); 

        for (int j = 0; j < L; ++j) {
        	x++;
            upsampledSignal.push_back({x,0}); 
        
        }
        x++;
    }

    return upsampledSignal;
}

// phep cong hai day tin hieu
vector<pair<int, double>> addSignals(const vector<pair<int, double>> &signal1, const vector<pair<int, double>> &signal2)
{
   
    int minX = min(signal1.front().first, signal2.front().first);
    int maxX = max(signal1.back().first, signal2.back().first);

   
    vector<int> xValues;
    for (int i = minX; i <= maxX; ++i)
    {
        xValues.push_back(i);
    }

    
    vector<pair<int, double>> result;

    
    for (int x : xValues)
    {
        double y = 0.0;

        
        auto it1 = find_if(signal1.begin(), signal1.end(), [x](const pair<int, double> &p)
                           { return p.first == x; });
        if (it1 != signal1.end())
        {
            y += it1->second;
        }

        
        auto it2 = find_if(signal2.begin(), signal2.end(), [x](const pair<int, double> &p)
                           { return p.first == x; });
        if (it2 != signal2.end())
        {
            y += it2->second;
        }

        result.push_back({x, y});
    }

    return result;
}

vector<pair<int, double>> subSignals(const vector<pair<int, double>> &signal1, const vector<pair<int, double>> &signal2)
{
   
    int minX = min(signal1.front().first, signal2.front().first);
    int maxX = max(signal1.back().first, signal2.back().first);

   
    vector<int> xValues;
    for (int i = minX; i <= maxX; ++i)
    {
        xValues.push_back(i);
    }

    
    vector<pair<int, double>> result;

    
    for (int x : xValues)
    {
        double y = 0.0;

        
        auto it1 = find_if(signal1.begin(), signal1.end(), [x](const pair<int, double> &p)
                           { return p.first == x; });
        if (it1 != signal1.end())
        {
            y += it1->second;
        }

        
        auto it2 = find_if(signal2.begin(), signal2.end(), [x](const pair<int, double> &p)
                           { return p.first == x; });
        if (it2 != signal2.end())
        {
            y -= it2->second;
        }

        result.push_back({x, y});
    }

    return result;
}

// Viet ra file output.txt
void writeToFile(const string& filename, vector<pair<int,double>> waveform) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Not Found " << filename << endl;
        return;
    }
	for(auto a: waveform){
		file << a.first <<" "<< a.second <<endl;
	}

    file.close();
}

vector<pair<int, double>> convolution(const vector<pair<int, double>>& signal1, const vector<pair<int, double>>& signal2) {
    
    int length = signal1.size() + signal2.size() - 1;
    vector<pair<int, double>> result(length, {0, 0.0});

    
    for (int i = 0; i < signal1.size(); ++i) {
        for (int j = 0; j < signal2.size(); ++j) {
            result[i + j].second += signal1[i].second * signal2[j].second;
            result[i + j].first = i + j; 
        }
    }

    return result;
}
void writeToFile2( const string& filename, vector<pair<int, Complex>>& waveform) {
  ofstream outfile(filename);
  for (auto& a : waveform) {
    outfile << a.first << " " << a.second.real << " " << a.second.imag << endl;
  }
  outfile.close();
}



int main() {
    double amplitude = 1.0; 
    int halfPeriod = 6; 
    double frequency = 1.0;
    int numSamples = 10;
    double step = 0.1;
    vector<pair<int,double>> finalSignal;
    
    double realExpConst = 0.2; // hang so mu thuc
	double baseValue = 0.9;
	
	int shiftAmount = -10; // Gia tri dich thoi gian // >0 delayed <0 advanced
	
	
	//Sinh 2 day cho tich chap
	vector<pair<int,double>> waveformA;
	for(int i = 1; i <= 6 ; i++){
		waveformA.push_back({i-1,i});
	}
	vector<pair<int,double>> waveformB;
	for(int i = 1; i <= 3 ; i++){
		waveformB.push_back({i-1,1});
	}
	
// 	vector<pair<int,double>> waveform = generateSquareWave(amplitude, halfPeriod);// sinh day chu nhat

//	vector<pair<int,double>> waveform = generateUnitImpulseFunction(); // sinh day xung don vi

//	vector<pair<int,double>> waveform = generateUnitStepFunction();//ham sinh day nhay don vi

//	vector<pair<int, Complex>> waveform = generateComplexExponential(baseValue);//ham mu
	
//	vector<pair<int,double>> waveform = generateRealExponentialFunction(baseValue);//ham mu; // sinh day ham mu thuc

// 	vector<pair<int,double>> waveform = generateSineWave(numSamples,frequency, amplitude);//ham sin

//	vector<pair<int,double>> waveform = generateUnitRampFunction(); // sinh day doc don vi
	
//	finalSignal = timeShift(waveform, shiftAmount);//phep dich thoi gian
	
//	finalSignal = timeReversal(waveform);//phep dao nguoc thoi gian
	
//	finalSignal = downsample(waveform, 2); // giam tan so lay mau
	
//	finalSignal = upsample(waveform, 2);

//	finalSignal = convolution(waveformA,waveformB);
	
//	finalSignal = addSignals(waveformA, waveformB);

	finalSignal = subSignals(waveformA, waveformB);
	
 	writeToFile(filename, finalSignal); // viet ra file
  	
    cout << "Updated " << filename << endl;
    
    //Neu gap loi "'gnuplot' is not recognized as an internal or external command" 
    //Mo Control Panel tr�n Windows v� di den System and Security > System > Advanced system settings.
	//Trong hop thoai System Properties, chon tab Advanced, v� nhap v�o n�t Environment Variables.
	//Trong phan System Variables, t�m bien m�i tru?ng c� ten PATH v� chon Edit.
	//Th�m duung dan den thu muc c�i dat Gnuplot v�o cuoi bien PATH (C:\Program Files\gnuplot\bin).
	//Nhap OK de luu thay doi 
	
    // Dong lenh nay dung de hien plot sau khi chay ct ma khong can mo gnuplot
    string command = "gnuplot singal_demo.plt";
    system("gnuplot -persist singal_demo.plt");
    return 0;
}

