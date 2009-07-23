// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#ifndef RAWHEIGHTMAP_H
#define RAWHEIGHTMAP_H

class RawHeightMap {
public:
    RawHeightMap(int width, int height, unsigned short* ptr);
    RawHeightMap();
    //~RawHeightMap();
    void free();
    const unsigned short* getData() const;
    int getWidth() const;
    int getHeight() const;
private:
    unsigned short* m_ptr;
    int m_width;
    int m_height;
};

#endif // RAWHEIGHTMAP_H
