/*
 * This file is a part of AFUIOT, Which is a free software project, Developed by
 * ShenZhen AFU Smart Internet Technology Co.,Ltd(AFU company).
 * AFUIOT, The new start of IoT, get news on www.afuiot.com.
 *
 * Copyright © 2019 [copyright AFU company].
 * @license (MIT license)
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction,including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:The above copyright
 * notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * @Author: liangjia
 * @Date: 2019-02-27 10:25:20
 * @Last Modified by: liangjia
 * @Last Modified time: 2019-02-27 10:26:54
 */

/* Includes ------------------------------------------------------------------*/
#include "prjlib.h"

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static CmdEntrance_t* firstEnt = NULL;
static stTaskWatchDog* pTWDGEnter = NULL;

/* Public variables ----------------------------------------------------------*/
const uint16_t crc16tab[256] = {
    0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241, 0xC601,
    0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440, 0xCC01, 0x0CC0,
    0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40, 0x0A00, 0xCAC1, 0xCB81,
    0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841, 0xD801, 0x18C0, 0x1980, 0xD941,
    0x1B00, 0xDBC1, 0xDA81, 0x1A40, 0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01,
    0x1DC0, 0x1C80, 0xDC41, 0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0,
    0x1680, 0xD641, 0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081,
    0x1040, 0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
    0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441, 0x3C00,
    0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41, 0xFA01, 0x3AC0,
    0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840, 0x2800, 0xE8C1, 0xE981,
    0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41, 0xEE01, 0x2EC0, 0x2F80, 0xEF41,
    0x2D00, 0xEDC1, 0xEC81, 0x2C40, 0xE401, 0x24C0, 0x2580, 0xE541, 0x2700,
    0xE7C1, 0xE681, 0x2640, 0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0,
    0x2080, 0xE041, 0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281,
    0x6240, 0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
    0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41, 0xAA01,
    0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840, 0x7800, 0xB8C1,
    0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41, 0xBE01, 0x7EC0, 0x7F80,
    0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40, 0xB401, 0x74C0, 0x7580, 0xB541,
    0x7700, 0xB7C1, 0xB681, 0x7640, 0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101,
    0x71C0, 0x7080, 0xB041, 0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0,
    0x5280, 0x9241, 0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481,
    0x5440, 0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
    0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841, 0x8801,
    0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40, 0x4E00, 0x8EC1,
    0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41, 0x4400, 0x84C1, 0x8581,
    0x4540, 0x8701, 0x47C0, 0x4680, 0x8641, 0x8201, 0x42C0, 0x4380, 0x8341,
    0x4100, 0x81C1, 0x8081, 0x4040};

const uint32_t crc32tab[] =
    {/* CRC polynomial 0xedb88320 */
     0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f,
     0xe963a535, 0x9e6495a3, 0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
     0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2,
     0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
     0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9,
     0xfa0f3d63, 0x8d080df5, 0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
     0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b, 0x35b5a8fa, 0x42b2986c,
     0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
     0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423,
     0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
     0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d, 0x76dc4190, 0x01db7106,
     0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
     0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d,
     0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
     0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
     0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
     0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7,
     0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
     0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa,
     0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
     0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81,
     0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
     0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84,
     0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
     0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb,
     0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
     0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e,
     0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
     0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55,
     0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
     0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28,
     0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
     0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f,
     0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
     0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242,
     0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
     0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69,
     0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
     0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc,
     0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
     0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693,
     0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
     0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d};

/* Private function prototypes -----------------------------------------------*/

/* function definition -------------------------------------------------------*/

/***
 * Description : ??????????��??????????.
 * Arguments   : Dst     ???????????.
 *               Src     ??????????????.
 *               lDst    ??????�A??.
 *               lData   ?????????????
 * Returns     : ?????????????????????,??????????????.
 * Caller      : Application.
 * Notes       : None.
 *******************************************************************************
 */
uint8_t* SearchMemData(uint8_t* Dst, uint8_t* Dat, uint16_t lDst,
                       uint16_t lDat) {
  int16_t i, n;

  if (Dst == NULL || Dat == NULL || lDst == 0 || lDat == 0) return NULL;

  i = 0;
  while (i <= (lDst - lDat)) {
    for (n = 0; *(Dst + i + n) == *(Dat + n); n++) {
      if ((n + 1) == lDat) {
        return (Dst + i);
      }
    }
    i++;
  }
  return NULL;
}

/***
 * Description : ??????????????.
 * Arguments   : s     ????????????????.
 * Returns     : ??????????????.
 *******************************************************************************
 */
