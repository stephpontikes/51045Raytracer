#ifndef SCENE_H
#define SCENE_H

#include "image.h"

class Scene {
   public:
    Scene() {}

    bool render(Image& outputImage) {
        int imgWidth = outputImage.getWidth();
        int imgHeight = outputImage.getHeight();

        for (int x = 0; x < imgWidth; x++) {
            for (int y = 0; y < imgHeight; y++) {
                double red = (static_cast<double>(x) / double(imgWidth)) * 255.0;
                double green = (static_cast<double>(y) / double(imgHeight)) * 255.0;
                outputImage.setPixel(x, y, red, green, 0.0);
            }
        }

        return true;
    }
};

#endif