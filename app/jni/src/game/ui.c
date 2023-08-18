#include "ui.h"
#include "engine/render.h"
#include "entity.h"

static struct Ui ui;

void ui_init_play(void)
{
        float scale = render_get_scale();

        ui.texture = IMG_LoadTexture(render_get_renderer(), "ui_atlas.png");

        ui.src = (SDL_Rect){0, 0, SCORE_W, SCORE_H};
        ui.dst = (SDL_Rect){render_get_offset(), 0, SCORE_W * scale, SCORE_H * scale};

        ui.score.score_rect = (SDL_Rect){49 * scale + render_get_offset(), 9 * scale, 30 * scale, 16 * scale};

        ui.last_fish_caught = entity_get_fish_caught();

        ui_update_score();
}

void ui_update_play(void)
{
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

void ui_render_play(void)
{
        render_draw(ui.texture, &ui.src, &ui.dst);
        render_draw(ui.score.label, NULL, &ui.score.label_rect);
}

void ui_destroy_play(void)
{
        SDL_DestroyTexture(ui.texture);
        SDL_DestroyTexture(ui.score.label);

        ui.src = (SDL_Rect){0, 0, 0, 0};
        ui.dst = (SDL_Rect){0, 0, 0, 0};
        ui.score.label_rect = (SDL_Rect){0, 0, 0, 0};
}

void ui_update_score(void)
{
        float scale = render_get_scale();

        SDL_DestroyTexture(ui.score.label);

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
}