uint32_t strlen_t(char* s) {
  uint32_t len = 0;

  if (s != NULL) len = strlen(s);
  return len;
}

/***
 * Description : ??????????????????.
 * Arguments   : val     ???????????.
 *               str     ??????????????????????.
 * Returns     : ?????????????.
 * Caller      : Application.
 * Notes       : None.
 *******************************************************************************
 */
char* uitoa(uint32_t val, char* str) {
  uint32_t power, j;
  char* p = NULL;

  if (str != NULL) {
    p = str;
    j = val;
    for (power = 1; j >= 10; j /= 10) {
      power *= 10;
    }
    for (; power > 0; power /= 10) {
      *p++ = '0' + val / power;
      val %= power;
    }
    *p = '\0';
    p = str;
  }
  return p;
}

/***
 * Description : ?????????????16?????????.
 * Arguments   : val     ???????????.
 *               str     ??????????????????????.
 * Returns     : ?????????????.
 * Caller      : Application.
 * Notes       : None.
 *******************************************************************************
 */
char* uitoax(uint32_t val, char* str) {
  uint32_t power, i, j;
  char* p = NULL;

  if (str != NULL) {
    p = str;
    j = val;
    i = 1;
    for (power = 1; j >= 16; j /= 16) {
      power *= 16;
      i++;
    }
    if (i % 2) {
      *p++ = '0';
    }
    for (; power > 0; power /= 16) {
      i = val / power;
      *p++ = (i > 9) ? i - 10 + 'A' : i + '0';
      val %= power;
    }
    *p = '\0';
    p = str;
  }
  return p;
}

/***
 * Description : ????????????????.
 * Arguments   : str     -??????????????????????.
 * Returns     : ????????????.
 * Caller      : Application.
 * Notes       : none.
 *******************************************************************************
 */
uint32_t uatoi(char* str) {
  uint32_t r = 0;

  if (str != NULL) {
    while (isdigit(*str)) {
      r = r * 10 + (*str - 0x30);
      str++;
    }
  }
  return r;
}

/***
 * Description : ??????????????.
 *******************************************************************************
 */
uint32_t uatoi_n(char* str, uint8_t n) {
  uint32_t r = 0;

  if (str != NULL) {
    while (isdigit(*str) && n > 0) {
      r = r * 10 + (*str - 0x30);
      str++;
      n--;
    }
  }
  return r;
}

/***
 * Description : ??????????��?????.
 * Arguments   : str     -??????????????????????.
 * Returns     : ????????????.
 * Caller      : Application.
 * Notes       : none.
 *******************************************************************************
 */
int32_t satoi(char* str) {
  int32_t r = 0;

  if (str != NULL) {
    if (*str == '-') {
      str++;
      while (isdigit(*str)) {
        r = r * 10 + (*str - 0x30);
        str++;
      }
      r = -r;
    } else if (*str == '+') {
      str++;
      while (isdigit(*str)) {
        r = r * 10 + (*str - 0x30);
        str++;
      }
    } else {
      while (isdigit(*str)) {
        r = r * 10 + (*str - 0x30);
        str++;
      }
    }
  }
  return r;
}

/***
 * Description : ??16??????????????????????.
 * Arguments   : str     -??????????????????????.
 * Returns     : ????????????.
 * Caller      : Application.
 *******************************************************************************
 */
uint32_t uatoix(char* str) {
  uint32_t n, r = 0;

  if (str == NULL) return 0;

  if (*str == '0' && (*(str + 1) == 'x' || *(str + 1) == 'X')) str += 2;
  while (*str) {
    if (isdigit(*str))
      n = *str - '0';
    else if (*str >= 'A' && *str <= 'F')
      n = *str - 'A' + 10;
    else if (*str >= 'a' && *str <= 'f')
      n = *str - 'a' + 10;
    else
      break;
    r = r * 16 + n;
    str++;
  }
  return r;
}

/***
 * Description : ??16?????????????64��?????????.
 * Arguments   : str     -??????????????????????.
 * Returns     : ????????????.
 * Caller      : Application.
 *******************************************************************************
 */
uint64_t ulatoix(char* str) {
  uint64_t n, r = 0;

  if (str == NULL) return 0;

  if (*str == '0' && (*(str + 1) == 'x' || *(str + 1) == 'X')) str += 2;
  while (*str) {
    if (isdigit(*str))
      n = *str - '0';
    else if (*str >= 'A' && *str <= 'F')
      n = *str - 'A' + 10;
    else if (*str >= 'a' && *str <= 'f')
      n = *str - 'a' + 10;
    else
      break;
    r = r * 16 + n;
    str++;
  }
  return r;
}

