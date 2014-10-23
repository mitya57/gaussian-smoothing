#ifndef __BITARRAY_H
#define __BITARRAY_H

#include <cstring>

class BitArray {
private:
    typedef unsigned char Byte;
    Byte *data;
    bool memoryAllocated;

public:
    BitArray(Byte *_data):
        data(_data), memoryAllocated(false) {}
    BitArray(size_t length):
        data(new Byte[(length >> 3) + 1]),
        memoryAllocated(true) {
        memset(data, 0, (length >> 3) + 1);
    }

    ~BitArray() {
        if (memoryAllocated) {
            delete[] data;
        }
    }

    void setValue(size_t i, bool value) const {
        Byte mask = 0x80 >> (i & 7);
        if (value) {
            data[i >> 3] |= mask;
        } else {
            data[i >> 3] &= ~mask;
        }
    }

    bool value(size_t i) const {
        Byte mask = 0x80 >> (i & 7);
        return data[i >> 3] & mask;
    }
};

#endif /* __BITARRAY_H */
