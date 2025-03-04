#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#include "./uiabstraction.h"


#ifdef _WIN32
#ifndef TWIG_DEBUG
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#endif


void logOutputFunction(void *userdata, int category, SDL_LogPriority priority, const char *message) {

}


textAreaTextLine fileHead;


void keypressss(SDL_KeyboardEvent k) {
    if (k.key > 0xff) {
        return;
    }
    SDL_Log("%d %d %c\n",k.key, k.mod, k.key);
}

void read_and_load_file(const char* filename) {
    FILE *file = fopen(filename, "r");

    if(file == NULL) {
        SDL_Log("Error opening file");
        exit(1);
    }

    char ch;
    textAreaTextLine* currentLine = &fileHead;
    int curLineNum = 0;


    size_t size = 0;
    size_t length = 0;
    
    while ((ch = fgetc(file)) != EOF) {
        //printf("started loop\n");
        if (ch == '\n') {
            textAreaTextLine* tmpCurLine = currentLine;
            currentLine->lineContent[length] = '\0';
            printf("%s\n",currentLine->lineContent);
            currentLine = (textAreaTextLine*)malloc(sizeof(textAreaTextLine));
            currentLine->previousNode = tmpCurLine;
            tmpCurLine->nextNode = currentLine;
            
            curLineNum++;
            currentLine->lineContent = NULL;
            currentLine->lineNum = curLineNum;
            currentLine->nextNode = nullptr;

            length = 0;
            size = 0;
            continue;
        }
        //printf("started rl\n");
        if (ch == '\r') {
            printf("backsl r\n");
            continue;
        }
        //printf("started rnnn\n");

        if (length + 1 >= size) {
            size += 1; // larger chunks for non-constant reallocations
            currentLine->lineContent = (char*)realloc(currentLine->lineContent, size);
            if (!currentLine->lineContent) {
                perror("Failed to allocate memory");
                exit(EXIT_FAILURE);
            }
        }
        currentLine->lineContent[length++] = ch;
        
    }

    fclose(file);
}

int main(int argc, char* argv[]) {
    initUI("Window", 800, 600);
    SDL_Log(":3");
    fontStyle style;
    style.fontSize = 24;
    style.style = "./arial.ttf";
    initSetFont(style);
    //char x[10];
    //SDL_itoa(10, x, 10);

    read_and_load_file("./file.txt");

    textAreaTextLine* tex;
    tex = &fileHead;
    printf("\n\na\n\n");
    while (tex->nextNode != nullptr) {
        printf("tx: %d : %s\n", tex->lineNum, tex->lineContent);
        tex = tex->nextNode;
    }

    printf("Yippe!!!");

    int a = 0;
    while (1) {
        clearRender();
        a++;    
        //int starttick=SDL_GetTicksNS();
        textAreaTextLine* rTex;
        rTex = &fileHead;
        SDL_FRect rec = {0,0,0,0};
        while (rTex->nextNode != nullptr) {
            printf("a: %d %d %d %d %s\n", (int)rec.h, (int)rec.w, (int)rec.x, (int)rec.y, rTex->lineContent);
            //{(unsigned char)(a%255),0,0,0}
            rec = render_text_with_color(rTex->lineContent, {255,255,255,0}, 0, (int)rec.h+(int)rec.y);
            rTex = rTex->nextNode;
        }
        presentOurRender();
        checkEvnts();
        //int tottick=SDL_GetTicksNS()-starttick;
        //SDL_itoa((int)(1.0/(((double)tottick)/1000000000.0)), x, 10);
        //SDL_Log(x);
    }
    return 0;
}