/***
 * Description : ??????????????.
 *******************************************************************************
 */
uint32_t uatoix_n(char* str, uint8_t n) {
  uint32_t d, r = 0;

  if (str == NULL) return 0;

  if (*str == '0' && (*(str + 1) == 'x' || *(str + 1) == 'X')) str += 2;
  while (*str && n > 0) {
    if (isdigit(*str))
      d = *str - '0';
    else if (*str >= 'A' && *str <= 'F')
      d = *str - 'A' + 10;
    else if (*str >= 'a' && *str <= 'f')
      d = *str - 'a' + 10;
    else
      break;
    r = r * 16 + d;
    str++;
    n--;
  }
  return r;
}

/***
 * Description : ????????????????��??????????????��????0.
 * Arguments   : val     ?????????.
 *               str     ????????????.
 *               dig     ???????��??
 * Returns     : ????????????.
 * Caller      : Application.
 * Notes       : ???????��????��?????????.
 *******************************************************************************
 */
char* uitoa_d(uint32_t val, char* str, uint8_t d) {
  char* p = NULL;
  char buf[12];
  uint8_t i = 0;

  p = uitoa(val, buf);
  if (p != NULL && str != NULL) {
    i = strlen_t(buf);
    if (i >= d) {
      i = i - d;
      strcpy(str, buf + i);
    } else {
      i = d - i;
      memset(str, '0', i);
      str[i] = 0;
      strcat(str, buf);
    }
    p = str;
  }
  return p;
}

/***
 * Description : ????????????????��??????????????��???????.
 * Arguments   : val     ?????????.
 *               str     ????????????.
 *               dig     ???????��??
 * Returns     : ????????????.
 * Caller      : Application.
 * Notes       : ???????��????��??????????.
 *******************************************************************************
 */
char* uitoa_n(uint32_t val, char* str, uint8_t d) {
  char* p = NULL;
  char buf[12];
  uint8_t i = 0;

  p = uitoa(val, buf);
  if (p != NULL && str != NULL) {
    i = strlen_t(buf);
    if (i >= d) {
      i = i - d;
      strcpy(str, buf + i);
    } else {
      i = d - i;
      memset(str, ' ', i);
      str[i] = 0;
      strcat(str, buf);
    }
    p = str;
  }
  return p;
}

/***
 * Description : ?? ?��??????????????.
 * Arguments   : val     ???????????.
 *               str     ??????????????????????.
 * Returns     : ?????????????.
 * Caller      : Application.
 * Notes       : ???????????.
 *******************************************************************************
 */
char* sitoa(int32_t val, char* str) {
  char* p = NULL;

  if (str != NULL) {
    if (val >= 0) {
      *str = '+';
    } else {
      *str = '-';
      val = -val;
    }
    uitoa(val, str + 1);
    p = str;
  }
  return p;
}

/***
 * Description : ???��????????????��??????????????��???????.
 * Arguments   : val     ?????????.
 *               str     ????????????.
 *               dig     ???????��,????????.
 * Returns     : ????????????.
 * Caller      : Application.
 * Notes       : ???????��????��??????????.
 *******************************************************************************
 */
char* sitoa_n(int32_t val, char* str, uint8_t d) {
  char* p = NULL;
  char buf[12];
  uint8_t i = 0;

  p = sitoa(val, buf);
  if (p != NULL && str != NULL) {
    i = strlen_t(buf);
    if (i >= d) {
      i = i - d;
      strcpy(str, buf + i);
    } else {
      i = d - i;
      memset(str, ' ', i);
      str[i] = 0;
      strcat(str, buf);
    }
    p = str;
  }
  return p;
}

/***
 * Description : 16��???? ??��????.
 * Arguments   : num        ????????????.
 * Returns     : ??????????????.
 * Caller      : Application.
 * Notes       : none.
 *******************************************************************************
 */
uint16_t int16tBS(uint16_t* num) {
  uint16_t i = *num;
  char* p1 = (char*)&i;
  char* p2 = (char*)num;

  *p2 = *(p1 + 1);
  *(p2 + 1) = *p1;
  return *num;
}

/***
 * Description : 32��???? ??��????.
 * Arguments   : num        ????????????.
 * Returns     : ??????????????.
 * Caller      : Application.
 * Notes       : none.
 *******************************************************************************
 */
uint32_t int32tBS(uint32_t* num) {
  uint32_t i = *num;
  char* p1 = (char*)&i;
  char* p2 = (char*)num;

  *p2 = *(p1 + 3);
  *(p2 + 1) = *(p1 + 2);
  *(p2 + 2) = *(p1 + 1);
  *(p2 + 3) = *(p1);
  return *num;
}

