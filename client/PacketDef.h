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
#define PACK_TYPE_LOGIN_OK 0x0011
#define PACK_TYPE_LOGOUT_REQ 0x0014

#define PACK_TYPE_ACC_CREATE 0x0020
#define PACK_TYPE_ACC_UPDATE 0x0021
#define PACK_TYPE_ACC_OK 0x0022

#define PACK_TYPE_MSG 0x0030

#define PACK_TYPE_CURSOR_POS 0x0041

#define PACK_TYPE_DOC_CREATE 0x0050
#define PACK_TYPE_DOC_DEL 0x0051
#define PACK_TYPE_DOC_OPEN 0x0052
#define PACK_TYPE_DOC_CLOSE 0x0053
#define PACK_TYPE_DOC_OK 0x0054
#define PACK_TYPE_DOC_ASKSURI 0x0055
#define PACK_TYPE_DOC_ERR 0x0056


#define PACK_TYPE_LAST_CODE 0x0099