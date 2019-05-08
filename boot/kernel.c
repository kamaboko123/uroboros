#include "graphic.h"

void Main(void){
    init_palette();
    init_screen(0);
    
    //white
    //put_font_asc(0, 0, 7, 'H');
    print_asc(0, 0, 7, "Hello, World! [Uroboros]");
    
    for(;;){
        io_hlt();
    }
}