/***
 * Description : 64��???? ??��????.
 * Arguments   : num        ????????????.
 * Returns     : ??????????????.
 * Caller      : Application.
 * Notes       : none.
 *******************************************************************************
 */
uint64_t int64tBS(uint64_t* num) {
  uint64_t i = *num;
  char* p1 = (char*)&i;
  char* p2 = (char*)num;

  *p2 = *(p1 + 7);
  *(p2 + 1) = *(p1 + 6);
  *(p2 + 2) = *(p1 + 5);
  *(p2 + 3) = *(p1 + 4);
  *(p2 + 4) = *(p1 + 3);
  *(p2 + 5) = *(p1 + 2);
  *(p2 + 6) = *(p1 + 1);
  *(p2 + 7) = *p1;
  return *num;
}

/***
 * Description : ????????��???��??.
 * Arguments   : p_start        ????????????.
 *               len            ??��??????????
 * Returns     : ????��????.
 * Caller      : Application.
 * Notes       : none.
 *******************************************************************************
 */
uint8_t XORCheck(uint8_t* p_start, uint16_t len) {
  uint16_t i;
  uint8_t Return_XORCheck = 0;

  if (p_start != NULL) {
    Return_XORCheck = *p_start++;
    for (i = 0; i < len - 1; i++) {
      Return_XORCheck = Return_XORCheck ^ (*p_start);
      p_start++;
    }
  }
  return Return_XORCheck;
}

/***
 * Description : ???????????��??.
 * Arguments   : p_start        ????????????.
 *               len            ??��??????????
 * Returns     : ????��????.
 * Caller      : Application.
 *******************************************************************************
 */
uint8_t AddCheck(uint8_t* p_start, uint16_t len) {
  uint8_t r = 0;
  uint16_t i = 0;

  if (p_start != NULL) {
    r = *p_start++;
    for (i = 0; i < len - 1; i++) {
      r = r + (*p_start);
      p_start++;
    }
  }
  return r;
}

/***
 * Description : ???��????.
 *******************************************************************************
 */
BOOL InitQueue(CirQueue_TypeDef* queue, char* buf, uint16_t len) {
  if (buf != NULL && len > 0) {
    queue->lenth = len;
    queue->pbuf = buf;
    queue->front = 0;
    queue->rear = 0;
    queue->count = 0;
    return TRUE;
  }
  return FALSE;
}

/***
 * Description : ???.
 *******************************************************************************
 */
BOOL EnQueue(CirQueue_TypeDef* queue, char* buff, uint16_t length) {
  uint16_t rem = 0, len = 0;

  len = length;
  rem = queue->lenth - queue->count;
  if (len <= rem && len > 0) {
    rem = queue->lenth - queue->rear;
    if (rem > len) rem = len;
    memcpy(queue->pbuf + queue->rear, buff, rem);
    len -= rem;
    if (len > 0) {
      memcpy(queue->pbuf, buff + rem, len);
      queue->rear = len;
    } else {
      queue->rear += rem;
    }
    queue->count += length;
    return TRUE;
  }
  return FALSE;
}

/***
 * Description : ????.
 *******************************************************************************
 */
BOOL DeQueue(CirQueue_TypeDef* queue, char* buff, uint16_t length) {
  uint16_t rem = 0, len = 0;

  len = length;
  rem = queue->count;
  if (len <= rem && len > 0) {
    rem = queue->lenth - queue->front;
    if (rem > len) rem = len;
    memcpy(buff, queue->pbuf + queue->front, rem);
    len -= rem;
    if (len > 0) {
      memcpy(buff + rem, queue->pbuf, len);
      queue->front = len;
    } else {
      queue->front += rem;
    }
    queue->count -= length;
    return TRUE;
  }
  return FALSE;
}

/***
 * Arguments   : aData��??????, aSize��?�A??
 * Notes       : 16��CRC��?�D??;
 ********************************************************************************
 */
uint16_t CRC_16(uint16_t init, uint8_t* aData, uint16_t aSize) {
  uint8_t uchCRCHi = 0xff;
  uint8_t uchCRCLo = 0xff;
  uint16_t uindex;
  while (aSize--) {
    uindex = uchCRCHi ^ *aData++;
    uchCRCHi = uchCRCLo ^ (uint8_t)crc16tab[uindex];
    uchCRCLo = crc16tab[uindex] >> 8;
  }
  return (uchCRCLo << 8 | uchCRCHi);
}

