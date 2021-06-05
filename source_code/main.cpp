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


/* //debug comp
int main(int argc, char const *argv[]) {
	char sourceFileName[256] = {0};
	char targetFileName[256] = {0};
	ALPHA_FREQ *alphaFreq = NULL;				//统计字符及频度的数组
	int alphaVariety = 0;								// 字符种类
	HUFFMAN_TAB *huffmanTab = NULL;			//哈夫曼表
	char *code = NULL;									//存储字符的哈夫曼编码
	int hufIndex[256] = {0};						//下标为字符的ASCII码，其值为该字符在哈夫曼表中的下标，形成键值对
		
	if(argc != 3) {
		printf("正确命令格式: ycyEnCode <源文件名> <目标文件名>\n");
		return 0;
	}
	
	//第二个参数为源文件名
	strcpy(sourceFileName, argv[1]);
	if(!isFileExist(sourceFileName)) {
		printf("源文件(%s)不存在！\n", sourceFileName);
		return 0;
	}
	
	strcpy(targetFileName, argv[2]);
	
	alphaFreq = getAlphaFreq(sourceFileName, &alphaVariety);
	//showAlphaFreq(alphaFreq, alphaVariety);
	
	huffmanTab = initHuffmanTab(alphaFreq, alphaVariety, hufIndex);
	creatHuffmanTree(huffmanTab, alphaVariety);
	code = (char *) calloc(sizeof(char), alphaVariety);
	makeHuffmanCode(huffmanTab, 2 * alphaVariety - 2, 0, code);
	//showHuffmanTab(huffmanTab, 2 * alphaVariety - 1);

	huffmanEncoding(huffmanTab, sourceFileName, targetFileName, hufIndex, alphaVariety, alphaFreq);
	
	destoryHuffmanTab(huffmanTab, alphaVariety);
	free(alphaFreq);
	free(code);
	
	return 0;

}*/


/*
int main(int argc, char const *argv[]) {
	char sourceFileName[256] = {0};
	char targetFileName[256] = {0};
	ALPHA_FREQ *alphaFreq = NULL;				//统计字符及频度的数组
	int alphaVariety = 0;								// 字符种类
	HUFFMAN_TAB *huffmanTab = NULL;			//哈夫曼表
	char *code = NULL;									//存储字符的哈夫曼编码
	int hufIndex[256] = {0};						//下标为字符的ASCII码，其值为该字符在哈夫曼表中的下标，形成键值对
	HUF_FILE_HEAD fileHead;

	if(argc != 3) {
		printf("正确命令格式: ycyDeCode <源文件名> <目标文件名>\n");
		return 0;
	}

	//第二个参数为源文件名
	strcpy(sourceFileName, argv[1]);
	if(!isFileExist(sourceFileName)) {
		printf("源文件(%s)不存在！\n", sourceFileName);
		return 0;
	}
	fileHead = readFileHead(sourceFileName);
	if(!(fileHead.flag[0] == 'w' && fileHead.flag[1] == 'c' && fileHead.flag[2] == 'h')) {
		printf("不可识别的文件格式\n");
	}
	//第三个参数为目标文件名
	strcpy(targetFileName, argv[2]);

	alphaFreq = getAlphaFreq(sourceFileName, &alphaVariety, fileHead);
	//showAlphaFreq(alphaFreq, alphaVariety);

	huffmanTab = initHuffmanTab(alphaFreq, alphaVariety, hufIndex);
	creatHuffmanTree(huffmanTab, alphaVariety);
	code = (char *) calloc(sizeof(char), alphaVariety);
	makeHuffmanCode(huffmanTab, 2 * alphaVariety - 2, 0, code);
	//showHuffmanTab(huffmanTab, 2 * alphaVariety - 1);

	huffmanDecoding(huffmanTab, sourceFileName, targetFileName, alphaVariety, fileHead);

	destoryHuffmanTab(huffmanTab, alphaVariety);
	free(alphaFreq);
	free(code);

	return 0;
}
*/
