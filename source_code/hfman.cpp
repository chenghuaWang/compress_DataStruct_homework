//
// Created by wang on 2021/5/19.
//
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
#pragma pack(push)
#pragma pack(1)
#include "hfman.h"

bool hfman::_fcheck(char *fpath){
    // check the file putin is exist or not.
    FILE *fp = fopen(fpath, "rb");
    if(fp==NULL) return false;
    fclose(fp);
    return true;
}

int hfman::_get_min_f(_huf_node *node, int n){
    //this function to get min requence of char
    //this find func has O(n) time com
    //return: the index of min node, or -1(DEFAULT) for none
    int mmin = DEFAULT;
    for(int i = 0; i < n; i++){
        if(!node[i].vis){
            if(DEFAULT==mmin||node[i].char_freq._freq<node[mmin].char_freq._freq){
                mmin = i;
            }
        }
    }
    node[mmin].vis = true;
    return mmin;
}

void hfman::_creat_huf_tree(_huf_node* node, int var){
    int left,right;
    for(int i=0;i<var-1;i++){
        left = hfman::_get_min_f(node, var+i);
        right = hfman::_get_min_f(node,var+i);
        node[var+i].char_freq._data = '#';
        node[var+i].char_freq._freq = node[left].char_freq._freq+node[right].char_freq._freq;
        node[var+i]._l = left;
        node[var+i]._r = right;
        node[var+i].vis = false;
    }
}

void hfman::_free_huf_tree(_huf_node* node, int var){
    for(int i=0;i<var;i++){
        free(node[i]._code);
    }
    free(node);
}

void hfman::_gen_huf_code(_huf_node *node,int root,int index,char *code){
    if(node[root]._l!=DEFAULT && node[root]._r!=DEFAULT){
        code[index] = '1';
        hfman::_gen_huf_code(node,node[root]._l,index+1,code);
        code[index] = '0';
        hfman::_gen_huf_code(node,node[root]._r,index+1,code);
    }else{
        //code[index] = '';
        //witch means that code[index] is leaf node.
        code[index] = 0;
        strcpy(node[root]._code, code);
    }
}

_huf_node *hfman::_init_node(_char_freq *char_freq,int var,int *index){
    _huf_node *node = NULL;
    node = (_huf_node *)calloc(sizeof(_huf_node),2 * var - 1);
    if(node == NULL){
        cout<<"ERROE: node mem out."<<endl;
        exit(1);
    }
    for(int i=0;i<var;i++){
        index[char_freq[i]._data]=i;
        node[i].char_freq = char_freq[i];
        node[i].vis = false;
        node[i]._l = node[i]._r = DEFAULT;
        node[i]._code = (char*)calloc(sizeof(char),var);
    }
    return node;
}

_info hfman::de::_fread_info(char *fpath){
    FILE *fp = fopen(fpath, "rb");
    if(!fp){
        cout<<"ERROR:_read_info, fp is not exist."<<endl;
        exit(1);
    }
    _info buf;
    fread(&buf, sizeof(_info), 1, fp);
    fclose(fp);
    return buf;
}

void hfman::de::_huf_de(_huf_node *node,char *fromp,char *top, _info info,int var){
    int root = 2*var - 2,index=0;
    FILE *fin,*fout;
    bool flag = false;
    unsigned char value,out_value;
    long file_size;
    long cur;

    fin = fopen(fromp,"rb");
    fout = fopen(top,"wb");

    fseek(fin, 0L,SEEK_END);
    file_size = ftell(fin); //get the totall size of this file.
    fseek(fin, 16+5*info._char_var,SEEK_SET);
    cur = ftell(fin);

    fread(&value, sizeof(unsigned char),1,fin);
    while(!flag){
        if(node[root]._l==DEFAULT && node[root]._r==DEFAULT){
            out_value = node[root].char_freq._data;
            fwrite(&out_value,sizeof(unsigned char),1,fout);
            if(cur>=file_size && index >= info._signi_dig) break;
            root = 2*var - 2;
        }

        if(GET_BYTE(value,index)){
            root = node[root]._l;
        }
        else root = node[root]._r;
        if(++index>=8){
            index = 0;
            fread(&value,sizeof(unsigned char),1,fin);
            cur = ftell(fin);
        }
    }
    
    fclose(fin);
    fclose(fout);
}

_char_freq *hfman::com::_get_freq(char *fpath, int *var){
    int frequence[256]={0};
    _char_freq *CharFreq;
    FILE *fin = fopen(fpath,"rb");

    int ch = fgetc(fin);
    while(!feof(fin)){
        frequence[ch]++;
        ch = fgetc(fin);
    }
    fclose(fin);

    for(int i=0;i<256;i++) {
		if(frequence[i]) (*var)++;
	}
    int index = 0;
    CharFreq = (_char_freq *)calloc(sizeof(_char_freq),(*var));
    for(int i=0;i<256;i++){
        if(frequence[i]){
            CharFreq[index]._data=i;
            CharFreq[index]._freq=frequence[i];
            index++;
        }
    }
    return CharFreq;
}