/***
 * Arguments   : CRC16��??2
 ********************************************************************************
 */
uint16_t crc16_compute(const uint8_t* p_data, uint32_t size,
                       const uint16_t* p_crc) {
  uint32_t i;
  uint16_t crc = (p_crc == NULL) ? 0xffff : *p_crc;

  for (i = 0; i < size; i++) {
    crc = (unsigned char)(crc >> 8) | (crc << 8);
    crc ^= p_data[i];
    crc ^= (unsigned char)(crc & 0xff) >> 4;
    crc ^= (crc << 8) << 4;
    crc ^= ((crc & 0xff) << 4) << 1;
  }

  return crc;
}

/***
 * Arguments   : aData��??????, aSize��?�A??
 * Notes       : 32��CRC��?�D??;
 ********************************************************************************
 */
uint32_t CRC_32(uint32_t init, uint8_t* aData, uint16_t aSize) {
  uint16_t i;
  uint32_t nAccum = init;

  for (i = 0; i < aSize; i++)
    nAccum = crc32tab[((uint32_t)nAccum ^ *aData++) & 0xff] ^
             ((nAccum >> 8) & 0x00FFFFFF);
  return nAccum;
}

/***
 * Description : BCD???????????
 * Arguments   : bcd    -bcd?
 * Returns     : ???????????
 *******************************************************************************
 */
uint8_t BCD_To_HEX(uint8_t bcd) {
  uint8_t temp = 0;

  temp = ((bcd & 0xF0) >> 4) * 10;
  temp += bcd & 0x0F;
  return temp;
}

/***
 * Description : ???????????BCD
 * Arguments   : hex    -hex?
 * Returns     : ???????????
 *******************************************************************************
 */
uint8_t HEX_To_BCD(uint8_t hex) {
  uint8_t temp = 0;

  temp = (hex / 10) << 4;
  temp |= hex % 10;
  return temp;
}

/***
 * Description : BCD?????????
 * Arguments   : bcd    -bcd????????
 *               len    -BCD????????
 * Returns     : ???????????
 *******************************************************************************
 */
uint32_t BCDStr_To_Dec(char* bcd, uint8_t len) {
  uint8_t temp = 0, i = 0;
  uint32_t r = 0, j = 1;

  for (i = len; i > 0; i--) {
    temp = bcd[i - 1] - (bcd[i - 1] >> 4) * 6;
    r += temp * j;
    j *= 100;
  }
  return r;
}

/***
 * Description : BCD??????.
 ********************************************************************************
 */
char* BCDStr_To_Str(char* bcd, uint16_t len, char* str) {
  uint8_t temp = 0;
  char* p = str;
  uint16_t ct;

  for (ct = 0; ct < len; ct++) {
    temp = *(bcd + ct);
    *p++ = ((temp >> 4) & 0x0f) + '0';
    *p++ = (temp & 0x0f) + '0';
  }
  *p = '\0';
  return str;
}

/***
 * Description : ??????BCD.
 * Arguments   : dec ?????????
 * Caller      : bcd bcd???????
 *******************************************************************************
 */
char* Dec_To_BCDStr(uint32_t dec, uint8_t len, char* bcd) {
  int i = 0;
  uint8_t temp = 0;

  for (i = len - 1; i >= 0; i--) {
    temp = dec % 100;
    bcd[i] = temp + temp / 10 * 6;
    dec /= 100;
  }
  return bcd;
}

/***
 * Description : ????????????.
 *******************************************************************************
 */
char* Str2Print(char* str) {
  char* p = str;

  while (*p) {
    if (isprint(*p) == 0) {
      break;
    }
    p++;
  }
  *p = '\0';
  return str;
}

/***
 * Description : ?????????????.
 *******************************************************************************
 */
char* Str2Graph(char* str) {
  char* p = str;

  while (*p) {
    if (isgraph(*p) == 0) {
      break;
    }
    p++;
  }
  *p = '\0';
  return str;
}

/***
 * Description : ?????????????.
 *******************************************************************************
 */
char* Str2Alpha(char* str) {
  char* p = str;

  while (*p) {
    if (isalpha(*p) == 0) {
      break;
    }
    p++;
  }
  *p = '\0';
  return str;
}

/***
 * Description : ?????????��?token.
 *******************************************************************************
 */
char* StrTokenDel(char* str, char token) {
  char* p = str;

  while (*p) {
    if (*p == token) {
      break;
    }
    p++;
  }
  *p = '\0';
  return str;
}

/***
 * Description : ?????????????.
 *******************************************************************************
 */
