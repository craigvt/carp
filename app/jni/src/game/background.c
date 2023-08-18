#include "background.h"
#include "engine/render.h"
#include "game.h"

static struct Background background;

void background_init_play(void)
{     
        background.play_bg.texture = IMG_LoadTexture(render_get_renderer(), "play_bg.png");
        background.texture = IMG_LoadTexture(render_get_renderer(), "bg_atlas.png");

        float scale = render_get_scale();
        SDL_Rect bounds;
        SDL_GetDisplayBounds(0, &bounds);

        background.play_bg.src = (SDL_Rect){0, 0, PLAYBG_W, PLAYBG_H};
        background.play_bg.dst = (SDL_Rect){0, 0, PLAYBG_W * scale, PLAYBG_H * scale};

        background.school1.src   = (SDL_Rect){0, 0, SCHOOL_W, SCHOOL_H};
        background.school1.dst   = (SDL_Rect){0 - SCHOOL_W * scale, 48 * scale, SCHOOL_W * scale, SCHOOL_H * scale};
        background.school1.speed = rand() % (2 + 1 - 1) + 1;

        background.school2.src   = (SDL_Rect){0, SCHOOL_H, SCHOOL_W, SCHOOL_H};
        background.school2.dst   = (SDL_Rect){bounds.w, 128 * scale, SCHOOL_W * scale, SCHOOL_H * scale};
        background.school2.speed = rand() % (2 + 1 - 1) + 1;

        background.fish_group1.src   = (SDL_Rect){0, 96, GROUP_W, GROUP_H};
        background.fish_group1.dst   = (SDL_Rect){0 - GROUP_W * scale, 112 * scale, GROUP_W * scale, GROUP_H * scale};
        background.fish_group1.speed = rand() % (3 + 1 - 1) + 1;

        background.fish_group2.src   = (SDL_Rect){0, 112, GROUP_W, GROUP_H};
        background.fish_group2.dst   = (SDL_Rect){bounds.w, 192 * scale, GROUP_W * scale, GROUP_H * scale};
        background.fish_group2.speed = rand() % (3 + 1 - 1) + 1;

        background.fish_solo1.src   = (SDL_Rect){64, 96, SOLO_W, SOLO_H};
        background.fish_solo1.dst   = (SDL_Rect){0 - SOLO_W * scale, 96 * scale, SOLO_W * scale, SOLO_H * scale};
        background.fish_solo1.speed = rand() % (4 + 1 - 1) + 1;

        background.fish_solo2.src   = (SDL_Rect){64, 96, SOLO_W, SOLO_H};
        background.fish_solo2.dst   = (SDL_Rect){0 - SOLO_W * scale, 176 * scale, SOLO_W * scale, SOLO_H * scale};
        background.fish_solo2.speed = rand() % (4 + 1 - 1) + 1;

        background.fish_solo3.src   = (SDL_Rect){64, 112, SOLO_W, SOLO_H};
        background.fish_solo3.dst   = (SDL_Rect){bounds.w, 208 * scale, SOLO_W * scale, SOLO_H * scale};
        background.fish_solo3.speed = rand() % (4 + 1 - 1) + 1;
}

