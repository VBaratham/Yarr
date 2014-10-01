// #################################
// # Author: Timon Heim
// # Email: timon.heim at cern.ch
// # Project: Yarr
// # Description: FE-I4 cpp Library
// # Comment: FE-I4 Pixel Register container
// ################################

#include "Fei4PixelCfg.h"

void DoubleColumnBit::set(const uint32_t *bitstream) {
    for(unsigned i=0; i<n_Words; i++)
        storage[i] = bitstream[i];
}

void DoubleColumnBit::setAll(const uint32_t val) {
    for(unsigned i=0; i<n_Words; i++)
        storage[i] = val;
}

void DoubleColumnBit::setPixel(const unsigned n, uint32_t val) {
    val <<= (n-1)%32;
    uint32_t mask = 0x1 << ((n-1)%32);
    storage[(n-1)/32] = val | (storage[(n-1)/32]&~mask);
}

uint32_t* DoubleColumnBit::getStream() {
    return storage;
}

uint32_t DoubleColumnBit::getPixel(const unsigned n) {
    uint32_t mask = 0x1 << ((n-1)%32);
    return ((storage[(n-1)/32] & mask) >> ((n-1)%32));
}

uint32_t DoubleColumnBit::getWord(const unsigned n) {
    return storage[n];
}

Fei4PixelCfg::Fei4PixelCfg() {
    for (unsigned i=0; i<n_DC; i++) {
        m_En[i].setAll(0);
        m_TDAC[i].setAll(15);
        m_LCap[i].setAll(0);
        m_SCap[i].setAll(0);
        m_Hitbus[i].setAll(1);
        m_FDAC[i].setAll(7);
    }
}

uint32_t* Fei4PixelCfg::getCfg(unsigned bit, unsigned dc) {
    switch (bit) {
        case 0:
            return m_En[dc].getStream();
            break;
        case 1:
            return m_TDAC[dc][0].getStream();
            break;
        case 2:
            return m_TDAC[dc][1].getStream();
            break;
        case 3:
            return m_TDAC[dc][2].getStream();
            break;
        case 4:
            return m_TDAC[dc][3].getStream();
            break;
        case 5:
            return m_TDAC[dc][4].getStream();
            break;
        case 6:
            return m_LCap[dc].getStream();
            break;
        case 7:
            return m_SCap[dc].getStream();
            break;
        case 8:
            return m_Hitbus[dc].getStream();
            break;
        case 9:
            return m_FDAC[dc][0].getStream();
            break;
        case 10:
            return m_FDAC[dc][1].getStream();
            break;
        case 11:
            return m_FDAC[dc][2].getStream();
            break;
        case 12:
            return m_FDAC[dc][3].getStream();
            break;
        default:
            return NULL;
            break;
    }
}

DoubleColumnBit& Fei4PixelCfg::En(unsigned dc) {
    return m_En[dc];
}

DoubleColumnField<5, true>& Fei4PixelCfg::TDAC(unsigned dc) {
    return m_TDAC[dc];
}

DoubleColumnBit& Fei4PixelCfg::LCap(unsigned dc) {
    return m_LCap[dc];
}


DoubleColumnBit& Fei4PixelCfg::SCap(unsigned dc) {
    return m_SCap[dc];
}

DoubleColumnBit& Fei4PixelCfg::Hitbus(unsigned dc) {
    return m_Hitbus[dc];
}

DoubleColumnField<4, false>& Fei4PixelCfg::FDAC(unsigned dc) {
    return m_FDAC[dc];
}

unsigned Fei4PixelCfg::to_dc(unsigned col) {
    return ((col-1)/2);
}

unsigned Fei4PixelCfg::to_bit(unsigned col, unsigned row) {
    return (col%2==0) ? 336-row : 335+row;
}

void Fei4PixelCfg::setEn(unsigned col, unsigned row, unsigned v) {
    m_En[to_dc(col)].setPixel(to_bit(col, row) , v);
}