BOOL Cmd_AddEntrance(CmdEntrance_t* pEnt) {
  BOOL r = FALSE;
  CmdEntrance_t* p = NULL;

  if (pEnt != NULL && pEnt->cmd != NULL) {
    if (firstEnt == NULL) {
      firstEnt = pEnt;
    } else {
      p = firstEnt;
      while (p->next != NULL) {
        p = p->next;
      }
      p->next = pEnt;
    }
    r = TRUE;
  }
  return r;
}

/***
 * Description : ??????.
 *******************************************************************************
 */
void Cmd_Handle(char* cmd) {
  int argc = 0;
  int flag = 1;
  char *argv[CMD_ARGC_MAX + 1], *p = NULL;
  CmdEntrance_t* pEnt = firstEnt;

  for (argc = 0; argc < CMD_ARGC_MAX; argc++) {
    argv[argc] = NULL;
  }
  while (*cmd && !isgraph(*cmd)) {
    cmd++;
  }
  argc = 1;
  argv[0] = cmd;
  p = cmd;
  /*???????????*/
  while (*p && argc < CMD_ARGC_MAX && flag) {
    //		while (*p && isgraph(*p)) {
    //			p++;
    //		}
    //		while (*p && !isgraph(*p)) {
    //			*p = 0;
    //			p++;
    //		}
    while (*p && *p != ' ') {
      p++;
    }

    if (*p && *p == ' ') {
      *p = 0;
      p++;
    }
    if (*p) {
      argv[argc] = p;
      if (*p == ' ') {
        flag = 0;
      }
      argc++;
    }
  }
  argv[argc] = NULL;
  printf("cmd:%s argv[1]:%d  argc:%d ", cmd, *(argv[1]), argc);
  /*???*/
  while (pEnt != NULL) {
    if (argc > 0 && strcmp(cmd, pEnt->cmd) == 0) {
      pEnt->pCB(argc, argv);
      break;
    }
    pEnt = pEnt->next;
  }
}

/***
 * Description : ??????��?????????????.
 *******************************************************************************
 */
int Cmd_ArgFind(char** argv, char* arg) {
  int ret = -1;
  uint16_t i = 0;

  while (argv[i] != NULL) {
    if (STR_EQUAL(argv[i], arg)) {
      ret = i;
      break;
    }
    i++;
  }
  return ret;
}

/**
 * @brief  ?????.
 * @param  de: ?????????.
 * @retval none.
 */
void delay(uint32_t de) {
  volatile uint32_t i = 0;

  while (i < de) {
    i++;
  }
}

/***
 * Description : ?????????????
 *******************************************************************************
 */
void TaskWDG_Add(stTaskWatchDog* pTWDG) {
  stTaskWatchDog* p;

  if (pTWDGEnter == NULL) {
    pTWDGEnter = pTWDG;
  } else {
    p = pTWDGEnter;
    while (p != pTWDG && p->pNext != NULL) {
      p = p->pNext;
    }
    if (p != pTWDG) {
      pTWDG->pNext = NULL;
      p->pNext = pTWDG;
    }
  }
}

/***
 * Description : ????????????
 *******************************************************************************
 */
void TaskWDG_OnOff(stTaskWatchDog* pTWDG, BOOL en) { pTWDG->Enable = en; }

/***
 * Description : ???????????
 *******************************************************************************
 */
void TaskWDG_Clear(stTaskWatchDog* pTWDG) { pTWDG->TaskCount = 0; }

/***
 * Description : ????????????????, ????????????????????????????
 *******************************************************************************
 */
char* TaskWDG_IsUpper(void) {
  char* r = NULL;
  stTaskWatchDog* p;

  p = pTWDGEnter;
  while (p != NULL) {
    if (p->TaskCount >= p->TaskUpper) {
      r = p->Name;
      break;
    }
    p = p->pNext;
  }
  return r;
}

/**
 * Description : ?????????????
 * @return ?????????????????
 */
char* TaskWDG_Tick(void) {
  char* r = NULL;
  stTaskWatchDog* p;

  p = pTWDGEnter;
  while (p != NULL) {
    if (p->Enable) {
      p->TaskCount++;
      //  printf("TaskCount:%d \r\n",p->TaskCount);
      //  printf("TaskCount:%d \r\n",p->TaskUpper);
      if (p->TaskCount >= p->TaskUpper) {
        r = p->Name;
        break;
      }
    }
    p = p->pNext;
  }
  return r;
}

/**
 * FIFO ?????
 * @param p_fifo   FIFO????
 * @param p_buf    FIFO????????
 * @param buf_size FIFO??????��
 * @return ???????????TRUE
 */
