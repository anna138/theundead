//Credits Screen
//

#include "fonts.h"

void showCredits(Rect r)
{

    ggprint8b(&r, 16, 0x00ff0000, "Credits of this Game is By:");
    ggprint8b(&r, 16, 0x00ff0000, "Kevin Mitra");
    ggprint8b(&r, 16, 0x00ff0000, "Gerardo Martinez");
    ggprint8b(&r, 16, 0x00ff0000, "Manvir Bal");
    ggprint8b(&r, 16, 0x00ff0000, "Anna Poon");
}
