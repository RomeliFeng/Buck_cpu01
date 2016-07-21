/*
 * OLED.cpp
 *
 *  Created on: 2016��5��22��
 *      Author: Romeli
 *      PinMap:		|P14------DC|*
 *      			|P15-----RES|*
 *      			|P16----MOSI|*
 *      			|P17----MISO|
 *      			|P18-----SLK|*
 *      			|P19-----STE|
 */
#include "OLED.h"
#include "SPI.h"
#include "stdio.h"
#include "string.h"
#include "math.h"

const unsigned char F6x8[][6] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // sp
		0x00, 0x00, 0x00, 0x2f, 0x00, 0x00, // !
		0x00, 0x00, 0x07, 0x00, 0x07, 0x00, // "
		0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14, // #
		0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12, // $
		0x00, 0x62, 0x64, 0x08, 0x13, 0x23, // %
		0x00, 0x36, 0x49, 0x55, 0x22, 0x50, // &
		0x00, 0x00, 0x05, 0x03, 0x00, 0x00, // '
		0x00, 0x00, 0x1c, 0x22, 0x41, 0x00, // (
		0x00, 0x00, 0x41, 0x22, 0x1c, 0x00, // )
		0x00, 0x14, 0x08, 0x3E, 0x08, 0x14, // *
		0x00, 0x08, 0x08, 0x3E, 0x08, 0x08, // +
		0x00, 0x00, 0x00, 0xA0, 0x60, 0x00, // ,
		0x00, 0x08, 0x08, 0x08, 0x08, 0x08, // -
		0x00, 0x00, 0x60, 0x60, 0x00, 0x00, // .
		0x00, 0x20, 0x10, 0x08, 0x04, 0x02, // /
		0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E, // 0
		0x00, 0x00, 0x42, 0x7F, 0x40, 0x00, // 1
		0x00, 0x42, 0x61, 0x51, 0x49, 0x46, // 2
		0x00, 0x21, 0x41, 0x45, 0x4B, 0x31, // 3
		0x00, 0x18, 0x14, 0x12, 0x7F, 0x10, // 4
		0x00, 0x27, 0x45, 0x45, 0x45, 0x39, // 5
		0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30, // 6
		0x00, 0x01, 0x71, 0x09, 0x05, 0x03, // 7
		0x00, 0x36, 0x49, 0x49, 0x49, 0x36, // 8
		0x00, 0x06, 0x49, 0x49, 0x29, 0x1E, // 9
		0x00, 0x00, 0x36, 0x36, 0x00, 0x00, // :
		0x00, 0x00, 0x56, 0x36, 0x00, 0x00, // ;
		0x00, 0x08, 0x14, 0x22, 0x41, 0x00, // <
		0x00, 0x14, 0x14, 0x14, 0x14, 0x14, // =
		0x00, 0x00, 0x41, 0x22, 0x14, 0x08, // >
		0x00, 0x02, 0x01, 0x51, 0x09, 0x06, // ?
		0x00, 0x32, 0x49, 0x59, 0x51, 0x3E, // @
		0x00, 0x7C, 0x12, 0x11, 0x12, 0x7C, // A
		0x00, 0x7F, 0x49, 0x49, 0x49, 0x36, // B
		0x00, 0x3E, 0x41, 0x41, 0x41, 0x22, // C
		0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C, // D
		0x00, 0x7F, 0x49, 0x49, 0x49, 0x41, // E
		0x00, 0x7F, 0x09, 0x09, 0x09, 0x01, // F
		0x00, 0x3E, 0x41, 0x49, 0x49, 0x7A, // G
		0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F, // H
		0x00, 0x00, 0x41, 0x7F, 0x41, 0x00, // I
		0x00, 0x20, 0x40, 0x41, 0x3F, 0x01, // J
		0x00, 0x7F, 0x08, 0x14, 0x22, 0x41, // K
		0x00, 0x7F, 0x40, 0x40, 0x40, 0x40, // L
		0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F, // M
		0x00, 0x7F, 0x04, 0x08, 0x10, 0x7F, // N
		0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E, // O
		0x00, 0x7F, 0x09, 0x09, 0x09, 0x06, // P
		0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E, // Q
		0x00, 0x7F, 0x09, 0x19, 0x29, 0x46, // R
		0x00, 0x46, 0x49, 0x49, 0x49, 0x31, // S
		0x00, 0x01, 0x01, 0x7F, 0x01, 0x01, // T
		0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F, // U
		0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F, // V
		0x00, 0x3F, 0x40, 0x38, 0x40, 0x3F, // W
		0x00, 0x63, 0x14, 0x08, 0x14, 0x63, // X
		0x00, 0x07, 0x08, 0x70, 0x08, 0x07, // Y
		0x00, 0x61, 0x51, 0x49, 0x45, 0x43, // Z
		0x00, 0x00, 0x7F, 0x41, 0x41, 0x00, // [
		0x00, 0x55, 0x2A, 0x55, 0x2A, 0x55, // 55
		0x00, 0x00, 0x41, 0x41, 0x7F, 0x00, // ]
		0x00, 0x04, 0x02, 0x01, 0x02, 0x04, // ^
		0x00, 0x40, 0x40, 0x40, 0x40, 0x40, // _
		0x00, 0x00, 0x01, 0x02, 0x04, 0x00, // '
		0x00, 0x20, 0x54, 0x54, 0x54, 0x78, // a
		0x00, 0x7F, 0x48, 0x44, 0x44, 0x38, // b
		0x00, 0x38, 0x44, 0x44, 0x44, 0x20, // c
		0x00, 0x38, 0x44, 0x44, 0x48, 0x7F, // d
		0x00, 0x38, 0x54, 0x54, 0x54, 0x18, // e
		0x00, 0x08, 0x7E, 0x09, 0x01, 0x02, // f
		0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C, // g
		0x00, 0x7F, 0x08, 0x04, 0x04, 0x78, // h
		0x00, 0x00, 0x44, 0x7D, 0x40, 0x00, // i
		0x00, 0x40, 0x80, 0x84, 0x7D, 0x00, // j
		0x00, 0x7F, 0x10, 0x28, 0x44, 0x00, // k
		0x00, 0x00, 0x41, 0x7F, 0x40, 0x00, // l
		0x00, 0x7C, 0x04, 0x18, 0x04, 0x78, // m
		0x00, 0x7C, 0x08, 0x04, 0x04, 0x78, // n
		0x00, 0x38, 0x44, 0x44, 0x44, 0x38, // o
		0x00, 0xFC, 0x24, 0x24, 0x24, 0x18, // p
		0x00, 0x18, 0x24, 0x24, 0x18, 0xFC, // q
		0x00, 0x7C, 0x08, 0x04, 0x04, 0x08, // r
		0x00, 0x48, 0x54, 0x54, 0x54, 0x20, // s
		0x00, 0x04, 0x3F, 0x44, 0x40, 0x20, // t
		0x00, 0x3C, 0x40, 0x40, 0x20, 0x7C, // u
		0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C, // v
		0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C, // w
		0x00, 0x44, 0x28, 0x10, 0x28, 0x44, // x
		0x00, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C, // y
		0x00, 0x44, 0x64, 0x54, 0x4C, 0x44, // z
		0x14, 0x14, 0x14, 0x14, 0x14, 0x14, // horiz lines
		};