BOOL FIFO_Init(FIFO_t* p_fifo, uint8_t* p_buf, uint32_t buf_size) {
  if (p_buf != NULL && IS_POWER_OF_TWO(buf_size)) {
    p_fifo->pBuffer = p_buf;
    p_fifo->sizeMask = buf_size - 1;
    p_fifo->rpos = 0;
    p_fifo->wpos = 0;
    return TRUE;
  }
  return FALSE;
}

/**
 * FIFO???
 * @param p_fifo FIFO????
 * @param byte   ��????
 * @return ?????????TRUE
 */
BOOL FIFO_Put(FIFO_t* p_fifo, uint8_t byte) {
  if (FIFO_Length(p_fifo) <= p_fifo->sizeMask) {
    p_fifo->pBuffer[p_fifo->wpos & p_fifo->sizeMask] = byte;
    p_fifo->wpos++;
    return TRUE;
  }
  return FALSE;
}

/**
 * FIFO????????
 * @param p_fifo FIFO????
 * @param byte   ???????
 * @return ?????????TRUE
 */
BOOL FIFO_Get(FIFO_t* p_fifo, uint8_t* p_byte) {
  if (FIFO_Length(p_fifo) > 0) {
    *p_byte = p_fifo->pBuffer[p_fifo->rpos & p_fifo->sizeMask];
    p_fifo->rpos++;
    return TRUE;
  }
  return FALSE;
}

/**
 * FIFO????????
 * @param p_fifo FIFO????
 * @return ????FIFO?????
 */
uint32_t FIFO_Length(FIFO_t* p_fifo) {
  uint32_t tmp = p_fifo->rpos;

  return (p_fifo->wpos - tmp);
}

/**
 * FIFO???
 * @param p_fifo FIFO????
 */
void FIFO_Flush(FIFO_t* p_fifo) {
  uint32_t tmp = p_fifo->wpos;

  p_fifo->rpos = tmp;
}

/**
 * FIFO????????
 * @param p_fifo FIFO????
 * @param pBuf   ????????????
 * @param size   ?????????
 * @return ??????????????
 */
uint32_t FIFO_Read(FIFO_t* p_fifo, uint8_t* pBuf, uint32_t size) {
  uint32_t len = 0, i = 0;

  if (pBuf != NULL && size > 0) {
    len = FIFO_Length(p_fifo);
    if (size > len) {
      size = len;
    }
    while (size > 0) {
      *(pBuf + i) = p_fifo->pBuffer[p_fifo->rpos & p_fifo->sizeMask];
      p_fifo->rpos++;
      size--;
      i++;
    }
  }
  return i;
}

/**
 * FIFO��??????
 * @param p_fifo FIFO????
 * @param pBuf   ??��??????????
 * @param size   ��??????
 * @return ???????��??????
 */
uint32_t FIFO_Write(FIFO_t* p_fifo, uint8_t* pBuf, uint32_t size) {
  uint32_t rem = 0, i = 0;

  if (pBuf != NULL && size > 0) {
    rem = p_fifo->sizeMask - FIFO_Length(p_fifo) + 1;
    if (size > rem) {
      size = rem;
    }
    while (size > 0) {
      p_fifo->pBuffer[p_fifo->wpos & p_fifo->sizeMask] = *(pBuf + i);
      p_fifo->wpos++;
      size--;
      i++;
    }
  }
  return i;
}

/**
 * FIFO???????.
 */
uint8_t FIFO_Query(FIFO_t* p_fifo, uint32_t offset) {
  uint8_t r = 0;

  if (offset < FIFO_Length(p_fifo)) {
    r = p_fifo->pBuffer[(p_fifo->rpos + offset) & p_fifo->sizeMask];
  }
  return r;
}

/**
 * FIFO???????
 * @param p_des ???FIFO
 * @param p_res ?FIFO
 * @param size  ????????
 * @return ???????��??????
 */
uint32_t FIFO_cpy(FIFO_t* p_des, FIFO_t* p_res, uint32_t size) {
  uint8_t temp = 0;
  uint32_t rem = 0, ret = 0, len = 0;
  ;

  if (size > 0) {
    rem = p_des->sizeMask - FIFO_Length(p_des) + 1;
    len = FIFO_Length(p_res);

    if (size > rem) {
      size = rem;
    }
    if (size > len) {
      size = len;
    }
    ret = size;
    while (size > 0) {
      temp = p_res->pBuffer[p_res->rpos & p_res->sizeMask];
      p_res->rpos++;
      p_des->pBuffer[p_des->wpos & p_des->sizeMask] = temp;
      p_des->wpos++;
      size--;
    }
  }
  return ret;
}

