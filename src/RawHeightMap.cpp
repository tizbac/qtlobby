#include "RawHeightMap.h"

RawHeightMap::RawHeightMap(int width, int height, unsigned short* ptr) {
    m_width = width;
    m_height = height;
    m_ptr = ptr;
}

RawHeightMap::RawHeightMap() {
    m_width = 0;
    m_height = 0;
    m_ptr = 0;
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

const unsigned short* RawHeightMap::getData() const {
    return m_ptr;
}

int RawHeightMap::getWidth() const {
    return m_width;
}

int RawHeightMap::getHeight() const {
    return m_height;
}
