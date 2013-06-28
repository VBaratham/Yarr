#ifndef SPECCONTROLLER_H
#define SPECCONTROLLER_H

// #################################
// # Author: Timon Heim
// # Email: timon.heim at cern.ch
// # Project: Yarr
// # Description: SPEC cpp library
// # Comment: Original driver taken from Marcus Guillermo
// #          Modified for SPEC card
// ################################

#include <stdint.h>

#include <SpecDevice.h>
#include <KernelMemory.h>
#include <UserMemory.h>
#include <Exception.h>

#define DMACTRLR 0x0
#define DMASTATR 0x1
#define DMACSTARTR 0x2
#define DMAHSTARTLR 0x3
#define DMAHSTARTHR 0x4
#define DMALENR 0x5
#define DMAHNEXTLR 0x6
#define DMAHNEXTHR 0x7
#define DMAATTRIBR 0x8

struct dma_linked_list {
    uint32_t carrier_start;
    uint32_t host_start_l;
    uint32_t host_start_h;
    uint32_t length;
    uint32_t host_next_l;
    uint32_t host_next_h;
    uint32_t attr;
};


using namespace specDriver;

class SpecController {
    public:
        SpecController(unsigned int id);
        ~SpecController();

        void writeSingle(uint32_t off, uint32_t val);
        uint32_t readSingle(uint32_t off);

        void writeBlock(uint32_t off, uint32_t *val, size_t words);
        void readBlock(uint32_t off, uint32_t *val, size_t words);

        void writeDma(uint32_t off, uint32_t *data, size_t words);
        void readDma(uint32_t off, uint32_t *data, size_t words);
             
    private:
        unsigned int specId;
        SpecDevice *spec;
        void *bar0, *bar4;
        
        void init();
        void configure();

        void write32(void *bar, uint32_t off, uint32_t val);
        uint32_t read32(void *bar, uint32_t off);
        void mask32(void *bar, uint32_t off, uint32_t mask, uint32_t val);

        void writeBlock(void *bar, uint32_t off, uint32_t *val, size_t words);
        void readBlock(void *bar, uint32_t off, uint32_t *val, size_t words);

        struct dma_linked_list* prepDmaList(UserMemory *um, KernelMemory *km, uint32_t off, bool write);
        void startDma();
};
#endif