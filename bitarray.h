#ifndef __BITARRAY_H
#define __BITARRAY_H

#include <cstdlib>
#include <cstring>

class BitArray {
private:
    typedef unsigned char Byte;
    Byte *data;

    class Reference {
    private:
        const BitArray *parent;
        const size_t index;

    public:
        Reference(BitArray *p, size_t i):
        parent(p), index(i) {}

        operator bool() const {
            return parent->value(index);
        }

        Reference &operator = (bool value) {
            parent->setValue(index, value);
            return *this;
        }

        bool operator == (bool value) const {
            return parent->value(index) == value;
        }
    };

    /* Not implemented */
    BitArray(const BitArray &other);
    BitArray &operator =(const BitArray &other);

public:
    BitArray(Byte *_data):
        data(_data) {}
    BitArray(size_t length):
        data(new Byte[(length >> 3) + 1]) {
        memset(data, 0, (length >> 3) + 1);
    }

    ~BitArray() {
        delete[] data;
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

    Reference operator [](size_t i) {
        return Reference(this, i);
    }
};

#endif /* __BITARRAY_H */
