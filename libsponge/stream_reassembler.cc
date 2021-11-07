#include "stream_reassembler.hh"

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity) : _output(capacity), _capacity(capacity) {
    _buffer.resize(capacity);
}

long StreamReassembler::merge_block(block_node &elm1, const block_node &elm2) {
    block_node x, y;
    if (elm1.begin > elm2.begin) {
        x = elm2;
        y = elm1;
    } else {
        x = elm1;
        y = elm2;
    }
    if (x.begin + x.length < y.begin) {
        return -1;  // no intersection, couldn't merge
    } else if (x.begin + x.length >= y.begin + y.length) {
        elm1 = x;
        return y.length;
    } else {
        elm1.begin = x.begin;
        elm1.data = x.data + y.data.substr(x.begin + x.length - y.begin);
        elm1.length = elm1.data.length();
        return x.begin + x.length - y.begin;
    }
}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    if (index >= _head_index + _capacity) {  // capacity over
        return;
    }

    // handle extra substring prefix
    block_node elm;
    if (index + data.length() <= _head_index) {  // couldn't equal, because there have emtpy substring
        goto JUDGE_EOF;
    } else if (index < _head_index) {
        size_t offset = _head_index - index;
        elm.data.assign(data.begin() + offset, data.end());
        elm.begin = index + offset;
        elm.length = elm.data.length();
    } else {
        elm.begin = index;
        elm.length = data.length();
        elm.data = data;
    }
    _unassembled_byte += elm.length;

    // merge substring
    do {
        // merge next
        long merged_bytes = 0;
        auto iter = _blocks.lower_bound(elm);
        while (iter != _blocks.end() && (merged_bytes = merge_block(elm, *iter)) >= 0) {
            _unassembled_byte -= merged_bytes;
            _blocks.erase(iter);
            iter = _blocks.lower_bound(elm);
        }
        // merge prev
        if (iter == _blocks.begin()) {
            break;
        }
        iter--;
        while ((merged_bytes = merge_block(elm, *iter)) >= 0) {
            _unassembled_byte -= merged_bytes;
            _blocks.erase(iter);
            iter = _blocks.lower_bound(elm);
            if (iter == _blocks.begin()) {
                break;
            }
            iter--;
        }
    } while (false);
    _blocks.insert(elm);

    // write to ByteStream
    if (!_blocks.empty() && _blocks.begin()->begin == _head_index) {
        const block_node head_block = *_blocks.begin();
        // modify _head_index and _unassembled_byte according to successful write to _output
        size_t write_bytes = _output.write(head_block.data);
        _head_index += write_bytes;
        _unassembled_byte -= write_bytes;
        _blocks.erase(_blocks.begin());
    }

JUDGE_EOF:
    if (eof) {
        _eof_flag = true;
    }
    if (_eof_flag && empty()) {
        _output.end_input();
    }
}

size_t StreamReassembler::unassembled_bytes() const { return _unassembled_byte; }

bool StreamReassembler::empty() const { return _unassembled_byte == 0; }


// my achieve
// StreamReassembler::StreamReassembler(const size_t capacity) : _output(capacity), _capacity(capacity), vec_substring(), 
//                     curr_length(0), index_list(), output_string(),check_index(0), eof_exist(false),eof_index(0){}

// //! \details This function accepts a substring (aka a segment) of bytes,
// //! possibly out-of-order, from the logical stream, and assembles any newly
// //! contiguous substrings and writes them into the output stream in order.
// void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
//     size_t each_index = index;
//     for (auto &s : data){
//         // if index doesn't exist in vec_substring
//         if(index_list.find(each_index) == index_list.end()){
//             vec_substring.push_back(make_pair(each_index,s));
//             index_list.insert(each_index);
//         }
//         ++each_index;
//     }

//     sort(vec_substring.begin(),vec_substring.end(), [](const pair<size_t, char>& a, const pair<size_t, char>& b){ return a.first < b.first;});
//     // size_t check_index = last_assemble_index;
//     auto it = vec_substring.begin();
    
//     for(auto &p : vec_substring){
//         if(p.first != check_index){
//             break;
//         }
//         else{
//             ++check_index;
//             output_string += p.second;
//         }
//     }
//     // cout << output_string << endl;
//     // cout << "isempty:" << vec_substring.empty() << endl;
//     // cout << "first_elem_index:" << vec_substring[0].first << endl;
//     // cout << "check_index:" << check_index << endl;
//     // while(output_string.size() <= _output.remaining_capacity() && it!= vec_substring.end()){
//     //     output_string += it->second;
//     //     ++it;
//     // }
//     size_t end_item = output_string.size();
//     vec_substring.erase(vec_substring.begin(),it + end_item);
//     if(_output.remaining_capacity() >= output_string.size()){
//         _output.write(output_string);
//         output_string = "";
//     }
//     else{
//         size_t temp = _output.remaining_capacity();
//         _output.write(output_string.substr(0,temp));
//         output_string = output_string.substr(temp);
//     }
    
//     if(eof){
//         eof_exist = eof;
//         eof_index = index + data.size() -1;
//     }
//     // cout << "is_empty2:" << vec_substring.empty() << endl;
//     if(eof_exist && output_string.empty() && vec_substring.empty()){
//         _output.end_input();
//     }
    
//     // curr_length += data.size();
//     // // sort the index substring
    
//     // //write the inorder substring
//     // auto it = vec_substring.begin();
//     // for(it = vec_substring.begin(); it != vec_substring.end(); ++it){
//     //     if(_output.remaining_capacity() > 0){
//     //         if(_output.remaining_capacity() > it->second.size()){
//     //             _output.write(it->second);
//     //         }
//     //         else{
//     //             _output.write(it->second.substr(0,_output.remaining_capacity()));
//     //             it->second = it->second.substr(_output.remaining_capacity());
//     //             break;
//     //         }
//     //     }
//     //     else{
//     //         break;
//     //     }
//     // }
//     // // erase the data that has been written in the output
    
//     // vec_substring.erase(vec_substring.begin(),it);
//     // DUMMY_CODE(data, index, eof);
// }

// size_t StreamReassembler::unassembled_bytes() const {
//         return vec_substring.size(); 
//     }

// bool StreamReassembler::empty() const { return vec_substring.empty(); }