const unsigned char F8x16[][16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, // 0
		0x00, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33,
		0x30, 0x00, 0x00,
		0x00, //!1
		0x00, 0x10, 0x0C, 0x06, 0x10, 0x0C, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00,
		0x00, //"2
		0x40, 0xC0, 0x78, 0x40, 0xC0, 0x78, 0x40, 0x00, 0x04, 0x3F, 0x04, 0x04,
		0x3F, 0x04, 0x04,
		0x00, //#3
		0x00, 0x70, 0x88, 0xFC, 0x08, 0x30, 0x00, 0x00, 0x00, 0x18, 0x20, 0xFF,
		0x21, 0x1E, 0x00,
		0x00, //$4
		0xF0, 0x08, 0xF0, 0x00, 0xE0, 0x18, 0x00, 0x00, 0x00, 0x21, 0x1C, 0x03,
		0x1E, 0x21, 0x1E,
		0x00, //%5
		0x00, 0xF0, 0x08, 0x88, 0x70, 0x00, 0x00, 0x00, 0x1E, 0x21, 0x23, 0x24,
		0x19, 0x27, 0x21,
		0x10, //&6
		0x10, 0x16, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00,
		0x00, //'7
		0x00, 0x00, 0x00, 0xE0, 0x18, 0x04, 0x02, 0x00, 0x00, 0x00, 0x00, 0x07,
		0x18, 0x20, 0x40,
		0x00, //(8
		0x00, 0x02, 0x04, 0x18, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x40, 0x20, 0x18,
		0x07, 0x00, 0x00,
		0x00, //)9
		0x40, 0x40, 0x80, 0xF0, 0x80, 0x40, 0x40, 0x00, 0x02, 0x02, 0x01, 0x0F,
		0x01, 0x02, 0x02,
		0x00, //*10
		0x00, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x1F,
		0x01, 0x01, 0x01,
		0x00, //+11
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xB0, 0x70, 0x00,
		0x00, 0x00, 0x00,
		0x00, //,12
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01,
		0x01, 0x01, 0x01,
		0x01, //-13
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x00,
		0x00, 0x00, 0x00,
		0x00, //.14
		0x00, 0x00, 0x00, 0x00, 0x80, 0x60, 0x18, 0x04, 0x00, 0x60, 0x18, 0x06,
		0x01, 0x00, 0x00,
		0x00, ///15
		0x00, 0xE0, 0x10, 0x08, 0x08, 0x10, 0xE0, 0x00, 0x00, 0x0F, 0x10, 0x20,
		0x20, 0x10, 0x0F,
		0x00, //016
		0x00, 0x10, 0x10, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x20, 0x3F,
		0x20, 0x20, 0x00,
		0x00, //117
		0x00, 0x70, 0x08, 0x08, 0x08, 0x88, 0x70, 0x00, 0x00, 0x30, 0x28, 0x24,
		0x22, 0x21, 0x30,
		0x00, //218
		0x00, 0x30, 0x08, 0x88, 0x88, 0x48, 0x30, 0x00, 0x00, 0x18, 0x20, 0x20,
		0x20, 0x11, 0x0E,
		0x00, //319
		0x00, 0x00, 0xC0, 0x20, 0x10, 0xF8, 0x00, 0x00, 0x00, 0x07, 0x04, 0x24,
		0x24, 0x3F, 0x24,
		0x00, //420
		0x00, 0xF8, 0x08, 0x88, 0x88, 0x08, 0x08, 0x00, 0x00, 0x19, 0x21, 0x20,
		0x20, 0x11, 0x0E,
		0x00, //521
		0x00, 0xE0, 0x10, 0x88, 0x88, 0x18, 0x00, 0x00, 0x00, 0x0F, 0x11, 0x20,
		0x20, 0x11, 0x0E,
		0x00, //622
		0x00, 0x38, 0x08, 0x08, 0xC8, 0x38, 0x08, 0x00, 0x00, 0x00, 0x00, 0x3F,
		0x00, 0x00, 0x00,
		0x00, //723
		0x00, 0x70, 0x88, 0x08, 0x08, 0x88, 0x70, 0x00, 0x00, 0x1C, 0x22, 0x21,
		0x21, 0x22, 0x1C,
		0x00, //824
		0x00, 0xE0, 0x10, 0x08, 0x08, 0x10, 0xE0, 0x00, 0x00, 0x00, 0x31, 0x22,
		0x22, 0x11, 0x0F,
		0x00, //925
		0x00, 0x00, 0x00, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30,
		0x30, 0x00, 0x00,
		0x00, //:26
		0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x60,
		0x00, 0x00, 0x00,
		0x00, //;27
		0x00, 0x00, 0x80, 0x40, 0x20, 0x10, 0x08, 0x00, 0x00, 0x01, 0x02, 0x04,
		0x08, 0x10, 0x20,
		0x00, //<28
		0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x00, 0x04, 0x04, 0x04, 0x04,
		0x04, 0x04, 0x04,
		0x00, //=29
		0x00, 0x08, 0x10, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, 0x20, 0x10, 0x08,
		0x04, 0x02, 0x01,
		0x00, //>30
		0x00, 0x70, 0x48, 0x08, 0x08, 0x08, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x30,
		0x36, 0x01, 0x00,
		0x00, //?31
		0xC0, 0x30, 0xC8, 0x28, 0xE8, 0x10, 0xE0, 0x00, 0x07, 0x18, 0x27, 0x24,
		0x23, 0x14, 0x0B,
		0x00, //@32
		0x00, 0x00, 0xC0, 0x38, 0xE0, 0x00, 0x00, 0x00, 0x20, 0x3C, 0x23, 0x02,
		0x02, 0x27, 0x38,
		0x20, //A33
		0x08, 0xF8, 0x88, 0x88, 0x88, 0x70, 0x00, 0x00, 0x20, 0x3F, 0x20, 0x20,
		0x20, 0x11, 0x0E,
		0x00, //B34
		0xC0, 0x30, 0x08, 0x08, 0x08, 0x08, 0x38, 0x00, 0x07, 0x18, 0x20, 0x20,
		0x20, 0x10, 0x08,
		0x00, //C35
		0x08, 0xF8, 0x08, 0x08, 0x08, 0x10, 0xE0, 0x00, 0x20, 0x3F, 0x20, 0x20,
		0x20, 0x10, 0x0F,
		0x00, //D36
		0x08, 0xF8, 0x88, 0x88, 0xE8, 0x08, 0x10, 0x00, 0x20, 0x3F, 0x20, 0x20,
		0x23, 0x20, 0x18,
		0x00, //E37
		0x08, 0xF8, 0x88, 0x88, 0xE8, 0x08, 0x10, 0x00, 0x20, 0x3F, 0x20, 0x00,
		0x03, 0x00, 0x00,
		0x00, //F38
		0xC0, 0x30, 0x08, 0x08, 0x08, 0x38, 0x00, 0x00, 0x07, 0x18, 0x20, 0x20,
		0x22, 0x1E, 0x02,
		0x00, //G39
		0x08, 0xF8, 0x08, 0x00, 0x00, 0x08, 0xF8, 0x08, 0x20, 0x3F, 0x21, 0x01,
		0x01, 0x21, 0x3F,
		0x20, //H40
		0x00, 0x08, 0x08, 0xF8, 0x08, 0x08, 0x00, 0x00, 0x00, 0x20, 0x20, 0x3F,
		0x20, 0x20, 0x00,
		0x00, //I41
		0x00, 0x00, 0x08, 0x08, 0xF8, 0x08, 0x08, 0x00, 0xC0, 0x80, 0x80, 0x80,
		0x7F, 0x00, 0x00,
		0x00, //J42
		0x08, 0xF8, 0x88, 0xC0, 0x28, 0x18, 0x08, 0x00, 0x20, 0x3F, 0x20, 0x01,
		0x26, 0x38, 0x20,
		0x00, //K43
		0x08, 0xF8, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x3F, 0x20, 0x20,
		0x20, 0x20, 0x30,
		0x00, //L44
		0x08, 0xF8, 0xF8, 0x00, 0xF8, 0xF8, 0x08, 0x00, 0x20, 0x3F, 0x00, 0x3F,
		0x00, 0x3F, 0x20,
		0x00, //M45
		0x08, 0xF8, 0x30, 0xC0, 0x00, 0x08, 0xF8, 0x08, 0x20, 0x3F, 0x20, 0x00,
		0x07, 0x18, 0x3F,
		0x00, //N46
		0xE0, 0x10, 0x08, 0x08, 0x08, 0x10, 0xE0, 0x00, 0x0F, 0x10, 0x20, 0x20,
		0x20, 0x10, 0x0F,
		0x00, //O47
		0x08, 0xF8, 0x08, 0x08, 0x08, 0x08, 0xF0, 0x00, 0x20, 0x3F, 0x21, 0x01,
		0x01, 0x01, 0x00,
		0x00, //P48
		0xE0, 0x10, 0x08, 0x08, 0x08, 0x10, 0xE0, 0x00, 0x0F, 0x18, 0x24, 0x24,
		0x38, 0x50, 0x4F,
		0x00, //Q49
		0x08, 0xF8, 0x88, 0x88, 0x88, 0x88, 0x70, 0x00, 0x20, 0x3F, 0x20, 0x00,
		0x03, 0x0C, 0x30,
		0x20, //R50
		0x00, 0x70, 0x88, 0x08, 0x08, 0x08, 0x38, 0x00, 0x00, 0x38, 0x20, 0x21,
		0x21, 0x22, 0x1C,
		0x00, //S51
		0x18, 0x08, 0x08, 0xF8, 0x08, 0x08, 0x18, 0x00, 0x00, 0x00, 0x20, 0x3F,
		0x20, 0x00, 0x00,
		0x00, //T52
		0x08, 0xF8, 0x08, 0x00, 0x00, 0x08, 0xF8, 0x08, 0x00, 0x1F, 0x20, 0x20,
		0x20, 0x20, 0x1F,
		0x00, //U53
		0x08, 0x78, 0x88, 0x00, 0x00, 0xC8, 0x38, 0x08, 0x00, 0x00, 0x07, 0x38,
		0x0E, 0x01, 0x00,
		0x00, //V54
		0xF8, 0x08, 0x00, 0xF8, 0x00, 0x08, 0xF8, 0x00, 0x03, 0x3C, 0x07, 0x00,
		0x07, 0x3C, 0x03,
		0x00, //W55
		0x08, 0x18, 0x68, 0x80, 0x80, 0x68, 0x18, 0x08, 0x20, 0x30, 0x2C, 0x03,
		0x03, 0x2C, 0x30,
		0x20, //X56
		0x08, 0x38, 0xC8, 0x00, 0xC8, 0x38, 0x08, 0x00, 0x00, 0x00, 0x20, 0x3F,
		0x20, 0x00, 0x00,
		0x00, //Y57
		0x10, 0x08, 0x08, 0x08, 0xC8, 0x38, 0x08, 0x00, 0x20, 0x38, 0x26, 0x21,
		0x20, 0x20, 0x18,
		0x00, //Z58
		0x00, 0x00, 0x00, 0xFE, 0x02, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x7F,
		0x40, 0x40, 0x40,
		0x00, //[59
		0x00, 0x0C, 0x30, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x06, 0x38, 0xC0,
		0x00, //\60
		0x00, 0x02, 0x02, 0x02, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x40,
		0x7F, 0x00, 0x00,
		0x00, //]61
		0x00, 0x00, 0x04, 0x02, 0x02, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00,
		0x00, //^62
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80,
		0x80, 0x80, 0x80,
		0x80, //_63
		0x00, 0x02, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00,
		0x00, //`64
		0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x19, 0x24, 0x22,
		0x22, 0x22, 0x3F,
		0x20, //a65
		0x08, 0xF8, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x11, 0x20,
		0x20, 0x11, 0x0E,
		0x00, //b66
		0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x0E, 0x11, 0x20,
		0x20, 0x20, 0x11,
		0x00, //c67
		0x00, 0x00, 0x00, 0x80, 0x80, 0x88, 0xF8, 0x00, 0x00, 0x0E, 0x11, 0x20,
		0x20, 0x10, 0x3F,
		0x20, //d68
		0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x1F, 0x22, 0x22,
		0x22, 0x22, 0x13,
		0x00, //e69
		0x00, 0x80, 0x80, 0xF0, 0x88, 0x88, 0x88, 0x18, 0x00, 0x20, 0x20, 0x3F,
		0x20, 0x20, 0x00,
		0x00, //f70
		0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x6B, 0x94, 0x94,
		0x94, 0x93, 0x60,
		0x00, //g71
		0x08, 0xF8, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0x20, 0x3F, 0x21, 0x00,
		0x00, 0x20, 0x3F,
		0x20, //h72
		0x00, 0x80, 0x98, 0x98, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x20, 0x3F,
		0x20, 0x20, 0x00,
		0x00, //i73
		0x00, 0x00, 0x00, 0x80, 0x98, 0x98, 0x00, 0x00, 0x00, 0xC0, 0x80, 0x80,
		0x80, 0x7F, 0x00,
		0x00, //j74
		0x08, 0xF8, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0x20, 0x3F, 0x24, 0x02,
		0x2D, 0x30, 0x20,
		0x00, //k75
		0x00, 0x08, 0x08, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x20, 0x3F,
		0x20, 0x20, 0x00,
		0x00, //l76
		0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x20, 0x3F, 0x20, 0x00,
		0x3F, 0x20, 0x00,
		0x3F, //m77
		0x80, 0x80, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0x20, 0x3F, 0x21, 0x00,
		0x00, 0x20, 0x3F,
		0x20, //n78
		0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x1F, 0x20, 0x20,
		0x20, 0x20, 0x1F,
		0x00, //o79
		0x80, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00, 0x80, 0xFF, 0xA1, 0x20,
		0x20, 0x11, 0x0E,
		0x00, //p80
		0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x0E, 0x11, 0x20,
		0x20, 0xA0, 0xFF,
		0x80, //q81
		0x80, 0x80, 0x80, 0x00, 0x80, 0x80, 0x80, 0x00, 0x20, 0x20, 0x3F, 0x21,
		0x20, 0x00, 0x01,
		0x00, //r82
		0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x33, 0x24, 0x24,
		0x24, 0x24, 0x19,
		0x00, //s83
		0x00, 0x80, 0x80, 0xE0, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F,
		0x20, 0x20, 0x00,
		0x00, //t84
		0x80, 0x80, 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x1F, 0x20, 0x20,
		0x20, 0x10, 0x3F,
		0x20, //u85
		0x80, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0x01, 0x0E, 0x30,
		0x08, 0x06, 0x01,
		0x00, //v86
		0x80, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x80, 0x0F, 0x30, 0x0C, 0x03,
		0x0C, 0x30, 0x0F,
		0x00, //w87
		0x00, 0x80, 0x80, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0x20, 0x31, 0x2E,
		0x0E, 0x31, 0x20,
		0x00, //x88
		0x80, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x81, 0x8E, 0x70,
		0x18, 0x06, 0x01,
		0x00, //y89
		0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x21, 0x30, 0x2C,
		0x22, 0x21, 0x30,
		0x00, //z90
		0x00, 0x00, 0x00, 0x00, 0x80, 0x7C, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x3F, 0x40,
		0x40, //{91
		0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0xFF, 0x00, 0x00,
		0x00, //|92
		0x00, 0x02, 0x02, 0x7C, 0x80, 0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x3F,
		0x00, 0x00, 0x00,
		0x00, //}93
		0x00, 0x06, 0x01, 0x01, 0x02, 0x02, 0x04, 0x04, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, //~94
		};
