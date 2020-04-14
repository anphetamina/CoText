//
// Created by Emanuele Munafò on 13/04/2020.
//
#pragma once

#define FLAG_ER 0x45
#define FLAG_OK 0x4D
#define FLAG_CTRL  0x00


// Full type definition (CAT|OPCode)
// (CAT|OPCode) => (Login|Authentication)
#define PACK_TYPE_PING 0x0001
#define PACK_TYPE_LOGIN_REQ 0x0010
#define PACK_TYPE_LOGIN_RES 0x0011

