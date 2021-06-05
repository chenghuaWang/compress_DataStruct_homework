//
// Created by wang on 2021/5/21.
//
/*
 *                    This program is free software: you can redistribute it and/or modify
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
#include "pack.h"
void pak::print_default(){
    cout<<"ERROR: Missing parameter"<<endl;
    cout<<"You can enter 'DataStructWork -h' to get help information"<<endl;
}

void pak::print_help(){
    // open readme file and print.
    ifstream infile("HELP.txt",ios::in);
    string buff;
    while(getline(infile,buff))cout<<buff<<endl;
}

void pak::print_war_RLC_com(){
    cout<<"ERROR: Missing parameter"<<endl;
    cout<<"enter 'DataStructWork -RLC/-LZW -c from_file to_file'"<<endl;
}

void pak::print_war_RLC_de(){
    cout<<"ERROR: Missing parameter"<<endl;
    cout<<"enter 'DataStructWork -RLC/-LZW -d from_file to_file'"<<endl;
}

void wf::RLC_com(char from_p[], char to_p[]){
    // this RLC compress is composed by charaters and numbers
    // so, the to_p needn't be binary file;
    // it's a naive code, can't judge the file is exist or not;
    ifstream infile(from_p,ios::in);
    ofstream outfile(to_p,ios::trunc);
    string buf;
    while(infile>>buf){
        outfile<<compress::RLC_compress(buf)<<endl;
    }
    infile.close();
    outfile.close();
    cout<<"NOTE: finish RLC compress"<<endl;
}

void wf::RLC_de(char from_p[], char to_p[]){
    // this RLC decompress is composed by charaters and numbers
    // so, the from_p needn't be binary file;
    // it's a naive code, can't judge the file is exist or not;
    ifstream infile(from_p,ios::in);
    ofstream outfile(to_p,ios::trunc);
    string buf;
    while(infile>>buf){
        outfile<<compress::RLC_decompress(buf)<<endl;
    }
    infile.close();
    outfile.close();
    cout<<"NOTE: finish RLC decompress"<<endl;
}

void wf::LZW_com(char from_p[], char to_p[]){
    // a LZW compress work flow,using binary flow to save to to_p.
    // but \n can't be compress, so all string are composed
    ifstream infile(from_p,ios::in);
    string all, buf;
    while(getline(infile,buf)) all.append(buf);
    vector<ll> uu = compress::LZW_compress(all);
    compress::binIO::LZW_write_nz(uu, to_p);
    infile.close();
    cout<<"NOTE: finish LZW compress"<<endl;
}

void wf::LZW_de(char from_p[],char to_p[]){
    // the LZW decompress func. from binary file from_p and save
    // to to_p.
    ofstream outfile(to_p,ios::trunc);
    vector<ll> buf;
    string all;
    buf = compress::binIO::LZW_read_nz(from_p);
    all = compress::LZW_decompress(buf);
    outfile << all;
    outfile.close();
}
