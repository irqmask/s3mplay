/*
 * s3mplay - A very simple player for S3M music.
 * Copyright (C) 2017  christian <irqmask@web.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file main.c
 * A very simple player application for libs3m.
 *
 * \author  CV (irqmask@web.de)
 * \date    2017-05-01
 */

/* Include section -----------------------------------------------------------*/
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <SDL.h>
#include <SDL_ttf.h>

#include "s3m.h"

/* Definitions ---------------------------------------------------------------*/

/* Type definitions ----------------------------------------------------------*/

typedef union {
    uint8_t u8[4];
    void*   v;
} evt_data_t;

/* Local variables -----------------------------------------------------------*/

static bool             g_have_player_window = true;
static bool             g_have_font = true;


static const uint32_t   g_sample_rate = 44100;
static SDL_AudioSpec    g_sdl_audio_spec;
static SDL_Window*      g_window;
static SDL_Renderer*    g_renderer;

static s3m_t            g_s3m;
static uint8_t          g_current_pattern = 0;
static uint8_t          g_current_row = 0;

static TTF_Font*        g_text_font = NULL;

/* Global variables ----------------------------------------------------------*/

static int init_sdl(void) 
{
    int retval = -1;
    
    do {
        if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) < 0) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
            break;
        }

        if (g_have_player_window) {
            if (SDL_CreateWindowAndRenderer(320, 240, SDL_WINDOW_RESIZABLE, &g_window, &g_renderer)) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
                break;
            }
    
            TTF_Init();
            if ((g_text_font = TTF_OpenFont("Perfect DOS VGA 437.ttf", 16)) == NULL) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
                g_have_font = false;
            }
        }

        g_sdl_audio_spec.freq = g_sample_rate;
        g_sdl_audio_spec.format = AUDIO_S16;
        g_sdl_audio_spec.channels = 2;
        g_sdl_audio_spec.samples = 2048;
        g_sdl_audio_spec.callback = (SDL_AudioCallback)s3m_sound_callback;
        g_sdl_audio_spec.userdata = NULL;

        SDL_AudioSpec obtainedSpec;

        // initialize audio
        if (SDL_OpenAudio(&g_sdl_audio_spec, &obtainedSpec) < 0) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't open audio: %s", SDL_GetError());
            break;
        }
        SDL_PauseAudio(1);
        
        retval = 0;
    } while (0);
    return retval;
}

static void close_sdl(void)
{
    if (g_have_player_window) {
        SDL_DestroyRenderer(g_renderer);
        SDL_DestroyWindow(g_window);
    }

    SDL_Quit();    
}

static void play(void)
{
    s3m_play(&g_s3m);
    SDL_PauseAudio(0);    
}

static void pause(void)
{
    SDL_PauseAudio(1);
}

static void resume(void)
{
    SDL_PauseAudio(0);
}

static void stop(void)
{
    SDL_PauseAudio(1);
    s3m_stop(&g_s3m);
}

static void update_display(void)
{
    char txt[256];
    SDL_Color color;
    
    if (!g_have_font) return;
    
    snprintf(txt, sizeof(txt), "p%03d - r%03d", g_current_pattern, g_current_row);
    txt[255] = '\0';
    color.r = 200;
    color.g = 200;
    color.b = 200;
    color.a = 10;   
    
    SDL_Surface* surface = TTF_RenderText_Solid(g_text_font, txt, color);
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(g_renderer, surface);

    SDL_Rect rect; //create a rect
    rect.x = 10;  //controls the rect's x coordinate 
    rect.y = 10; // controls the rect's y coordinte
    rect.w = 11*16; // controls the width of the rect
    rect.h = 16; // controls the height of the rect

    SDL_RenderCopy(g_renderer, texture, NULL, &rect);
}

static void row_changed_func(s3m_t* s3m, void* arg)
{
    SDL_Event event;
    SDL_UserEvent userevent;
    evt_data_t ed;
    
    ed.u8[0] = s3m_get_current_pattern_idx(s3m);
    ed.u8[1] = s3m_get_current_row_idx(s3m);
 
    userevent.type = SDL_USEREVENT;
    userevent.code = 0;
    userevent.data1 = ed.v;
 
    event.type = SDL_USEREVENT;
    event.user = userevent;
 
    SDL_PushEvent (&event);
}

int main(int argc, char* argv[])
{
    bool running = 1;
    bool is_paused = false;
    
    const char* filename;
    
    SDL_Event event;
       
    if (argc < 2) {
        printf("s3mplay\n");
        printf("usage: s3mplay [filename.s3m]\n");
        printf("Playing will automatically start.\n");
        printf("Keys during playing:\n");
        printf("  ESC   - Escape! end playing and program.\n");
        printf("  F5    - Play\n");
        printf("  Space - Pause / Resume\n");
        printf("  F8    - Stop\n"); 
        return -1;
    }
    
    filename = argv[1];

    if (init_sdl() < 0) return -1;
    
    s3m_initialize(&g_s3m, g_sample_rate);
    s3m_register_row_changed_callback(&g_s3m, (s3m_func_t)row_changed_func, NULL);
    if (s3m_load(&g_s3m, filename) != 0) {
        printf("File %s not found!\n", filename);
        running = 0;
    }
    //s3m_print_header(&g_s3m);
    //s3m_print_channels(&g_s3m);
    //s3m_print_arrangement(&g_s3m);
    //s3m_print_instruments(&g_s3m);
    //s3m_print_patterns(&g_s3m);       
    if (running) play();
    
    while (running) {
        if (g_have_player_window) SDL_RenderClear(g_renderer);        
        
        SDL_WaitEventTimeout(&event,10);

        switch (event.type) {
        case SDL_QUIT:
            running = false;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:                     
                running = false;
                break;
            case SDLK_SPACE:
                if (is_paused) {
                    is_paused = false;
                    resume();
                } else {
                    is_paused = true;
                    pause();
                }
                break;
            case SDLK_F5:
                play();
                break;
            case SDLK_F8:
                stop();
                break;
            }           
            break;       
        
        case SDL_USEREVENT:
            if (g_have_player_window) update_display();
            evt_data_t ed;
            ed.v = event.user.data1;
            g_current_pattern = ed.u8[0];
            g_current_row = ed.u8[1];
            break;
        }

        if (g_have_player_window) SDL_RenderPresent(g_renderer);
    }
    
    SDL_PauseAudio(1);
    s3m_unload(&g_s3m);

    close_sdl();
    return 0;
}

/* EOF: main.c */
