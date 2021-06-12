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

#include <iostream>
#include <cstring>
#include "compress.h"
#include "pack.h"
//#include "debugg.h"
#include "hfman.h"
using namespace std;

int main(int argc,char *argv[]){
    if(argc == 0)   pak::print_default();
    else{
        if(!strcmp(argv[1],"-h")){
            pak::print_help();
            return 0;
        }
        else if(!strcmp(argv[1],"-RLC")){//RLC main flow
            if(!strcmp(argv[2],"-c")){ // -RLC compress
                if(argc!=5){
                    pak::print_war_RLC_com();
                    return 0;
                }
                else{
                    char *from = argv[3], *to = argv[4];
                    wf::RLC_com(from,to);
                }
            }
            if(!strcmp(argv[2],"-d")){ // -RLC decompress
                if(argc!=5){
                    pak::print_war_RLC_de();
                    return 0;
                }
                else{
                    char *from = argv[3], *to = argv[4];
                    wf::RLC_de(from,to);
                }
            }
        }
        else if(!strcmp(argv[1],"-LZW")){// -LZW main flow.
            if(!strcmp(argv[2],"-c")){ // -LZW compress
                if(argc!=5){
                    pak::print_war_RLC_com();
                    return 0;
                }
                else{
                    char *from = argv[3], *to = argv[4];
                    wf::LZW_com(from,to);
                }
            }
            if(!strcmp(argv[2],"-d")){ // -RLC decompress
                if(argc!=5){
                    pak::print_war_RLC_de();
                    return 0;
                }
                else{
                    char *from = argv[3], *to = argv[4];
                    wf::LZW_de(from,to);
                }
            }
        }
        else if(!strcmp(argv[1],"-HUF")){
			if(!strcmp(argv[2],"-c")){
                if(argc!=5){
                    pak::print_war_RLC_com();
                    return 0;
                }
                else{
                    char *from = argv[3], *to = argv[4];
                    hfman::HUFF_COM(from,to);
                }
            }
            if(!strcmp(argv[2],"-d")){
                if(argc!=5){
                    pak::print_war_RLC_de();
                    return 0;
                }
                else{
                    char *from = argv[3], *to = argv[4];
                    hfman::HUFF_DE(from,to);
                }
            }
        }
        else{
            cout<<"no such command,you can use -h to check."<<endl;
        }
    }
}