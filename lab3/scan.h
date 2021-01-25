/*
* scan头文件
*/
#ifndef _SCAN_H_
#define _SCAN_H_

// token的最大长度
#define MAXTOKENLEN 40

// 保存token的数组
extern char tokenString[MAXTOKENLEN+1];

// 读取token
TokenType getToken(void);

#endif
