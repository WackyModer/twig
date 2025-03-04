#ifndef TWIG_UI_ABSTRACTION_H
#define TWIG_UI_ABSTRACTION_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>


// ctrl z is a "future me" problem
typedef struct {
    
} editHistory;

namespace UIENUMS {

    typedef enum {
        LEFT = 0,
        RIGHT = 1,
        MIDDLE = 2
    } textAlign;

    typedef enum {
        UI_NULL = 0,
        UI_BUTTON = 1,
        UI_SINGLE_LINE_TEXT = 2,
        UI_MULTI_LINE_TEXT = 3,
    } UIElemType;
};



typedef struct {
    char* style;
    int fontSize;
    int lineSpacing;
    UIENUMS::textAlign align;

} fontStyle;

// might not be the best way but text area text will be stored in a linked list

typedef struct textColoring {
    int len;
    char* text;
    SDL_Color color;
};

typedef struct textAreaTextLine{
    char* lineContent = NULL;

    // may remain unimplemented as it will require recounting all of the lines every time one is removed or added.
    int lineNum = 0;

    // nullptr if first
    textAreaTextLine *previousNode = nullptr;
    // nullptr if last
    textAreaTextLine *nextNode = nullptr;
};

typedef struct {
    int id;
    int fontSize;

    bool editable = true;
    bool isFocused;
    bool isVisible;
//    editHistory editHist;
    textAreaTextLine* textHeadNode;

    int cursorY;
    int cursorX;

    int linesScrolled;
    float lineOffset;

} textArea;

typedef struct {
    int id;
    int fontSize;

    bool editable = false;
    bool isFocused;
    editHistory editHist;

    char* textContent;
    int cursorX;
} singleLineText;





typedef struct {
    // no point in defining these for each elem.
    int xPos;
    int yPos;
    int width;
    int height;
    UIENUMS::UIElemType type;
    void* uiElem;
} uiElement;


typedef struct {
    int elementCount;
    uiElement* elements;
} uiInstance;



int initUI(const char* windowName, int windowW, int windowH);
void checkEvnts();
void exitUI();

void addElementToUIInst(uiInstance *inst, void* element, UIENUMS::UIElemType elemType);

// If I'm smart and want to save draw calls
// could use this to redraw a portion of a changed UIInstance
// when updating a part of this so theres no redundant draws.
void drawElement(uiElement* elemToDraw);

// Does NOT do anything smart to determine if stuff has changed. Will just redraw everything.
void drawUI(uiInstance* instanceToDraw);


SDL_FRect render_text_with_color(char* textToRender, SDL_Color textCol, int yOffset, int xOffset);
void initSetFont(fontStyle fontSty);
void presentOurRender();
void clearRender();

#endif