const unsigned char F16x16[][32] = { { 0x00, 0x00, 0x18, 0x16, 0x10, 0xD0, 0xB8,
		0x97, 0x90, 0x90, 0x90, 0x92, 0x94, 0x10, 0x00, 0x00, 0x00, 0x20, 0x10,
		0x8C, 0x83, 0x80, 0x41, 0x46, 0x28, 0x10, 0x28, 0x44, 0x43, 0x80, 0x80,
		0x00 },/*"��",0*/

{ 0x80, 0x40, 0x30, 0x1E, 0x10, 0x10, 0x10, 0xFF, 0x10, 0x10, 0x10, 0x10, 0x10,
		0x10, 0x00, 0x00, 0x40, 0x40, 0x42, 0x42, 0x42, 0x42, 0x42, 0x7F, 0x42,
		0x42, 0x42, 0x42, 0x42, 0x40, 0x40, 0x00 },/*"��",1*/

{ 0x00, 0xF8, 0x0C, 0x0B, 0x08, 0x08, 0xF8, 0x40, 0x30, 0x8F, 0x08, 0x08, 0x08,
		0xF8, 0x00, 0x00, 0x00, 0x7F, 0x21, 0x21, 0x21, 0x21, 0x7F, 0x00, 0x00,
		0x00, 0x43, 0x80, 0x40, 0x3F, 0x00, 0x00 },/*"��",2*/

{ 0x20, 0x24, 0x24, 0xA4, 0xFE, 0x23, 0x22, 0x20, 0xFC, 0x04, 0x04, 0x04, 0x04,
		0xFC, 0x00, 0x00, 0x10, 0x08, 0x06, 0x01, 0xFF, 0x01, 0x06, 0x80, 0x63,
		0x19, 0x01, 0x01, 0x09, 0x33, 0xC0, 0x00 },/*"��",3*/

{ 0x80, 0x40, 0x20, 0x90, 0x88, 0x86, 0x80, 0x80, 0x80, 0x83, 0x8C, 0x10, 0x20,
		0x40, 0x80, 0x00, 0x00, 0x80, 0x40, 0x20, 0x18, 0x07, 0x00, 0x40, 0x80,
		0x40, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00 },/*"��",4*/

{ 0x10, 0x10, 0x10, 0xFF, 0x90, 0x20, 0x10, 0xE8, 0x27, 0x24, 0xE4, 0x34, 0x2C,
		0xE0, 0x00, 0x00, 0x02, 0x42, 0x81, 0x7F, 0x00, 0x84, 0x84, 0x47, 0x24,
		0x1C, 0x07, 0x1C, 0x24, 0x47, 0x84, 0x00 },/*"��",5*/
{ 0x00, 0x00, 0x18, 0x16, 0x10, 0xD0, 0xB8, 0x97, 0x90, 0x90, 0x90, 0x92, 0x94,
		0x10, 0x00, 0x00, 0x00, 0x20, 0x10, 0x8C, 0x83, 0x80, 0x41, 0x46, 0x28,
		0x10, 0x28, 0x44, 0x43, 0x80, 0x80, 0x00 },/*"��",0*/

{ 0x80, 0x40, 0x30, 0x1E, 0x10, 0x10, 0x10, 0xFF, 0x10, 0x10, 0x10, 0x10, 0x10,
		0x10, 0x00, 0x00, 0x40, 0x40, 0x42, 0x42, 0x42, 0x42, 0x42, 0x7F, 0x42,
		0x42, 0x42, 0x42, 0x42, 0x40, 0x40, 0x00 },/*"��",1*/

{ 0x00, 0xF8, 0x0C, 0x0B, 0x08, 0x08, 0xF8, 0x40, 0x30, 0x8F, 0x08, 0x08, 0x08,
		0xF8, 0x00, 0x00, 0x00, 0x7F, 0x21, 0x21, 0x21, 0x21, 0x7F, 0x00, 0x00,
		0x00, 0x43, 0x80, 0x40, 0x3F, 0x00, 0x00 },/*"��",2*/

{ 0x20, 0x24, 0x24, 0xA4, 0xFE, 0x23, 0x22, 0x20, 0xFC, 0x04, 0x04, 0x04, 0x04,
		0xFC, 0x00, 0x00, 0x10, 0x08, 0x06, 0x01, 0xFF, 0x01, 0x06, 0x80, 0x63,
		0x19, 0x01, 0x01, 0x09, 0x33, 0xC0, 0x00 },/*"��",3*/

{ 0x80, 0x40, 0x20, 0x90, 0x88, 0x86, 0x80, 0x80, 0x80, 0x83, 0x8C, 0x10, 0x20,
		0x40, 0x80, 0x00, 0x00, 0x80, 0x40, 0x20, 0x18, 0x07, 0x00, 0x40, 0x80,
		0x40, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00 },/*"��",4*/

{ 0x10, 0x10, 0x10, 0xFF, 0x90, 0x20, 0x10, 0xE8, 0x27, 0x24, 0xE4, 0x34, 0x2C,
		0xE0, 0x00, 0x00, 0x02, 0x42, 0x81, 0x7F, 0x00, 0x84, 0x84, 0x47, 0x24,
		0x1C, 0x07, 0x1C, 0x24, 0x47, 0x84, 0x00 }, /*"��",5*/
};

