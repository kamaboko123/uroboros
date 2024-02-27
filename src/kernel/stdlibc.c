#include "stdlibc.h"

uint32_t __last_rand = RAND_INIT;

uint32_t sprintf(char *s, char *format, ...){
    my_va_list args;
    
    char *s_org = s;
    char disp_digit;
    int pad_flg;
    uint32_t conv_len;
    char tmp[256];
    int i;
    
    int data_int;
    char pad_char;
    
    my_va_start(args, format);
    
    while(*format != '\0'){
        pad_flg = false;
        conv_len = 0;
        
        if(*format == '%'){
            format++;
            
            memset(tmp, '\0', sizeof(tmp));
            
            if(*format == '\0') break;
            else if(*format == '%'){
                *s = '%';
                s++;
                goto next;
            }
            else if(isdigit(*format)){
                pad_flg = true;
                
                if(*format == '0'){
                    pad_char = '0';
                }
                else{
                    pad_char = ' ';
                }
                
                disp_digit = atoi(format);
                while(isdigit(*format)) format++;
            }
            
            if(*format == 'd' || *format == 'u'){
                data_int = my_va_arg(args, int);
                if(((data_int & 0x8000) != 0) && *format == 'd'){
                    
                    if(pad_char == ' '){
                        *tmp = '-';
                        conv_len = to_dec_asc(tmp + 1, ~data_int + 1) + 1;
                    }
                    else if(pad_char == '0'){
                        *s = '-';
                        s++;
                        conv_len = to_dec_asc(tmp, ~data_int + 1);
                        disp_digit--;
                    }
                }
                else{
                    conv_len = to_dec_asc(tmp, data_int);
                }
            }
            if(*format == 'x'){
                conv_len = to_hex_asc(tmp, my_va_arg(args, int), false);
            }
            if(*format == 'X'){
                conv_len = to_hex_asc(tmp, my_va_arg(args, int), true);
            }
            if(*format == 's'){
                char *str = my_va_arg(args, char *);
                strncpy(tmp, str, 256);
                conv_len = strlen(tmp);
                if(conv_len > 255){
                    conv_len = 255;
                }
            }
            
            if(pad_flg && (conv_len < disp_digit)){
                for(i = conv_len; i < disp_digit; i++){
                    *s = pad_char;
                    s++;
                }
            }
            
            memcpy(s, tmp, conv_len);
            s += conv_len;
        }
        else{
          *s = *format;
          s++;
        }
        
next:
        format++;
    }
    *s = *format;
    
    my_va_end(args);
    
    return ((s - s_org) / sizeof(char));
}

uint32_t to_dec_asc(char *buf, int n){
    char *p;
    uint32_t ret;
    uint32_t i;
    
    i = ndigit(n, 10);
    ret = i;
    p = buf;
    
    while(i > 0){
        *p = ((n / upow(10, i - 1)) % 10) + '0';
        p++;
        i--;
    }
    return(ret);
}

