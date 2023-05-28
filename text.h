// battleships
// A simple text api to be used with a libnds projec
// requires a 32x32 tile mode background.
// update 255th bg_palette for main and sub to change colour.
// default colour white.
// currently uses text tileset in text_and_shots.h 
// in the future this tile set will be suppliable in init.
// Tharen and Rafael

#ifndef TEXT_H
#define TEXT_H

#include <nds.h>

#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 192
#define MAX_TEXTS 10


#define TEXT_HEIGHT 8
#define TEXT_WIDTH 8
typedef enum text_errors {NONE, MEM_ERR, INDEX_OUT_OF_BOUNDS, INVALID_PARAM } text_errors;


// initialise text api with tile mode setup map and tile pointers for main and sub engines.
// palette offsets should first available pal index, assumes all following are available. 
int init_text_api(u16 * map_main, u8* tile_main, u16 *map_sub, u8* tile_sub);

/*
    adds new text obj to datastructure and returns its id.
    returns -text_error if error.
*/
int new_text(char * string, int x, int y, int is_main);

/*
    changes the text displayed for text object "id" and its position.
    give negative position if want same position.

    returns 0 on success;
*/
int update_text(int id, char * string, int x, int y);



#endif