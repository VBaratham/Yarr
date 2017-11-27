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


void vb_read_rawData(string fn){

  // tree variables
  uint16_t bcid, l1id;
  uint32_t tag, nhits;
  std::vector<int> iden, col, row, tot, lv1;

  // tree and branches
  TFile* rootfile = new TFile("rootfile.root","RECREATE");
  TTree* tree = new TTree("HitTree", "Hit Tree");
  // tree->Branch("nhits", &nhits, "nhits/I");
  // tree->Branch("tag", &l1id, "tag/I");
  tree->Branch("euEvt", &bcid, "bcid/I");
  tree->Branch("iden", &iden);
  tree->Branch("col", &col);
  tree->Branch("row", &row);
  tree->Branch("tot", &tot);
  tree->Branch("lv1", &lv1);
  
  std::fstream file(fn, std::fstream::in | std::fstream::binary);

  file.seekg(0, std::ios_base::end);
  int size = file.tellg();
  file.seekg(0, std::ios_base::beg);
  std::cout << "Size: " << size/1024.0/1024.0 << " MB" << std::endl;

  while(file) {
    // eventFromFileBinary(file, &bcid, &l1id, &tag, col, row, tot);
    col.clear();
    row.clear();
    tot.clear();

    file.read((char*)&tag, sizeof(uint32_t));
    file.read((char*)&l1id, sizeof(uint16_t));
    file.read((char*)&bcid, sizeof(uint16_t));
    file.read((char*)&nhits, sizeof(uint16_t));

    for (unsigned i=0; i<nhits; i++) {
      struct Fei4Hit hit;
      file.read((char*)&hit, sizeof(Fei4Hit));

      iden.push_back(1);
      col.push_back(hit.col);
      row.push_back(hit.row);
      tot.push_back(hit.tot);
      lv1.push_back(l1id);

      iden.push_back(2);
      col.push_back(hit.col);
      row.push_back(hit.row);
      tot.push_back(hit.tot);
      lv1.push_back(l1id);
    }
    tree->Fill();
  }

  tree->Write();
  rootfile->Close();
}
