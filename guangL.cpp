// Author: Guangjin Liu
// CMPS 3350
// modified date: 2-23-18
// file for background 

#include <Windows.h>
#include <font.h>
#include <graphics.h>
#pragma once
#pragma warning (disable : 4075)
#pragma warning (disable : 4098)
#include <cstdio>
#include <exception>
#include <memory>
#include <fstream>
#include <string>
#include <cassert>
#include <vector>
#include <stack>
#include <list>
#include <queue>
#include <map>
#include <complex>
#include <SDL.h>
#include <SDL_imge.h>
#include <SDL_ttf.h>


struct layer {
    SDL_Texture* pTexture;
    SDL_Rect src, dst;
    int w, h;

    struct {
        int scrollX, scrollY;
        int alpha;
    } Effect;
};

void background(double dt) {

    /*
    Texture texture;
    if (!texture.loadFromFile("image/background.gif")) {

    }

    Sprite background (texture);

    window.draw (background);
    window.draw (rectangle);
    window.display ();
    */
    
    //render background
    
    
    for (unsinged int i = 0; i < m_layers.size(); ++i) {
        SDL_Rect src = m_layers[i].src;

        if (m_layers[i].src.x < 0) {
            m_layers[i].src.x = 0;
        }
        else if (m_layers[i].src.x > m_rightEdge) {
            m_layers[i].src.x = m_rightEdge;
        }

        SDL_RenderCopyEx();
        //process stage effect
        if (m_layers[i].Effect.scrollX || m_layers[i].Effect.scrollY) {
            //scroll the layer.
            m_layers[i].dst.x += static_cast<int>(m_layers[i].Effect.scrollY *dt);

            //calc offset for 2nd render
            SDL_Rect dst2 = m_layers[i].dst;
            
            dst2.x = dst2.x - dst2.w - m_layers[i].src.x + m_rightEdge;

            //render 2nd time with offset
            SDL_RenderCopyEx();

            if (dst2.x >=0) {
                m_layers[i].dst.x = 0;
            }
        }
    }
}
