#include "engine/system.h"
#include "game/game.h"

int main(int argc, char* args[])
{
        LOGI("------------------ begin debug -----------------------------");
        if (system_init() < 1) {
                game_loop();
        }

        return 0;
}