void background_update_play(void)
{
        float scale = render_get_scale();

        SDL_Rect bounds;
        SDL_GetDisplayBounds(0, &bounds);

        background.school1.dst.x = background.school1.dst.x + background.school1.speed;
        if (background.school1.dst.x > bounds.w) {
                background.school1.dst.x = 0 - SCHOOL_W * scale;
        } 

        background.school2.dst.x = background.school2.dst.x - background.school2.speed;      
        if (background.school2.dst.x < 0 - SCHOOL_W * scale) {
                background.school2.dst.x = bounds.w;
        } 

        background.fish_group1.dst.x = background.fish_group1.dst.x + background.fish_group1.speed;
        if (background.fish_group1.dst.x > bounds.w) {
                background.fish_group1.dst.x = 0 - GROUP_W * scale;
                background.fish_group1.speed = rand() % (2 + 1 - 1) + 1;
        }

        background.fish_group2.dst.x = background.fish_group2.dst.x - background.fish_group2.speed;
        if (background.fish_group2.dst.x < 0 - GROUP_W * scale) {
                background.fish_group2.dst.x = bounds.w;
                background.fish_group2.speed = rand() % (2 + 1 - 1) + 1;
        }

        background.fish_solo1.dst.x = background.fish_solo1.dst.x + background.fish_solo1.speed;
        if (background.fish_solo1.dst.x > bounds.w) {
                background.fish_solo1.dst.x = 0 - SOLO_W * scale;
                background.fish_solo1.speed = rand() % (3 + 1 - 1) + 1;
        }

        background.fish_solo2.dst.x = background.fish_solo2.dst.x + background.fish_solo2.speed;
        if (background.fish_solo2.dst.x > bounds.w) {
                background.fish_solo2.dst.x = 0 - SOLO_W * scale;
                background.fish_solo2.speed = rand() % (3 + 1 - 1) + 1;
        }

        background.fish_solo3.dst.x = background.fish_solo3.dst.x - background.fish_solo3.speed;
        if (background.fish_solo3.dst.x < 0 - SOLO_W * scale) {
                background.fish_solo3.dst.x = bounds.w;
                background.fish_solo3.speed = rand() % (3 + 1 - 1) + 1;
        }

        int key_frame = game_get_keyframe();

        if (key_frame == 1) {
                background.school1.src.x = 0;
                background.school2.src.x = 0;
                background.fish_group1.src.x = 0;
                background.fish_group2.src.x = 0;
                background.fish_solo1.src.x  = 64;
                background.fish_solo2.src.x  = 64;
                background.fish_solo3.src.x  = 64;
        }

        if (key_frame == 2) {
                background.school1.src.x = SCHOOL_W;
                background.school2.src.x = SCHOOL_W;
                background.fish_group1.src.x = GROUP_W;
                background.fish_group2.src.x = GROUP_W;
                background.fish_solo1.src.x  = 64 + SOLO_W;
                background.fish_solo2.src.x  = 64 + SOLO_W;
                background.fish_solo3.src.x  = 64 + SOLO_W;
        }     

        render_draw(background.play_bg.texture, &background.play_bg.src, &background.play_bg.dst);
        render_draw(background.texture, &background.school1.dst, &background.school1.dst);
        render_draw(background.texture, &background.school2.src, &background.school2.dst);
        render_draw(background.texture, &background.fish_group1.src, &background.fish_group1.dst);
        render_draw(background.texture, &background.fish_group2.src, &background.fish_group2.dst);
        render_draw(background.texture, &background.fish_solo1.src, &background.fish_solo1.dst);
        render_draw(background.texture, &background.fish_solo2.src, &background.fish_solo2.dst);
        render_draw(background.texture, &background.fish_solo3.src, &background.fish_solo3.dst);                         
}

void background_render_play(void)
{
        render_draw(background.play_bg.texture, &background.play_bg.src, &background.play_bg.dst);
        render_draw(background.texture, &background.school1.dst, &background.school1.dst);
        render_draw(background.texture, &background.school2.src, &background.school2.dst);
        render_draw(background.texture, &background.fish_group1.src, &background.fish_group1.dst);
        render_draw(background.texture, &background.fish_group2.src, &background.fish_group2.dst);
        render_draw(background.texture, &background.fish_solo1.src, &background.fish_solo1.dst);
        render_draw(background.texture, &background.fish_solo2.src, &background.fish_solo2.dst);
        render_draw(background.texture, &background.fish_solo3.src, &background.fish_solo3.dst);
}

void background_destroy_play(void)
{
        SDL_DestroyTexture(background.play_bg.texture);
        SDL_DestroyTexture(background.texture);
}
