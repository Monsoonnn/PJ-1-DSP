#include <iostream>
#include <fstream>
#include <cstring>
#include <cmath>

using namespace std;

struct RIFFHeader{
    char chunk_id[4];
    uint32_t chunk_size;
    char format[4];
};

struct ChunkInfo{
    char chunk_id[4];
    uint32_t chunk_size;
};

struct FmtChunk{
    uint16_t audio_format;
    uint16_t num_channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;
    uint16_t bits_per_sample;
};

struct DataChunk
// We assume 16-bit monochannel samples
{  
    int16_t* data;
    int nb_of_samples;
    DataChunk(int s): nb_of_samples{s}, data{new int16_t[s]} {}
    ~DataChunk(){delete[] data;}
};

void fadeIn(DataChunk& chunk, double duration_seconds, double fade_factor = 1.0) {
    double fade_step = fade_factor / (chunk.nb_of_samples * duration_seconds);
    for (int i = 0; i < chunk.nb_of_samples; ++i) {
        double factor = min(1.0, (i * fade_step));
        chunk.data[i] *= factor;
    }
}

void writeWAVFile(const char* filename, const RIFFHeader& header, const FmtChunk& fmt, const DataChunk& data) {
    ofstream ofs(filename, ios::binary);
    if (!ofs){
        cerr << "Cannot open file for writing" << endl;
        return;
    }

    ofs.write((const char*)(&header), sizeof(header));
    ofs.write((const char*)(&fmt), sizeof(fmt));

    // Write data chunk info
    ChunkInfo data_chunk_info;
    memcpy(data_chunk_info.chunk_id, "data", 4);
    data_chunk_info.chunk_size = data.nb_of_samples * sizeof(int16_t);
    ofs.write((const char*)(&data_chunk_info), sizeof(data_chunk_info));

    // Write data
    ofs.write((const char*)data.data, data_chunk_info.chunk_size);

    ofs.close();
}

int main(){
    constexpr char riff_id[4] = {'R','I','F','F'};
    constexpr char format[4] = {'W','A','V','E'};
    constexpr char fmt_id[4] = {'f','m','t',' '};
    constexpr char data_id[4] = {'d','a','t','a'};

    ifstream ifs{"./input.wav", ios_base::binary};
    if (!ifs){
        cerr << "Cannot open file" << endl;
        return -1;
    }

    // first read RIFF header
    RIFFHeader h;
    ifs.read((char*)(&h), sizeof(h));
    if (!ifs || memcmp(h.chunk_id, riff_id, 4) || memcmp(h.format, format, 4)){
        cerr << "Bad formatting" << endl;
        return -1;
    }

    FmtChunk fmt; // Move fmt declaration here
    bool fmt_read = false;
    bool data_read = false;

    // read chunk infos iteratively
    ChunkInfo ch;
    while(ifs.read((char*)(&ch), sizeof(ch))){
        // if fmt chunk?
        if (memcmp(ch.chunk_id, fmt_id, 4) == 0){
            ifs.read((char*)(&fmt), ch.chunk_size);
            fmt_read = true;
        }
        // is data chunk?
        else if(memcmp(ch.chunk_id, data_id, 4) == 0){
            DataChunk dat_chunk(ch.chunk_size/sizeof(int16_t));
            ifs.read((char*)dat_chunk.data, ch.chunk_size);
            data_read = true;  
            // Write modified data to new WAV file
            RIFFHeader new_header = h;
            writeWAVFile("./output.wav", new_header, fmt, dat_chunk); // Pass fmt here
        }
        // otherwise skip the chunk
        else{
            ifs.seekg(ch.chunk_size, ios_base::cur);
        }
    }
    if (!data_read || !fmt_read){
        cout << "Problem when reading data" << endl;
        return -1;
    }
}


