//
// Created by wang on 2021/5/19.
//
/*
 * This program is free software: you can redistribute it and/or modify
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
#ifndef DATASTRUCTWORK_COMPRESS_H
#define DATASTRUCTWORK_COMPRESS_H

#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <cstdio>
#include <cstdlib>
#define MAX_COUNT (9)
#define MINUS (62)
using namespace std;
typedef long long ll;
typedef map<string,ll> MSL;
typedef map<ll,string> MLS;

namespace compress{
    string RLC_compress(string &str);
    string RLC_decompress(string &str);
    vector<ll> LZW_compress(string &str);
    string LZW_decompress(vector<ll> &str);

    // function in compress namespace is packed for main to use
    
    //pre for LZW

    void initDic();
    void initDic_de();
    void print_dic();
    void DEBUG_DIC();

    namespace binIO{
        bool LZW_write_nz(vector<ll> &str, char path[]);
        vector<ll> LZW_read_nz(char path[]);
    }
}
#endif //DATASTRUCTWORK_COMPRESS_H
