#include<iostream>
#include<math.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <random>

std::vector<double> generateData(int size) {
  std::vector<double> data(size);
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<double> noise(-0.1, 0.1);

  // Generate exponentially decaying signal with random noise
  for (int i = 0; i < size; ++i) {
    data[i] = std::exp(-0.1 * i) + noise(gen);
  }

  return data;
}
std::vector<double> echoEffect(const std::vector<double>& data, int delay, double echoFactor) {
  if (delay <= 0 || delay >= data.size()) {
    return data;
  }

  std::vector<double> echoedData(data.size());

  for (size_t i = 0; i < data.size(); ++i) {
    echoedData[i] = data[i];
    if (i >= delay) {
      echoedData[i] += echoFactor * data[i - delay];
    }
  }

  return echoedData;
}

void writeDataToText(const std::vector<double>& data, const std::string& filename, bool append = false) {
  std::ofstream outfile;
  if (append) {
    outfile.open(filename, std::ios::app);  // Open in append mode
  } else {
    outfile.open(filename);
  }
  if (outfile.is_open()) {
    for (double value : data) {
      outfile << value << " ";  // Add space after each value
    }
    outfile << std::endl;
    outfile.close();
  } else {
    std::cerr << "Error opening file: " << filename << std::endl;
  }
}

int main() {
  // Sample data
  std::vector<double> data = generateData(10);
  

  // Apply echo effect
  int delay = 10;
  double echoFactor = 0.5;
  std::vector<double> echoedData = echoEffect(data, delay, echoFactor);

  // Write data to text file
  const std::string filename = "data.txt";
  writeDataToText(data, filename);
  writeDataToText(echoedData, filename,true);  

  return 0;
}