void OLED_GPIO_Init() {
	EALLOW;
	//!!!!!�ܱ�����Ҫ����
	//DC
	GpioCtrlRegs.GPAPUD.bit.GPIO14 = GPIO_PULLUP;
	GpioCtrlRegs.GPADIR.bit.GPIO14 = GPIO_OUTPUT;
	GpioCtrlRegs.GPAGMUX1.bit.GPIO14 = GPIO_MUX_CPU1;
	//Rst
	GpioCtrlRegs.GPAPUD.bit.GPIO15 = GPIO_PULLUP;
	GpioCtrlRegs.GPADIR.bit.GPIO15 = GPIO_OUTPUT;
	GpioCtrlRegs.GPAGMUX1.bit.GPIO15 = GPIO_MUX_CPU1;
	EDIS;
}

void OLED_Init() {
	OLED_GPIO_Init();

	GpioDataRegs.GPACLEAR.bit.GPIO15 = 1;
	DELAY_US(10000);
	GpioDataRegs.GPASET.bit.GPIO15 = 1;
	DELAY_US(10000);
	//���ϵ絽���濪ʼ��ʼ��Ҫ���㹻��ʱ�䣬���ȴ�RC��λ���
	OLED_Write(0xae, Cmd_Mode);
	OLED_Write(0xae, Cmd_Mode); //--turn off oled panel
	OLED_Write(0x00, Cmd_Mode); //---set low column address
	OLED_Write(0x10, Cmd_Mode); //---set high column address
	OLED_Write(0x40, Cmd_Mode); //--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_Write(0x81, Cmd_Mode); //--set contrast control register
	OLED_Write(0xcf, Cmd_Mode); // Set SEG Output Current Brightness
	OLED_Write(0xa1, Cmd_Mode); //--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1��
	OLED_Write(0xc8, Cmd_Mode); //Set COM/Row Scan Direction   0xc0���·��� 0xc8��
	OLED_Write(0xa6, Cmd_Mode); //--set normal display
	OLED_Write(0xa8, Cmd_Mode); //--set multiplex ratio(1 to 64)
	OLED_Write(0x3f, Cmd_Mode); //--1/64 duty
	OLED_Write(0xd3, Cmd_Mode); //-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_Write(0x00, Cmd_Mode); //-not offset
	OLED_Write(0xd5, Cmd_Mode); //--set display clock divide ratio/oscillator frequency
	OLED_Write(0x80, Cmd_Mode); //--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_Write(0xd9, Cmd_Mode); //--set pre-charge period
	OLED_Write(0xf1, Cmd_Mode); //Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_Write(0xda, Cmd_Mode); //--set com pins hardware configuration
	OLED_Write(0x12, Cmd_Mode);
	OLED_Write(0xdb, Cmd_Mode); //--set vcomh
	OLED_Write(0x40, Cmd_Mode); //Set VCOM Deselect Level
	OLED_Write(0x20, Cmd_Mode); //-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_Write(0x02, Cmd_Mode); //
	OLED_Write(0x8d, Cmd_Mode); //--set Charge Pump enable/disable
	OLED_Write(0x14, Cmd_Mode); //--set(0x10) disable
	OLED_Write(0xa4, Cmd_Mode); // Disable Entire Display On (0xa4/0xa5)
	OLED_Write(0xa6, Cmd_Mode); // Disable Inverse Display On (0xa6/a7)
	OLED_Write(0xaf, Cmd_Mode); //--turn on oled panel
	OLED_Fill(0x00); //��ʼ����
	OLED_Set_pos(0, 0);
}

