#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstdlib>
using namespace std;


vector<double> echoEffect(const vector<double>& data, int delay, double echoFactor) {
  if (delay <= 0 || delay >= data.size()) {
    return data; 
  }

  vector<double> echoedData(data.size());

  for (size_t i = 0; i < data.size(); ++i) {
    echoedData[i] = data[i];
    if (i >= delay) {
      echoedData[i] += echoFactor * data[i - delay]; 
    }
  }

  return echoedData;
}

//echo

// vector<double> echo(const vector<double>& signal, double delay, double gain) {
//   int signal_length = signal.size();
//   vector<double> echoed_signal(signal_length);

//   for (int i = 0; i < signal_length; ++i) {
//     echoed_signal[i] = signal[i];
//   }

//   for (int i = delay; i < signal_length; ++i) {
//     echoed_signal[i] += gain * signal[i - delay];
//   }
//   return echoed_signal;
// }

  void writeToFile(const string& filename, vector<double> waveform) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Not Found " << filename << endl;
        return;
    }
	for(auto a: waveform){
		file << a << endl;
	}

    file.close();
}


int main(){
    vector<double> originalData = {1.0, 2.5, 3.8, 4.2, 1.9, 0.7, 5.4, 2.3, 3.1, 6.5,3.7, 1.2, 4.8, 2.9, 3.6,1.5, 4.1, 0.8, 3.2, 2.9, 4.3, 1.6};
    int delay = 2; 
    double echoFactor = 0.5; //0 -> 1
    
    vector<double> waveform = echoEffect(originalData, delay, echoFactor);
    writeToFile("EchoData.txt",waveform);
  
    return 0;

}