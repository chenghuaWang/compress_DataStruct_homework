//
// Created by wang on 2021/6/2.
// __author__: ChengHua WANG
/*
 *  This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
/*
 * this hfman head for huffman encode and decode.
 * all of the tmp file was encode to *.nz with binary mode.
 * 
 * in this cpp file,all inname function was start with a _.
 * all of this function were packaged with a totall methods.
*/
#ifndef HFMAN
#define HFMAN

#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <string>
using namespace std;

// define some method to deal with the byte.
/*
 * GET_BYTE judge the index byte if is true or not
 * TRUE_BYTE set byte to true(1)
 * FALSE_BYTE set byte to false(0)
*/
#define GET_BYTE(buf_byte, index) (((buf_byte) & (1 << ((index) ^ 7))) != 0)
#define TRUE_BYTE(buf_byte, index) ((buf_byte) |= (1 << ((index) ^ 7)))
#define FALSE_BYTE(buf_byte, index) ((buf_byte) &= (~(1 << ((index) ^ 7))))

//set default min index to huffman freq node.
#define DEFAULT -1

#pragma pack(push)
#pragma pack(1)

//struct to load char's frequency
//use unsignedf char due to load more char.
typedef struct _char_freq{
    unsigned char _data;
    int _freq;
}_char_freq;

//struct of huffman tree node
typedef struct _huf_node{
    _char_freq char_freq;
    int _l;
    int _r;
    bool vis;
    char * _code;
}_huf_node;

//struct info write to file's head.
//para: _key, to judge the file can be processed by this program
//para: _char_var, the different char's number
//para: _signi_dig, the last struct in bin file
typedef struct _info{
    unsigned char _key[3];
    unsigned char _char_var;
    unsigned char _signi_dig;
    unsigned char buff[11];
}_info;


namespace hfman{
    bool _fcheck(char *fpath);
    int _get_min_f(_huf_node *node, int n);
    void _creat_huf_tree(_huf_node *node, int var);
    void _free_huf_tree(_huf_node *node, int var);
    void _gen_huf_code(_huf_node *node,int root,int index,char *code);
    _huf_node *_init_node(_char_freq *char_freq,int var,int *index);
    namespace de{
        _info _fread_info(char *fpath);
        _char_freq *_get_freq(char *fpath, int *var,_info finfo);
        void _huf_de(_huf_node *node,char *fromp,char *top, _info info,int var);
    }
    namespace com{
        _char_freq *_get_freq(char *fpath, int *var);
        void _huf_co(_huf_node *node,char *fromp,char *top,int *index, int var, _char_freq *freq);
        int _get_last_bit(_huf_node *node, int var);
    }
    namespace debug{
        void _print_huf_tree(_huf_node *node, int n);
        void _print_char_freq(_char_freq *node, int var);
    }
    void HUFF_COM(char *fromp,char *top);
    void HUFF_DE(char *fromp,char *top);
}
#pragma pack(pop)
#endif