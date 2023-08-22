#include "ui.h"
#include "render.h"
#include "entity.h"
#include "game.h"
#include "input.h"

static struct Ui ui;

void ui_init(void)
{
        float scale = render_get_scale();

        ui.texture = IMG_LoadTexture(render_get_renderer(), "ui_atlas.png");

        if (game_get_state() == LOGO) {
                ui.logo.src = (SDL_Rect){0, 128, LOGO_W, LOGO_H};
                ui.logo.dst = (SDL_Rect){(32 * scale) + render_get_offset(), 112 * scale, LOGO_W * scale, LOGO_H * scale};

                ui.logo.frame_count = 0;
        }

        if (game_get_state() == TITLE) {
                ui.title.src = (SDL_Rect){0, 176, TITLE_W, TITLE_H};
                ui.title.dst = (SDL_Rect){(18 * scale) + render_get_offset(), 48 * scale, TITLE_W * scale, TITLE_H * scale};

                ui.play_btn.src = (SDL_Rect){0, 272, PLAYBTN_W, PLAYBTN_H};
                ui.play_btn.dst = (SDL_Rect){(49 * scale) + render_get_offset(), (176 * scale) + render_get_offset(), PLAYBTN_W * scale, PLAYBTN_H * scale};
        }

        if (game_get_state() == PLAY) {

                ui.src = (SDL_Rect){0, 0, SCORE_W, SCORE_H};
                ui.dst = (SDL_Rect){render_get_offset(), 0, SCORE_W * scale, SCORE_H * scale};

                ui.score.score_rect = (SDL_Rect){49 * scale + render_get_offset(), 9 * scale, 30 * scale, 16 * scale};

                ui.last_fish_caught = entity_get_fish_caught();
                ui_update_score();
        }

        if (game_get_state() == GAMEOVER) {

                ui.gameover.src = (SDL_Rect){0, 304, GAMEOVER_W, GAMEOVER_H};
                ui.gameover.dst = (SDL_Rect){ (25 * scale) + render_get_offset(), (48 * scale) + render_get_offset(), GAMEOVER_W * scale, GAMEOVER_H * scale};

                ui.src = (SDL_Rect){0, 336, GO_UI_W, GO_UI_H};
                ui.dst = (SDL_Rect){(51 * scale) + render_get_offset(), (87 * scale) + render_get_offset(), GO_UI_W * scale, GO_UI_H * scale};
                ui.score.score_rect = (SDL_Rect){(91 * scale) + render_get_offset(), 103 * scale, 30 * scale, 16 * scale};

                ui.play_btn.src = (SDL_Rect){0, 272, PLAYBTN_W, PLAYBTN_H};
                ui.play_btn.dst = (SDL_Rect){(49 * scale) + render_get_offset(), (176 * scale) + render_get_offset(), PLAYBTN_W * scale, PLAYBTN_H * scale};

                ui_update_score();
        }        
}

void ui_update(void)
{
        if (game_get_state() == LOGO) {
                ui.logo.frame_count++;
                if (ui.logo.frame_count >= 60) {
                        game_set_new_state(2);
                }
        }

        if (game_get_state() == TITLE) {
                if (input_get_event()) {
                        SDL_Point touch_loc = input_get_touch_loc();
                        SDL_bool collision  = SDL_PointInRect(&touch_loc, &ui.play_btn.dst);
                        if(collision) {
                                game_set_new_state(3);
                        }
                }
        }

        if (game_get_state() == PLAY) {   

                if (entity_get_fish_caught() > ui.last_fish_caught) {
                        ui_update_score();
                        ui.last_fish_caught = entity_get_fish_caught();
                }

                switch (entity_get_turtles_caught()) {
                        case 0: ui.src.y = 0; break;
                        case 1: ui.src.y = SCORE_H; break;
                        case 2: ui.src.y = SCORE_H * 2; break;
                        case 3: ui.src.y = SCORE_H * 3; break;
                }
        }

        if (game_get_state() == GAMEOVER) {
                if (input_get_event()) {
                        SDL_Point touch_loc = input_get_touch_loc();
                        SDL_bool collision  = SDL_PointInRect(&touch_loc, &ui.play_btn.dst);
                        if(collision) {
                                game_set_new_state(3);
                        }
                }
        }
}

void ui_render(void)
{
        if (game_get_state() == LOGO) {
                SDL_RenderCopy(render_get_renderer(), ui.texture, &ui.logo.src, &ui.logo.dst);
        }

        if (game_get_state() == TITLE) {
                SDL_RenderCopy(render_get_renderer(), ui.texture, &ui.title.src, &ui.title.dst);
                SDL_RenderCopy(render_get_renderer(), ui.texture, &ui.play_btn.src, &ui.play_btn.dst);
        }

        if (game_get_state() == PLAY) {
                SDL_RenderCopy(render_get_renderer(), ui.texture, &ui.src, &ui.dst);
                SDL_RenderCopy(render_get_renderer(), ui.score.label, NULL, &ui.score.label_rect);
        }

        if (game_get_state() == GAMEOVER) {
                SDL_RenderCopy(render_get_renderer(), ui.texture, &ui.gameover.src, &ui.gameover.dst);
                SDL_RenderCopy(render_get_renderer(), ui.texture, &ui.src, &ui.dst);
                SDL_RenderCopy(render_get_renderer(), ui.score.label, NULL, &ui.score.label_rect);
                SDL_RenderCopy(render_get_renderer(), ui.texture, &ui.play_btn.src, &ui.play_btn.dst);
        }
}

void ui_destroy(void)
{
        SDL_DestroyTexture(ui.texture);
        SDL_DestroyTexture(ui.score.label);
}

void ui_update_score(void)
{
        float scale = render_get_scale();

        ui.score.font = TTF_OpenFont("fonts/BitPap.ttf", 4 * scale);
        
        int fish_caught = entity_get_fish_caught();
        char buffer[256];
        sprintf(buffer, "%i", fish_caught);

        SDL_Color blue = (SDL_Color){0, 120, 139, 255};
        SDL_Surface *surface = TTF_RenderUTF8_Solid(ui.score.font, buffer, blue);

        ui.score.label = SDL_CreateTextureFromSurface(render_get_renderer(), surface);

        int text_center  = surface->w * scale / 2;
        int score_center = ui.score.score_rect.x + ui.score.score_rect.w / 2;
        int text_pos_x   = score_center - text_center;

        ui.score.label_rect = (SDL_Rect){text_pos_x, ui.score.score_rect.y, surface->w * scale, surface->h * scale};

        SDL_FreeSurface(surface);
        TTF_CloseFont(ui.score.font);

        for(size_t i = 0; i < sizeof buffer; ++i) {
                buffer[i] = 0;
        }
}