uint32_t to_hex_asc(char *buf, int n, bool capital){
    char *p;
    uint32_t ret;
    uint32_t i;
    char charset[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    
    i = ndigit(n, 16);
    ret = i;
    p = buf;
    
    while(i > 0){
        *p = charset[((n / upow(16, i - 1)) % 16)];
        
        if(capital && isdigit(*p)){
            *p -= 0x20;
        }
        
        p++;
        i--;
    }
    return(ret);
}


uint32_t ndigit(uint32_t n, uint32_t base){
    uint32_t i = 1;
    
    while(n >= base){
        n /= base;
        i++;
    }
    return(i);
}

uint32_t upow(uint32_t x, uint32_t n){
    if(n == 0) return(1);
    if(n == 1) return(x);
    return(x * upow(x, n-1));
}

bool isdigit(char c){
    return((c >= '0' && c <= '9'));
}

int atoi(char *s){
    int result = 0;
    int sign = false;
    
    //符号付き
    if(*s == '-'){
        sign = true;
        s++;
    }
    
    while(isdigit(*s)){
        //すでに入ってるものを桁上げ + その桁の数値を加算
        result = (result * 10) + (*s - '0');
        s++;
    }
    
    if (sign) result *= -1;
    
    return result;
}

bool iscapital(char c){
    return((c >= 'A' && c <= 'Z'));
}

char *memcpy(char *buf1, char *buf2, int n){
    int i;
    for(i = 0; i < n; i++){
            buf1[i] = buf2[i];
        }
    return buf1;
}

int memset(char *buf, char byte, int n){
    int i;
    for(i = 0; i < n; i++){
        buf[i] = byte;
    }
    return(n);
}

int strcmp(char *s1, char *s2){
    while(*s1 != '\0' && *s2 != '\0'){
        if(*s1 != *s2) break;
        s1++;
        s2++;
    }
    
    return(*s1 - *s2);
}

int strncmp(char *s1, char *s2, uint32_t n){
    uint32_t i = 0;
    while(*s1 != '\0' && *s2 != '\0' && ++i < n){
        if(*s1 != *s2) break;
        s1++;
        s2++;
    }
    
    return(*s1 - *s2);
}

int strlen(char *str){
    int i;
    for(i = 0; str[i] != '\0'; i++);
    return(i);
}

int strtol(char *s, char **endp, int base){
    int _base;
    int ret = 0;
    int sign = 0;
    
    //空白のスキップ
    while(*s == ' ') s++;
    
    if(*s == '-') sign = 1;
    
    //base = 0なら基数は渡された文字列の表記に従う
    if(base == 0){
        //渡された文字列の表記方法の検出
        //16進数
        if(strncmp(s, "0x", 2) == 0 || strncmp(s, "0X", 2) == 0){
            _base = 16;
        }
        //8進数
        if(strncmp(s, "0", 1) == 0){
            _base = 8;
        }
        //それ以外は10進数
        else{
            _base = 10;
        }
    }
    else{
        _base = base;
    }
    
    //16進数
    if(_base == 16){
       while(isdigit(*s) || (*s >= 'a' && *s <= 'f') || (*s >= 'A' && *s <= 'F') ){
            if(isdigit(*s)){
                ret = (ret * 16) + (*s - '0');
            }
            else if(*s >= 'a' && *s <= 'f'){
                ret = (ret * 16) + (*s - 'a' + 10);
            }
            else if(*s >= 'A' && *s <= 'F'){
                ret = (ret * 16) + (*s - 'A' + 10);
            }
            s++;
        }
    }
    
    //8進数（多分使わないので省略）
    
    //10進数
    if(_base == 10){
        while(isdigit(*s)){
            ret = (ret * 10) + (*s - '0');
            s++;
        }
    }
    
    if(sign != 0) ret *= -1;
    *endp = s;
    return ret;
}

void rand_seed(uint32_t x){
    __last_rand = x;
}

uint32_t rand(){
    //線形合同法による乱数生成
    //ポケモン3,4世代と同じ値
    static uint32_t a = 0x41c64e6d;
    static uint32_t b = 0x6073;
    __last_rand = (a * __last_rand + b) % (_UINT_MAX / 2);
    return __last_rand;
}

uint32_t roundup(uint32_t x, uint32_t n){
    uint32_t m = x % n;
    if(m == 0){
        return x;
    }
    else{
        return (n * (x / n)) + n;
    }
}

uint32_t rounddown(uint32_t x, uint32_t n){
    return (x / n) * n;
}


char *strncpy(char *s1, char *s2, int n){
    bool end = false;
    int i;
    for(i = 0; i < n; i++){
        if(end){
            s1[i] = '\0';
            continue;
        }
        if(s2[i] == '\0'){
            s1[i] = '\0';
            end = true;
        }
        s1[i] = s2[i];
    }
    return(s1);
}