void OLED_Write(unsigned char data, WriteMode mode) {

	if (mode == Cmd_Mode) {
		if (GpioDataRegs.GPADAT.bit.GPIO14 == 1) {
			DELAY_US(1);
			GpioDataRegs.GPACLEAR.bit.GPIO14 = 1;
			DELAY_US(1);
		}
	} else {
		if (GpioDataRegs.GPADAT.bit.GPIO14 == 0) {
			DELAY_US(1);
			GpioDataRegs.GPASET.bit.GPIO14 = 1;
			DELAY_US(1);
		}
	}
	SPI_transfer(data);
}
void OLED_Set_pos(unsigned char x, unsigned char y) {
	OLED_Write(0xb0 + y, Cmd_Mode);
	OLED_Write(((x & 0xf0) >> 4) | 0x10, Cmd_Mode);
	OLED_Write((x & 0x0f) | 0x00, Cmd_Mode);
}

void OLED_Fill(unsigned char bmp_dat) {
	unsigned char y, x;
	for (y = 0; y < 8; y++) {
		OLED_Set_pos(0, y);
		for (x = 0; x < 128; x++)
			OLED_Write(bmp_dat, Data_Mode);
	}

}

void OLED_Print_str(unsigned char x, unsigned char y, char* str, CharMode mode) {
	while (x <= (128 - (128 % mode) - mode) && *str != '\0') //�������һλ��дλ && ѭ������ַ�
	{
		OLED_Print_c(x, y, *str, mode);
		x += mode;
		str++;
	}
}