void hfman::com::_huf_co(_huf_node *node,char *fromp,char *top,int *index, int var, _char_freq *freq){
    FILE *fin,*fout;
    unsigned char value;
    char *huf_code = NULL;
    _info finfo = {'w','c','h'};

    fin = fopen(fromp,"rb");
    fout = fopen(top,"wb");

    if(!(fin)){
        cout<<"ERROR:check files."<<endl;
        exit(1);
    }

    finfo._char_var = (unsigned char) var;
    finfo._signi_dig = hfman::com::_get_last_bit(node,var);

    fwrite(&finfo,sizeof(_info),1,fout);
    fwrite(freq,sizeof(_char_freq),var,fout);

    int index_bit=0;

    int ch = fgetc(fin);
    while(!feof(fin)){
        huf_code = node[index[ch]]._code;
        for(int i=0;huf_code[i];i++){
            if('0'==huf_code[i]){
                FALSE_BYTE(value,index_bit);
            }
            else TRUE_BYTE(value,index_bit);

            index_bit++;
            if(index_bit>=8){
                index_bit = 0;
                fwrite(&value,sizeof(unsigned char),1,fout);
            }
        }
        ch = fgetc(fin);
    }
    if(index_bit) fwrite(&value,sizeof(unsigned char),1,fout);

    fclose(fin); //TODO BUG
    fclose(fout);
}

int hfman::com::_get_last_bit(_huf_node *node, int var){
    // no bug
    int sum = 0;
    for(int i=0;i<var;i++){
        sum+=strlen(node[i]._code) * node[i].char_freq._freq;
        sum &= 0xFF;
    }
    sum &= 0x7;
    return sum == 0?8:sum;
}

_char_freq *hfman::de::_get_freq(char *fpath, int *var, _info finfo){
    int frequence[256]={0};
    _char_freq *CharFreq = NULL;
    *var = finfo._char_var;
    CharFreq = (_char_freq*)calloc(sizeof(_char_freq),(*var));

    FILE *fin = fopen(fpath,"rb");
    fseek(fin,16,SEEK_SET);
    fread(CharFreq,sizeof(_char_freq),*var,fin);
    fclose(fin);
    return CharFreq;
}

// function under is just for debug,don't use those function
void hfman::debug::_print_huf_tree(_huf_node *node, int n){
    for(int i=0;i<n;i++){
        cout<<
        node[i].char_freq._data<<" "<<
        node[i].char_freq._freq<<" "<<
        node[i]._l<<" "<<node[i]._r<<" "<<
        node[i].vis<<" "<<node[i]._code<<endl;

    }
}

void hfman::debug::_print_char_freq(_char_freq *node, int var){
    for(int i=0;i<var;i++){
        cout<<node[i]._data<<endl;
    }
}

//pack of huffman tree;
void hfman:: HUFF_COM(char *fromp,char *top){
    _char_freq *CharFreq;
    _huf_node *HufNode;
    char *Code = NULL;
    int CharVar = 0;
    int MapIndex[256] = {0};

    if(!hfman::_fcheck(fromp)){
        cout<<"ERROR:(huff man) source file is not exist."<<endl;
        exit(1);
    }

    CharFreq = hfman::com::_get_freq(fromp,&CharVar);
    //cout<<"test:com"<<"1"<<endl;
    HufNode = hfman::_init_node(CharFreq, CharVar, MapIndex);
    //cout<<"test:com"<<"2"<<endl;
    hfman::_creat_huf_tree(HufNode,CharVar);
    //cout<<"test:com"<<"1"<<endl;
    Code = (char*)calloc(sizeof(char),CharVar);
    //cout<<"test:com"<<"2"<<endl;
    hfman::_gen_huf_code(HufNode,2*CharVar-2,0,Code);
    //cout<<"test:com"<<"3"<<endl;
    hfman::com::_huf_co(HufNode,fromp,top,MapIndex,CharVar,CharFreq);
    //cout<<"test:com"<<"4"<<endl;
    hfman::_free_huf_tree(HufNode,CharVar);
    //cout<<"test:com"<<"5"<<endl;
    delete CharFreq;
    delete Code;
}

void hfman::HUFF_DE(char *fromp,char *top){
    _char_freq *CharFreq;
    _huf_node *HufNode;
    char *Code;
    int CharVar = 0;
    int MapIndex[256] = {0};
    _info finfo;

    finfo = hfman::de::_fread_info(fromp);
    if(!(finfo._key[0]=='w'&&finfo._key[1]=='c'&&finfo._key[2]=='h')){
        cout<<"ERROR:You can't decode this file,this file is not compressed by this program"<<endl;
        exit(1);
    }
    //cout<<"test: decode"<<" 1"<<endl;
    CharFreq = hfman::de::_get_freq(fromp,&CharVar,finfo);
    HufNode = hfman::_init_node(CharFreq, CharVar, MapIndex);
    hfman::_creat_huf_tree(HufNode,CharVar);
    Code = (char*)calloc(sizeof(char),CharVar);
    //cout<<"test: decode"<<" 2"<<endl;
    hfman::_gen_huf_code(HufNode,2*CharVar-2,0,Code);
    //cout<<"test: decode"<<" 3"<<endl;
    hfman::de::_huf_de(HufNode,fromp,top,finfo,CharVar);
    //cout<<"test: decode"<<" 4"<<endl;
    hfman::_free_huf_tree(HufNode,CharVar);

    free(CharFreq);
    free(Code);
}
#pragma pack(pop)