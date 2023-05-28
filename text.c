// battleships
// A simple text api to be used with a libnds projec
// requires a 32x32 tile mode background.
// update 255th bg_palette for main and sub to change colour.
// default colour white.
// currently uses text tileset in text_and_shots.h 
// in the future this tile set will be suppliable in init.
// Tharen and Rafael

#include "text.h"
#include "text_and_shots.h"
#include "stdio.h"
#include "string.h"
#include "ctype.h"


typedef struct text_obj {
    uint8 x;
    uint8 y;
    uint8 len;
    uint8 is_main;
} text_obj;

uint8 text_count;
text_obj text_arr[MAX_TEXTS];


u16 * map_ptr_main;
u8 * tile_ptr_main;
u16 * map_ptr_sub;
u8 * tile_ptr_sub;

u16 * pal_main;
u16 * pal_sub;
int pal_count;

const int maxpal = 255;

#define ASCII_A 97
#define ASCII_Z 122
#define ASCII_0 48
#define ASCII_9 57
#define DIGIT_OFF 26
#define BLANK 37
#define MAP_DIM 32
#define MAP_IDX(y,x) (y*MAP_DIM + x)
/*  
    simply displays nothing for special characters.
    note: x and y are screen coordinates, they will be rounded to a multiple of 8.
*/
void insert_str(u16 * map, char * string, int len, int x, int y) {
    x /= 8;
    y /= 8;
    int i;
    for (i = 0; i < len; i++) {
        char c = (char) tolower(string[i]);
        if (c >= ASCII_A && c <= ASCII_Z)
            c -= ASCII_A;
        else if (c >= ASCII_0 && c <= ASCII_9)
            c = c - ASCII_0 + DIGIT_OFF;
        else
            c = (char) BLANK;
        
        map[MAP_IDX(y,x+i)] = (u16)c;
    }   

}

/*
    clears portion of text map, x and y are screen coordinates. 
*/
void clear_map_portion(u16* map, int x, int y, int len) {
    x/= 8;
    y/= 8;
    int i;
    for (i = 0; i < len; i ++) {
        map[MAP_IDX(y,x+i)] = (u16)BLANK;
    }
}

/*
    text api assumes 32x32 tiling for main and sub engines. 
    has to receive pointers for both engines.
*/
int init_text_api(u16 * map_main, u8* tile_main, u16 *map_sub, u8* tile_sub) {
    if (map_main == NULL || tile_main == NULL || map_sub == NULL || tile_sub == NULL)
        return -1;
    map_ptr_main = map_main;
    tile_ptr_main = tile_main;
    map_ptr_sub = map_sub;
    tile_ptr_sub = tile_sub;
    
    BG_PALETTE[TEXT_PAL_IDX] = ARGB16(1, 31, 31, 31);
    BG_PALETTE_SUB[TEXT_PAL_IDX] = ARGB16(1, 31, 31, 31);

    // pal_main[0] = ARGB16(1, 0, 31, 31);
    // pal_sub[0] = ARGB16(1, 0, 31, 31);

    int i;
    dmaCopy(textTiles, tile_ptr_main, TEXT_LEN);
    dmaCopy(textTiles, tile_ptr_sub, TEXT_LEN);
    int j;
    for (i = 0; i < MAP_DIM; i++) {
        for (j = 0; j < MAP_DIM; j++) {
            map_main[MAP_IDX(i,j)] = (u16)BLANK;
            map_sub[MAP_IDX(i,j)] = (u16)BLANK;
        }
    }
    return 0;
};
/*
    adds new text obj to datastructure and returns its id.
    returns -text_error if error. 

    set text to "" if it needs to be hidden.
*/
int new_text(char * string, int x, int y, int is_main) {
    text_count ++;
    if (text_count >= MAX_TEXTS)
        return -MEM_ERR;

    int len = strlen(string);
   
    if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT || x + (len*8) >= SCREEN_WIDTH || x < 0 || y < 0)
        return -INVALID_PARAM;

    if (!(is_main == 0 || is_main == 1)) 
        return -INVALID_PARAM;

    if (string == NULL)
        return -INVALID_PARAM;
    text_obj *obj = text_arr+text_count;
    u16* map = map_ptr_main;
    if (!is_main) { 
        map = map_ptr_sub;
    }
    obj->x = x;
    obj->y = y;
    obj->is_main = is_main;
    obj->len = len;

    insert_str(map, string, len, x, y);
    return text_count;
}



int update_text(int id, char * string, int x, int y) {
   
    if (string == NULL)
        return -INVALID_PARAM;
    if (id < 0 || id > text_count)
        return -INDEX_OUT_OF_BOUNDS;
        
    int len = strlen(string);
    if (x < 0 || y < 0) {
        x = text_arr[id].x;
        y = text_arr[id].y;
    }
        
    if (text_arr[id].x + (len*8) >= SCREEN_WIDTH || x < 0 || y < 0 || y >= SCREEN_HEIGHT)
    return -INVALID_PARAM;

 
    u16* map = map_ptr_main;
    if (!text_arr[id].is_main)
        map = map_ptr_sub;
    clear_map_portion(map, text_arr[id].x, text_arr[id].y, text_arr[id].len);
    text_arr[id].len = len;
    text_arr[id].x = x;
    text_arr[id].y = y;
    insert_str(map, string, len, x ,y);
    return 0;
}




