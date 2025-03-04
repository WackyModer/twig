#include "./uiabstraction.h"

#define SDL_HINT_RENDER_VSYNC "1"

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#ifdef _WIN32
#ifndef TWIG_DEBUG
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#endif


static SDL_Window* window;
static SDL_Renderer *renderer;
TTF_Font *font;


int initUI(const char* windowName, int windowW, int windowH) {
    #ifdef _WIN32
    #ifndef TWIG_DEBUG
        // Windows-specific setup to avoid the console window
        //FreeConsole();  // This will detach the console on Windows if it was created
    #endif
    #endif
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL_Init Error: %s", SDL_GetError());
        return 1;
    }

    //window = SDL_CreateWindow(windowName, windowW, windowH, SDL_WINDOW_RESIZABLE);
    SDL_CreateWindowAndRenderer(windowName, windowW, windowH, SDL_WINDOW_RESIZABLE, &window, &renderer); 
    if (window == NULL) {
        SDL_Log("SDL_CreateWindow Error: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    if( !SDL_SetRenderVSync( renderer, 1 ) )
    {
        SDL_Log("SDL_SetRenderVSync Error: %s\n", SDL_GetError() );
        exit(1);
    }
    
    if(!TTF_Init()) {
        SDL_Log("TFF_Init Error: %s\n", SDL_GetError() );
        exit(1);
    }
    SDL_Log("Successfully *initialized* UI\n");
    return 0;
} 


void exitUI() {
    SDL_Log("Exiting UI\n");
    SDL_DestroyWindow(window);
    SDL_Quit();
    SDL_Log("UI Exited\n");
    exit(1);
}



void registerKeypress(SDL_KeyboardEvent k) {
    SDL_Log("dfasd1dfadsf");
    if (k.key > 0xff) {
        return;
    }
    SDL_Log("%d %d %c\n",k.key, k.mod, k.key);
}


void checkEvnts() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            SDL_Log("exit...\n");
            exitUI();
        } else if (event.type == SDL_EVENT_KEY_DOWN) {
            registerKeypress(event.key);
        }
    }
}


void addElementToUIInst(uiInstance *inst, void* element, UIENUMS::UIElemType elemType) {
    
}


void get_text_and_rect(SDL_Renderer *renderer, int x, int y, char *text,
    TTF_Font *font, SDL_Texture **texture, SDL_FRect *rect, SDL_Color textColor) {
    int text_width;
    int text_height;
    SDL_Surface *surface;

    surface = TTF_RenderText_Solid(font, text, strlen(text), textColor);
    *texture = SDL_CreateTextureFromSurface(renderer, surface);
    text_width = surface->w;
    text_height = surface->h;
    SDL_Log("dfas: %d %d\n", surface->h, surface->w);
    SDL_DestroySurface(surface);
    SDL_Log("dfas: %d %d\n", text_width, text_height);
    rect->x = x;
    rect->y = y;
    rect->w = text_width;
    rect->h = text_height;
    SDL_Log("dfas: %d %d\n", (int)rect->h, (int)rect->w);
}


void initSetFont(fontStyle fontSty) {
    SDL_Log(fontSty.style);
    font = TTF_OpenFont(fontSty.style, fontSty.fontSize);
}
int a = 0;
char b[10];
SDL_FRect render_text_with_color(char* textToRender, SDL_Color textCol, int xOffset, int yOffset) {
a++;
    //SDL_itoa(a, b, 10);
    //SDL_Log(b);
    SDL_Texture *textTex;
    SDL_FRect textRect;
    if (font == NULL) {
        SDL_Log("error: font not found\n");
        //exit(1);
    }
    get_text_and_rect(renderer, xOffset, yOffset, textToRender, font, &textTex, &textRect, textCol);
    //get_text_and_rect(renderer, 0, 0, b, font, &textTex, &textRect);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    //SDL_RenderClear(renderer);
    /* Use TTF textures. */
    SDL_RenderTexture(renderer, textTex, NULL, &textRect);

    return textRect;
}

void clearRender() {
    SDL_RenderClear(renderer);
}

void presentOurRender() {
    SDL_RenderPresent(renderer);
}

void drawElement(uiElement* elemToDraw) {
    switch (elemToDraw->type)
    {
        case (UIENUMS::UI_NULL):
            SDL_Log("[drawElement] Element with type `NULL` given!");
            break;
        case (UIENUMS::UI_BUTTON):
            break;
        case (UIENUMS::UI_MULTI_LINE_TEXT):
            break;
        case (UIENUMS::UI_SINGLE_LINE_TEXT):
            break;
        default:
            break;
    }
}


void drawUI(uiInstance* instanceToDraw) {
    for (int i = 0; i<instanceToDraw->elementCount; i++) {
        drawElement(&(instanceToDraw->elements[i]));
        SDL_RenderPresent(renderer);
    }
}