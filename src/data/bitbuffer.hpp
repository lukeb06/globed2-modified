// as much as i wanted to separate it into bitbuffer.hpp and bitbuffer.cpp
// c++ got the best of me. go fuck yourself with your
// error: undefined symbol: public: __thiscall BitBuffer<16>::BitBuffer<16>(void)
// i dont know how to do this crap when templates are involved and i dont wanna learn.

/*
* BitBuffer - a simple interface that allows you to write/read bits
*/

#pragma once
#include <bitset>
#include <cstdint>

template <size_t BitCount> requires (BitCount <= 64)
class BitBuffer {
public:
    using UnderlyingType = std::conditional_t<(BitCount <= 8), uint8_t,
                 std::conditional_t<(BitCount <= 16), uint16_t,
                 std::conditional_t<(BitCount <= 32), uint32_t,
                 std::conditional_t<(BitCount <= 64), uint64_t, void>>>>;

    BitBuffer(UnderlyingType val) {
        bitset = std::bitset<BitCount>(val);
    }

    BitBuffer() {}

    void writeBit(bool value) {
        // remember - we are writing from MSB to LSB so we need to reverse the position
        GLOBED_ASSERT(position != BitCount, "BitBuffer tried to write too many bits");
        size_t actualPosition = BitCount - ++position;

        bitset.set(actualPosition, value);
    }

    template <typename... Args> requires (std::same_as<Args, bool> && ...)
    void writeBits(Args... args) {
        (writeBit(args), ...);
    }

    void writeBits(std::vector<bool> values) {
        for (bool value : values) {
            writeBit(value);
        }
    }

    bool readBit() {
        GLOBED_ASSERT(position != BitCount, "BitBuffer tried to read too many bits");
        size_t actualPosition = BitCount - ++position;

        return bitset.test(actualPosition);
    }

    UnderlyingType contents() const {
        return static_cast<UnderlyingType>(bitset.to_ullong());
    }

    size_t getPosition() {
        return position;
    }

    void setPosition(size_t pos) {
        position = pos;
    }

    void clear() {
        bitset.reset();
    }

    constexpr size_t size() {
        return BitCount;
    }
private:
    std::bitset<BitCount> bitset;
    size_t position = 0;
};

template <size_t BitCount>
using BitBufferUnderlyingType = typename BitBuffer<BitCount>::UnderlyingType;