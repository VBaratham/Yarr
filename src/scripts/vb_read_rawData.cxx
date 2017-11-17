#include <iostream>
#include <fstream>

#include "TTree.h"


struct Fei4Hit {
    unsigned col : 7;
    unsigned row : 9;
    unsigned tot : 5;
    unsigned unused : 11;
};

void eventFromFileBinary(std::fstream &handle,  uint16_t* bcid, uint16_t* l1id, uint32_t* tag) {
    uint16_t t_hits = 0;
    handle.read((char*)&tag, sizeof(uint32_t));
    handle.read((char*)&l1id, sizeof(uint16_t));
    handle.read((char*)&bcid, sizeof(uint16_t));
    handle.read((char*)&t_hits, sizeof(uint16_t));

    std::cout << bcid << ","
              << l1id << ","
              << tag << ","
              << t_hits << std::endl;
    
    for (unsigned i=0; i<t_hits; i++) {
        struct Fei4Hit hit;
        handle.read((char*)&hit, sizeof(Fei4Hit));
        std::cout << "\t"
                  << hit.row << "," 
                  << hit.col << "," 
                  << hit.tot << std::endl;
    }
}


void vb_read_rawData(string fn){
    std::fstream file(fn, std::fstream::in | std::fstream::binary);

    file.seekg(0, std::ios_base::end);
    int size = file.tellg();
    file.seekg(0, std::ios_base::beg);
    std::cout << "Size: " << size/1024.0/1024.0 << " MB" << std::endl;

    while(file) {
      eventFromFileBinary(file);
    }
}
