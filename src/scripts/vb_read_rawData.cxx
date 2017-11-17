#include <iostream>
#include <fstream>
#include <vector>

#include "TTree.h"
#include "TFile.h"


struct Fei4Hit {
    unsigned col : 7;
    unsigned row : 9;
    unsigned tot : 5;
    unsigned unused : 11;
};

void eventFromFileBinary(std::fstream &handle,  uint16_t* bcid, uint16_t* l1id,
			 uint32_t* tag, vector<int> col, vector<int> row, vector<int> tot) {
  uint16_t nhits;
  col.clear();
  row.clear();
  tot.clear();
  
  handle.read((char*)tag, sizeof(uint32_t));
  handle.read((char*)l1id, sizeof(uint16_t));
  handle.read((char*)bcid, sizeof(uint16_t));
  handle.read((char*)nhits, sizeof(uint16_t));

  for (unsigned i=0; i<nhits; i++) {
    struct Fei4Hit hit;
    handle.read((char*)&hit, sizeof(Fei4Hit));

    col.push_back(hit.col);
    row.push_back(hit.row);
    tot.bush_back(hit.tot);
  }
}


void vb_read_rawData(string fn){

  // tree variables
  uint16_t bcid, l1id;
  uint32_t tag;
  std::vector<int> row, col, tot;
  int ONE = 1;

  // tree and branches
  TFile* rootfile = new TFile("rootfile.root","RECREATE");
  TTree* tree = new TTree("HitTree", "Hit Tree");
  // tree->Branch("nhits", &nhits, "nhits/I");
  tree->Branch("euEvt", &bcid, "bcid/I");
  tree->Branch("lv1", &l1id, "l1id/I");
  tree->Branch("iden", &ONE, "l1id/I");  // TODO: read this for real
  // tree->Branch("tag", &l1id, "tag/I");
  tree->Branch("col", &col);
  tree->Branch("row", &row);
  tree->Branch("tot", &tot);
  
  std::fstream file(fn, std::fstream::in | std::fstream::binary);

  file.seekg(0, std::ios_base::end);
  int size = file.tellg();
  file.seekg(0, std::ios_base::beg);
  std::cout << "Size: " << size/1024.0/1024.0 << " MB" << std::endl;

  while(file) {
    eventFromFileBinary(file);
    tree.Fill()
  }

  tree->Write();
  rootfile->Close();
}
