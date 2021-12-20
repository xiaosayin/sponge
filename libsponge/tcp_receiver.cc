#include "tcp_receiver.hh"

// Dummy implementation of a TCP receiver

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

bool TCPReceiver::segment_received(const TCPSegment &seg) {
    // DUMMY_CODE(seg);
    bool ret =false;
    static size_t abs_seqno = 0;

    // segment (include syn and fin) occupies seqno length
    size_t length;
    // check segment syn flag
    if(seg.header().syn){
        // already get a SYN, refuse other SYN.
        if (_syn_flag) {
            return false;
        }

        _syn_flag = true;
        ret = true;
        _isn = seg.header().seqno.raw_value();
        abs_seqno = 1;
        _base = 1;
        length = seg.length_in_sequence_space() - 1;

        // segment have only a SYN flag
        if(length == 0){
            return true;
        }
    }
    // not true syn segment , before get a SYN ,refuse any segment
    else if(!_syn_flag){
        return false;
    }
    //not a SYN segment, compute its abs_seqno and length
    else{
        abs_seqno = unwrap(seg.header().seqno,WrappingInt32(_isn),abs_seqno);
        length = seg.length_in_sequence_space();
    }

    // deal with fin
    if(seg.header().fin){
        //already get a FIN, refuse other FIn
        if(_fin_flag){
            return false;
        }
        _fin_flag = true;
        ret = true;
    }
    // not FIN and not SYN, check border
    else if(seg.length_in_sequence_space() == 0 && abs_seqno == _base){
        return true;
    }
    else if(abs_seqno >= _base + window_size() || abs_seqno + length <= _base){
        if(!ret)
            return false;
    }

    _reassembler.push_substring(seg.payload().copy(), abs_seqno - 1, seg.header().fin);
    _base = _reassembler.head_index() + 1;
    if(_reassembler.input_ended())  // FIN as one byte
        ++(_base);
    return true;
}

optional<WrappingInt32> TCPReceiver::ackno() const {
    if(_base > 0){
        return WrappingInt32(wrap(_base, WrappingInt32(_isn)));
    }
    else
        return std::nullopt; 
}

size_t TCPReceiver::window_size() const { return _capacity - _reassembler.stream_out().buffer_size();}
