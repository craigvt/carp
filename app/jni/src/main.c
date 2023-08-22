#include "system.h"
#include "game.h"

int main(int argc, char* args[])
{
        if (system_init() < 1) {
                game_loop();
        }

        return 0;
}