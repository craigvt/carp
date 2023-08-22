#include "background.h"
#include "render.h"
#include "game.h"

static struct Background background;

void background_init(void)
{          
        float scale = render_get_scale();
        SDL_Rect bounds;
        SDL_GetDisplayBounds(0, &bounds);

        if (game_get_state() == LOGO) {
                background.logo_bg.src = (SDL_Rect){bounds.x, bounds.y, bounds.w, bounds.h};
        }

        if (game_get_state() == PLAY || game_get_state() == TITLE) {

                if (game_get_state() == TITLE) {
                        background.bg.texture = IMG_LoadTexture(render_get_renderer(), "title_bg.png");
                        background.texture = IMG_LoadTexture(render_get_renderer(), "bg_atlas.png");
                }

                if (game_get_state() == PLAY) {
                        background.bg.texture = IMG_LoadTexture(render_get_renderer(), "play_bg.png");
                        background.texture = IMG_LoadTexture(render_get_renderer(), "bg_atlas.png");
                }

                background.bg.src = (SDL_Rect){0, 0, PLAYBG_W, PLAYBG_H};
                background.bg.dst = (SDL_Rect){- render_get_offset(), 0, PLAYBG_W * scale, PLAYBG_H * scale};

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

        if (game_get_state() == GAMEOVER) {
                background.bg.texture = IMG_LoadTexture(render_get_renderer(), "title_bg.png");
        }
}

void background_update(void)
{
        if (game_get_state() == PLAY || game_get_state() == TITLE) {
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

                if (game_get_keyframe() == 1) {
                        background.school1.src.x = 0;
                        background.school2.src.x = 0;
                        background.fish_group1.src.x = 0;
                        background.fish_group2.src.x = 0;
                        background.fish_solo1.src.x  = 64;
                        background.fish_solo2.src.x  = 64;
                        background.fish_solo3.src.x  = 64;
                }

                if (game_get_keyframe() == 2) {
                        background.school1.src.x = SCHOOL_W;
                        background.school2.src.x = SCHOOL_W;
                        background.fish_group1.src.x = GROUP_W;
                        background.fish_group2.src.x = GROUP_W;
                        background.fish_solo1.src.x  = 64 + SOLO_W;
                        background.fish_solo2.src.x  = 64 + SOLO_W;
                        background.fish_solo3.src.x  = 64 + SOLO_W;
                }
        }                            
}

void background_render(void)
{       
        if (game_get_state() == LOGO) {
                SDL_SetRenderDrawColor(render_get_renderer(), 68, 102, 166, 255);
                SDL_RenderFillRect(render_get_renderer(), &background.logo_bg.src);
        }

        if (game_get_state() == PLAY || game_get_state() == TITLE) {
                SDL_RenderCopy(render_get_renderer(), background.bg.texture, &background.bg.src, &background.bg.dst);
                SDL_RenderCopy(render_get_renderer(), background.texture, &background.school1.dst, &background.school1.dst);
                SDL_RenderCopy(render_get_renderer(), background.texture, &background.school2.src, &background.school2.dst);
                SDL_RenderCopy(render_get_renderer(), background.texture, &background.fish_group1.src, &background.fish_group1.dst);
                SDL_RenderCopy(render_get_renderer(), background.texture, &background.fish_group2.src, &background.fish_group2.dst);
                SDL_RenderCopy(render_get_renderer(), background.texture, &background.fish_solo1.src, &background.fish_solo1.dst);
                SDL_RenderCopy(render_get_renderer(), background.texture, &background.fish_solo2.src, &background.fish_solo2.dst);
                SDL_RenderCopy(render_get_renderer(), background.texture, &background.fish_solo3.src, &background.fish_solo3.dst);
        }

        if (game_get_state() == GAMEOVER) {
                SDL_RenderCopy(render_get_renderer(), background.bg.texture, &background.bg.src, &background.bg.dst);    
        }
}

void background_destroy(void)
{
        if (game_get_state() == PLAY || game_get_state() == TITLE) {
                SDL_DestroyTexture(background.bg.texture);
                SDL_DestroyTexture(background.texture);
        }

        if (game_get_state() == GAMEOVER) {
                SDL_DestroyTexture(background.bg.texture);
        }
}
