// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#include "RawHeightMap.h"

RawHeightMap::RawHeightMap(int width, int height, float min, float max, unsigned short* ptr) {
    m_width = width;
    m_height = height;
    m_min = min;
    m_max = max;
    m_ptr = ptr;
    m_ratio = 1.0;
}

RawHeightMap::RawHeightMap() {
    m_width = 0;
    m_height = 0;
    m_ptr = 0;
    m_min = 0;
    m_max = 0;
    m_ratio = 1.0;
}

/*RawHeightMap::~RawHeightMap() {
    if(m_ptr)
        delete m_ptr;
}*/

void RawHeightMap::free() {
    if (m_ptr) {
        delete m_ptr;
        m_ptr = 0;
    }
}

#define PTR(i,j) ( m_ptr[(i)*m_width+(j)] )
#define AVERAGE(a, b)   ( ((a) + (b)) >> 1 )

void RawHeightMap::minifyByTwo() {
    int tgWidth = m_width/2;
    int tgHeight = m_height/2;
    unsigned short* target = new unsigned short[tgWidth*tgHeight];
    unsigned short p, q;
    for (int i = 0; i < tgHeight; i++) {
        int i2 = i * 2;
        for (int j = 0; j < tgWidth; j++) {
            int j2 = j * 2;
            p = AVERAGE(PTR(i2,j2), PTR(i2,j2+1));
            q = AVERAGE(PTR(i2+1,j2), PTR(i2+1,j2+1));
            target[i*tgWidth+j] = AVERAGE(p, q);
        }
    }
    m_width = tgWidth;
    m_height = tgHeight;
    delete m_ptr;
    m_ptr = target;
    m_ratio = m_ratio / 2;
}


void RawHeightMap::downscale(unsigned int x) {
    for(unsigned int n = 0; n < x; n++) {
        minifyByTwo();
    }
}
