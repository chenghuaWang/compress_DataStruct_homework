//
// Created by wang on 2021/5/21.
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
#ifndef DATASTRUCTWORK_PACK_H
#define DATASTRUCTWORK_PACK_H
#include "compress.h"

namespace pak{
    void print_default();
    void print_help();
    void print_war_RLC_com();
    void print_war_RLC_de();
}

namespace wf{ //WorkFlow
    void RLC_com(char from_p[], char to_p[]);
    void RLC_de(char from_p[], char to_p[]);
    void LZW_com(char from_p[], char to_p[]);
    void LZW_de(char from_p[],char to_p[]);
}

#endif //DATASTRUCTWORK_PACK_H
