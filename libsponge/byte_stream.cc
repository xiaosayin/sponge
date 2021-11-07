#include "byte_stream.hh"

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity):write_capacity(capacity),buffer(""),    \
remain_capacity(capacity),cul_bytes_written(0), cul_bytes_read(0), stop_input(false){};

size_t ByteStream::write(const string &data) {
    //DUMMY_CODE(data);
    // if(input_ended()){
    //     return 0;
    // }

    if( (buffer.size() + data.size()) <= write_capacity){
        buffer += data;
        cul_bytes_written += data.size();
        return data.size();
    }
    else{
        int length_fill = write_capacity - buffer.size();
        buffer += data.substr(0,length_fill);
        cul_bytes_written += length_fill;
        return length_fill;
    }

    // return {};
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    // DUMMY_CODE(len);
    // return {};
    return buffer.substr(0,len);
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
    buffer = buffer.substr(len);
    cul_bytes_read += len;
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    // DUMMY_CODE(len);
    // return {};
    
    string read_s = buffer.substr(0,len);
    pop_output(len);
    
    return read_s;
}

void ByteStream::end_input() {stop_input = true;}

bool ByteStream::input_ended() const { return (stop_input) ? true : false; }

size_t ByteStream::buffer_size() const { return buffer.size(); }

bool ByteStream::buffer_empty() const { return buffer.empty(); }

bool ByteStream::eof() const { return (stop_input && buffer.empty()) ; }
//bool ByteStream::eof() const { return eof_private;}


size_t ByteStream::bytes_written() const { return cul_bytes_written; }

size_t ByteStream::bytes_read() const { return cul_bytes_read;}

size_t ByteStream::remaining_capacity() const { return write_capacity - buffer.size(); }
