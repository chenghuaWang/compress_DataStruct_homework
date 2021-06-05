//
// Created by wang on 2021/5/19.
//
/*
 *                     This program is free software: you can redistribute it and/or modify
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
#include "compress.h"
map<string,ll> dictionary;// this map only for compress
map<ll,string> dic_deco;
const int N = 1e5+5;
ll exp_buf[N];

// RLC_compress model
string compress::RLC_compress(string &str){
    // run length code method compress.
    if(str.empty()) return "";
    string res;
    res = "";
    int count = 1;
    char last = str[0];
    for(int i=1;i<str.size();i++){
        if(last == str[i] && count < MAX_COUNT) count++;
        else{
            res += to_string(count) + last;
            last = str[i];
            count = 1;
        }
    }
    res += to_string(count) + last;
    return res;
}

string compress::RLC_decompress(string &str){
    if(str.empty()) return "";
    string res;
    res = "";
    for(int i=0;i<str.size();i++){
        if(!(i%2)){
            for(int j=0;j<str[i]-'0';j++){
                res += str[i+1];
            }
        }
    }
    return res;
}

// LZW compress model

void compress::initDic(){
    dictionary.erase(dictionary.begin(),dictionary.end());
    int count = 0;
    string make_dic;
    for(int i=0;i<10;i++){
        make_dic = char('0' + i);
        dictionary[make_dic] = count++;
    }
    for(int i=0;i<26;i++){
        make_dic = char('a' + i);
        dictionary[make_dic] = count++;
    }
    for(int i=0;i<26;i++){
        make_dic = char('A' + i);
        dictionary[make_dic] = count++;
    }
    //dictionary["\n"] = count++;
    //dictionary[" "] = count++;
}

void compress::initDic_de(){
    dic_deco.erase(dic_deco.begin(),dic_deco.end());
    int count = 0;
    string make_dic;
    for(int i=0;i<10;i++){
        make_dic = char('0' + i);
        dic_deco[count++] = make_dic;
    }
    for(int i=0;i<26;i++){
        make_dic = char('a' + i);
        dic_deco[count++] = make_dic;
    }
    for(int i=0;i<26;i++){
        make_dic = char('A' + i);
        dic_deco[count++] = make_dic;
    }
    //dic_deco[count++] = "\n";
    //dic_deco[count++] = " ";
}

void compress::DEBUG_DIC(){
    initDic();
    cout<<"this line only show when debug!!!\n";
    for(auto & it : dictionary){
        cout<<it.first<<"   "<<it.second<<endl;
    }
}

void compress::print_dic(){
    for(auto & it : dictionary){
        cout<<it.first<<"   "<<it.second<<endl;
    }
}

vector<ll> compress::LZW_compress(string &str){
    vector<ll> empty_;empty_.push_back(0);
    if(str.empty()) return empty_;
    initDic();
    vector<ll> res;
    string P, C;
    P = C = "";
    ll minus = MINUS;
    ll len = str.length();
    /*
    for(auto i:str) {
        C = i;
        if (dictionary.count(P + C)) P.append(C);
        else {
            res += (to_string(dictionary[P]) + " ");
            dictionary[P + C] = minus++;
            P = C;
        }
    }*/
    //test
    P = str[0];
    for(int i=0;i<len-1;i++){
        C = str[i+1];
        string buf_fir,buf_sec;
        buf_fir = buf_sec = P;
        if(dictionary.count(buf_fir.append(C))!=0) P = buf_fir;
        else{
            res.push_back(dictionary[P]);
            dictionary[buf_sec.append(C)] = minus++;
            P = C;
        }
    }
    if(!P.empty()){
        res.push_back(dictionary[P]);
    }

    //test
    //print_dic();
    return res;
}

string compress::LZW_decompress(vector<ll> &str){
    if(str.empty()) return "";
    initDic_de();
    string res;
    ll minus = MINUS, cW, pW, buff;
    string P, C;
    P = C = "";
    //step 2 read the first cW;
    buff = str[0];
    cW = buff;
    res += dic_deco[cW];
    //step 3 loop to decode
    for(int i = 1;i<str.size();i++){
        pW = cW;
        cW = str[i];
        if(dic_deco.count(cW)){
            res += dic_deco[cW];
            P = dic_deco[pW], C = dic_deco[cW][0];
            dic_deco[minus++] = P+C;
        }
        else{
            P = dic_deco[pW], C = dic_deco[pW][0];
            //dic_deco[cW] = P+C;
            dic_deco[minus++] = P+C;
            res += (P+C);
        }
    }
    /*
    for(auto & it : dic_deco){
        cout<<it.first<<"   "<<it.second<<endl;
    }
     */
    return res;
}

// IO write to binary .nz file, read from a .txt file

bool compress::binIO::LZW_write_nz(vector<ll> &str, char path[]){
    FILE *fp;
    fp = fopen(path, "w+");
    ll len = str.size();
    ll *buf = new ll[len + 5];
    buf[0] = len;
    for(int i = 1;i<=str.size();i++){
        //cout<<str[i-1]<<" ";
        buf[i] = str[i-1];
    }
    fwrite(buf, sizeof(ll) , len + 5, fp);
    return true;
    fclose(fp);
}

vector<ll> compress::binIO::LZW_read_nz(char path[]){
    vector<ll> res;
    fill(exp_buf, exp_buf+N, 0);
    FILE *fp;
    fp = fopen(path, "r");
    // fseek(fp, 0, SEEK_SET); // find the head of this file.
    fread(exp_buf, sizeof(ll), N, fp);
    res.reserve(exp_buf[0]+5); // re allocate the vector mem before push_back.
    for(int i = 1;i<=exp_buf[0];i++){
        res.push_back(exp_buf[i]);
        //cout<<exp_buf[i]<<" ";
    }
    fclose(fp);
    return res;
}