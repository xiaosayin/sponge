#include "wrapping_integers.hh"

// Dummy implementation of a 32-bit wrapping integer

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

//! Transform an "absolute" 64-bit sequence number (zero-indexed) into a WrappingInt32
//! \param n The input absolute 64-bit sequence number
//! \param isn The initial sequence number
WrappingInt32 wrap(uint64_t n, WrappingInt32 isn) {
    // auto mod 2^32
    return isn + n;
}

//! Transform a WrappingInt32 into an "absolute" 64-bit sequence number (zero-indexed)
//! \param n The relative sequence number
//! \param isn The initial sequence number
//! \param checkpoint A recent absolute 64-bit sequence number
//! \returns the 64-bit sequence number that wraps to `n` and is closest to `checkpoint`
//!
//! \note Each of the two streams of the TCP connection has its own ISN. One stream
//! runs from the local TCPSender to the remote TCPReceiver and has one ISN,
//! and the other stream runs from the remote TCPSender to the local TCPReceiver and
//! has a different ISN.
uint64_t unwrap(WrappingInt32 n, WrappingInt32 isn, uint64_t checkpoint) {
    int64_t offset = n.raw_value() - isn.raw_value();
    int64_t check_offset = wrap(checkpoint,isn).raw_value() - isn.raw_value();

    // difference between checkpoint and absolute n 
    int64_t Difference_offset = check_offset - offset;
    // absolute value
    uint64_t absolute_value = checkpoint - Difference_offset;
    
    // absolute n must be closet to checkpoint
    if(abs(static_cast<int64_t> (absolute_value + (1ul<<32) - checkpoint) ) < 
       abs(static_cast<int64_t> (absolute_value - checkpoint)) ){
           return absolute_value + (1ul << 32);
    }

    if( (absolute_value >= (1ul<<32) ) && abs(static_cast<int64_t> (absolute_value - (1ul<<32) - checkpoint)) <       
                                         abs(static_cast<int64_t> (absolute_value - checkpoint)) ){
        return absolute_value - (1ul<<32);
    }

    return absolute_value;

}
