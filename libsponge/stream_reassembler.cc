#include "stream_reassembler.hh"

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity) : _output(capacity), _capacity(capacity), vec_substring(), \
                    curr_length(0), index_list(), output_string(),check_index(0), eof_exist(false),eof_index(0){}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    size_t each_index = index;
    for (auto &s : data){
        // if index doesn't exist in vec_substring
        if(index_list.find(each_index) == index_list.end()){
            vec_substring.push_back(make_pair(each_index,s));
            index_list.insert(each_index);
        }
        ++each_index;
    }

    sort(vec_substring.begin(),vec_substring.end(), [](const pair<size_t, char>& a, const pair<size_t, char>& b){ return a.first < b.first;});
    // size_t check_index = last_assemble_index;
    auto it = vec_substring.begin();
    
    for(auto &p : vec_substring){
        if(p.first != check_index){
            break;
        }
        else{
            ++check_index;
            output_string += p.second;
        }
    }
    // cout << output_string << endl;
    // cout << "isempty:" << vec_substring.empty() << endl;
    // cout << "first_elem_index:" << vec_substring[0].first << endl;
    // cout << "check_index:" << check_index << endl;
    // while(output_string.size() <= _output.remaining_capacity() && it!= vec_substring.end()){
    //     output_string += it->second;
    //     ++it;
    // }
    size_t end_item = output_string.size();
    vec_substring.erase(vec_substring.begin(),it + end_item);
    if(_output.remaining_capacity() >= output_string.size()){
        _output.write(output_string);
        output_string = "";
    }
    else{
        size_t temp = _output.remaining_capacity();
        _output.write(output_string.substr(0,temp));
        output_string = output_string.substr(temp);
    }
    
    if(eof){
        eof_exist = eof;
        eof_index = index + data.size() -1;
    }
    // cout << "is_empty2:" << vec_substring.empty() << endl;
    if(eof_exist && output_string.empty() && vec_substring.empty()){
        _output.end_input();
    }
    
    // curr_length += data.size();
    // // sort the index substring
    
    // //write the inorder substring
    // auto it = vec_substring.begin();
    // for(it = vec_substring.begin(); it != vec_substring.end(); ++it){
    //     if(_output.remaining_capacity() > 0){
    //         if(_output.remaining_capacity() > it->second.size()){
    //             _output.write(it->second);
    //         }
    //         else{
    //             _output.write(it->second.substr(0,_output.remaining_capacity()));
    //             it->second = it->second.substr(_output.remaining_capacity());
    //             break;
    //         }
    //     }
    //     else{
    //         break;
    //     }
    // }
    // // erase the data that has been written in the output
    
    // vec_substring.erase(vec_substring.begin(),it);
    // DUMMY_CODE(data, index, eof);
}

size_t StreamReassembler::unassembled_bytes() const {
        return vec_substring.size(); 
    }

bool StreamReassembler::empty() const { return vec_substring.empty(); }
