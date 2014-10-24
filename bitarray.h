#ifndef __BITARRAY_H
#define __BITARRAY_H

struct BitArray {
    typedef unsigned char Byte;
    Byte *data;

    BitArray(Byte *_data):
        data(_data) {}

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
