#ifndef KU040RXCORE_H
#define KU040RXCORE_H

// #################################
// # Author: Marius Wensing
// # Email: marius.wensing at cern.ch
// # Project: Yarr
// # Description: KU040 Receiver
// # Comment:
// # Data: May 2017
// ################################

#include <queue>
#include <vector>
#include <cstdint>
#include "RxCore.h"
#include "IPbus.h"
#include "RawData.h"

class KU040RxCore : virtual public RxCore {
    public:
        KU040RxCore();
        ~KU040RxCore();

        void setRxEnable(uint32_t val);
        void maskRxEnable(uint32_t val, uint32_t mask);

        RawData* readData();
        
        uint32_t getDataRate();
        uint32_t getCurCount();
        bool isBridgeEmpty();

        void setEmu(uint32_t mask, uint8_t hitcnt = 0);
        uint32_t getEmu();

		void setSkipRecsWithErrors(bool value)
		{
			if(value)
			{
				std::cout << "KU040RxCore will now skip records with errors." << std::endl;
			}
			else
			{
				std::cout << "KU040RxCore will now process ALL records (even with errors)." << std::endl;
			}
			m_skipRecsWithErrors = value;
		}

		bool getSkipRecsWithErrors()
		{
			return m_skipRecsWithErrors;
		}

        void setCom(IPbus *com) {
            m_com = com;
        }

        IPbus *getCom() {
            return m_com;
        }

    private:

        uint32_t m_enableMask;
        uint32_t m_emuMask;
		bool m_skipRecsWithErrors;
        IPbus *m_com;
};

#endif
