#include <iostream>
#include <fstream>
#include <vector>

#include "Fei4EventData.h"

#include "TTree.h"
#include "TFile.h"


int main(int argc, char* argv[]){
  std::vector<std::fstream*> files;

  // Open files
  for(int i = 1; i < argc; ++i)
    files.push_back(new std::fstream(argv[i]));

  // tree variables
  uint16_t bcid;
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

  // Report file sizes
  for(std::fstream *file : files) {
      file->seekg(0, std::ios_base::end);
      int size = file->tellg();
      file->seekg(0, std::ios_base::beg);
      std::cout << "Size: " << size/1024.0/1024.0 << " MB" << std::endl;
  }

  while(files[0]) {
    col.clear();
    row.clear();
    tot.clear();

    if (!*files[0])
      break;

    // file.read((char*)&tag, sizeof(uint32_t));
    // file.read((char*)&l1id, sizeof(uint16_t));
    // file.read((char*)&bcid, sizeof(uint16_t));
    // file.read((char*)&nhits, sizeof(uint16_t));

    // for (unsigned i=0; i<nhits; i++) {
    //   struct Fei4Hit hit;
    //   file.read((char*)&hit, sizeof(Fei4Hit));

    //   iden.push_back(1);
    //   col.push_back(hit.col);
    //   row.push_back(hit.row);
    //   tot.push_back(hit.tot);
    //   lv1.push_back(l1id);

    //   // iden.push_back(2);
    //   // col.push_back(hit.col);
    //   // row.push_back(hit.row);
    //   // tot.push_back(hit.tot);
    //   // lv1.push_back(l1id);
    // }

    for (int i = 0; i < files.size(); i++) {
      // eventFromFileBinary(files[i], &bcid, &l1id, &tag, col, row, tot);
      Fei4Event event;

      event.fromFileBinary(*files[i]);
      iden.push_back(i);
      lv1.push_back(event.l1id);
    }

    tree->Fill();
  }

  tree->Write();
  rootfile->Close();
}
