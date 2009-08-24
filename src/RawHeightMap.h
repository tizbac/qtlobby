// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#ifndef RAWHEIGHTMAP_H
#define RAWHEIGHTMAP_H

class RawHeightMap {
public:
    RawHeightMap(int width, int height, float min, float max, unsigned short* ptr);
    RawHeightMap();
    //~RawHeightMap();
    void free();
    const unsigned short* getData() const;
    int getWidth() const;
    int getHeight() const;
    int getMinHeight() const;
    int getMaxHeight() const;
    void downscale(unsigned int x);
    float getRatio();
private:
    void minifyByTwo();
    
    unsigned short* m_ptr;
    int m_width;
    int m_height;
    float m_min;
    float m_max;
    float m_ratio;
};

inline const unsigned short* RawHeightMap::getData() const {
    return m_ptr;
}

inline int RawHeightMap::getWidth() const {
    return m_width;
}

inline int RawHeightMap::getHeight() const {
    return m_height;
}

inline int RawHeightMap::getMinHeight() const {
    return m_min;
}

inline int RawHeightMap::getMaxHeight() const {
    return m_max;
}

inline float RawHeightMap::getRatio() {
    return m_ratio;
}

#endif // RAWHEIGHTMAP_H