void OLED_Print_num(unsigned char x, unsigned char y, long num, CharMode mode) {
	char str[20];
	sprintf(str, "%ld", num);
	OLED_Print_str(x, y, str, mode);
}

void OLED_Print_lf(unsigned char x, unsigned char y, double lf,
		unsigned char ndigit, CharMode mode) {
	char str[20];
	char str1[10];
	sprintf(str, "%d", (int) lf);
	sprintf(str1, "%d", (int) ((lf - (int) lf) * pow(10, ndigit)));
	strcat(strcat(str, "."), str1);
	OLED_Print_str(x, y, str, mode);
}

void OLED_Print_c(unsigned char x, unsigned char y, char c, CharMode mode) {
	unsigned char i = 0;
	OLED_Set_pos(x, y);
	switch (mode) {
	case C6x8:
		for (i = 0; i < 6; i++) {
			OLED_Write(F6x8[c - 32][i], Data_Mode);
		}
		break;
	case C8x16:
		for (i = 0; i < 8; i++) {
			OLED_Write(F8x16[c - 32][i], Data_Mode);
		}
		OLED_Set_pos(x, y + 1);
		for (i = 8; i < 16; i++) {
			OLED_Write(F8x16[c - 32][i], Data_Mode);
		}
		break;
	case C16x16:
		for (i = 0; i < 16; i++) {
			OLED_Write(F16x16[c - 32][i], Data_Mode);
		}
		OLED_Set_pos(x, y + 1);
		for (i = 16; i < 32; i++) {
			OLED_Write(F16x16[c - 32][i], Data_Mode);
		}
		break;
	}
}

