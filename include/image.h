#ifndef IMAGE_H
#define IMAGE_H
#include <SDL3/SDL.h>

#include <iostream>
#include <iterator>
#include <vector>

using std::vector;

class Image {
   public:
    Image() : width(0), height(0), texture(NULL) {}
    ~Image() {
        if (texture != NULL) {
            SDL_DestroyTexture(texture);
        }
    }

    void init(SDL_Renderer *r, int const w, int const h) {
        renderer = r;
        width = w;
        height = h;

        rChannel.resize(width, std::vector<double>(height, 0));
        gChannel.resize(width, std::vector<double>(height, 0));
        bChannel.resize(width, std::vector<double>(height, 0));

        initTexture();
    }

    int getWidth() {
        return width;
    }

    int getHeight() {
        return height;
    }

    void setPixel(int const x, int const y, double const red,
                  double const green, double const blue) {
        rChannel.at(x).at(y) = red;
        gChannel.at(x).at(y) = green;
        bChannel.at(x).at(y) = blue;
    }

    void display() {
        vector<Uint32> pixels{};

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                pixels.push_back(rgbToInt32(rChannel.at(x).at(y),
                                            gChannel.at(x).at(y),
                                            bChannel.at(x).at(y)));
            }
        }

        SDL_UpdateTexture(texture, NULL, &pixels[0], width * sizeof(Uint32));

        SDL_FRect srcRect;
        SDL_FRect bounds;
        srcRect.x = 0;
        srcRect.y = 0;
        srcRect.w = width;
        srcRect.h = height;
        bounds = srcRect;
        SDL_RenderTexture(renderer, texture, &srcRect, &bounds);
    }

   private:
    Uint32 rgbToInt32(double const red, double const green, double const blue) {
        unsigned char r = static_cast<unsigned char>(red);
        unsigned char g = static_cast<unsigned char>(green);
        unsigned char b = static_cast<unsigned char>(blue);

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        Uint32 pixelColor = (b << 24) + (g << 16) + (r << 8) + 255;
#else
        Uint32 pixelColor = (255 << 24) + (r << 16) + (g << 8) + b;
#endif

        return pixelColor;
    }

    void initTexture() {
        Uint32 format;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        format = SDL_PIXELFORMAT_BGRA32;
#else
        format = SDL_PIXELFORMAT_ARGB32;
#endif

        if (texture != NULL) {
            SDL_DestroyTexture(texture);
        }

        SDL_Surface *tempSurface = SDL_CreateSurface(width, height, format);
        texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
        SDL_DestroySurface(tempSurface);
    }

   private:
    std::vector<std::vector<double>> rChannel;
    std::vector<std::vector<double>> gChannel;
    std::vector<std::vector<double>> bChannel;

    int width;
    int height;

    SDL_Renderer *renderer;
    SDL_Texture *texture;
};

#endif