/**
 * ??FIFO?��???????
 * @param p_fifo FIFO???
 * @param offset ?????????????
 * @param str    ????????????
 * @return ??????????��????FIFO?��??????,��???????-1
 */
int32_t FIFO_str(FIFO_t* p_fifo, uint32_t offset, char* str) {
  uint8_t temp = 0;
  uint32_t i = 0, j = 0, dlen = 0, slen = 0;

  if (str != NULL) {
    slen = strlen(str);
    dlen = FIFO_Length(p_fifo);

    for (i = 0; i <= dlen - slen; i++) {
      temp = p_fifo->pBuffer[(p_fifo->rpos + offset + i) & p_fifo->sizeMask];
      for (j = 0; temp == *(str + j); j++) {
        if (*(str + j + 1) == 0) {
          return i;
        }
        temp =
            p_fifo->pBuffer[(p_fifo->rpos + offset + i + j) & p_fifo->sizeMask];
      }
    }
  }
  return -1;
}

/**
 * ??FIFO?��???????,???????????/0
 * @param p_fifo FIFO???
 * @param offset ????????????
 * @param str    ???????????
 * @return ?????????0????????-1
 */
int32_t FIFO_cmp(FIFO_t* p_fifo, uint32_t offset, char* str) {
  uint8_t temp = 0;
  uint32_t i = 0, dlen = 0;

  if (str != NULL) {
    dlen = FIFO_Length(p_fifo);

    do {
      temp = p_fifo->pBuffer[(p_fifo->rpos + offset + i) & p_fifo->sizeMask];
      if (temp != *(str + i)) {
        return -1;
      }
      i++;
    } while (i < dlen && *(str + i) != 0);
    return 0;
  }
  return -1;
}

/**
 * ??FIFO?��??????
 * @param p_fifo FIFO???
 * @param offset ????????????
 * @param ch    ??????????
 * @return ??????????��????FIFO?��??????,��???????-1
 */
int32_t FIFO_chr(FIFO_t* p_fifo, uint32_t offset, char ch) {
  uint8_t temp = 0;
  uint32_t i = 0, dlen = 0;

  dlen = FIFO_Length(p_fifo);

  do {
    temp = p_fifo->pBuffer[(p_fifo->rpos + offset + i) & p_fifo->sizeMask];
    if (temp == ch) {
      return i;
    }
    i++;
  } while (i < dlen);

  return -1;
}

/**
 * ?????HEX
 */
void Array2Hex(uint8_t* in, uint8_t* out, uint16_t len) {
  int i = 0;
  uint8_t *p = out, tmp = 0;

  for (i = 0; i < len; i++) {
    tmp = *(in + i) / 16;
    *p++ = (tmp > 9) ? tmp - 10 + 'A' : tmp + '0';
    tmp = *(in + i) % 16;
    *p++ = (tmp > 9) ? tmp - 10 + 'A' : tmp + '0';
    *p++ = ' ';
  }
  *p++ = '\r';
  *p++ = '\n';
  *p++ = '\0';
}

/**
 * ??��????
 * @return ??????TRUE
 */
BOOL IsCPUendianBig(void) {
  union {
    unsigned long i;
    uint8_t s[4];
  } c;

  c.i = 0x12345678;
  return (BOOL)(0x12 == c.s[0]);
}

/***
 *Version history **************************************************************
 *2012.4.25   jonas       Create file.
 *2012.6.7    jonas       ????uitoax BUG
 *2012.8.7    jonas       ???????
 *2012.11.15  jonas       uitoax��????????????0?????
 *2013.3.10   jonas       ??????��??��???.
 *2013.3.20   jonas       ????uatoix.
 *2013.3.22   jonas       ???????��??.
 *2014.4.2    jonas       ????CRC16��??.
 *2015.12.20  jonas       ????strlen_t????.
 *2016.1.2    jonas       ???BCD???????.
 *2016.1.2    jonas       ?????????????
 *2016.1.12   jonas       ?????????��?????????.
 *2016.1.20   jonas       ????CRC16??????.
 *2016.3.25   jonas       ???????????????
 *2016.4.6    jonas       ????FIFO????
 *2016.6.27   jonas       ???FIFO??????????BUG.
 *2016.6.27   jonas       ?????????HEX????.
 *2016.8.11   jonas       ????Str2Alpha????.
 *2016.9.19   jonas       ????FIFO string????.
 *2016.10.10  jonas       ?????��????.
 *2016.10.28  jonas       ????Cmd_ArgFind????

 */

/***************************************************************END OF FILE****/
