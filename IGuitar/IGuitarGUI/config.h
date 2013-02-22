#ifndef CONFIG_H
#define CONFIG_H

#include <QtGui>

class Config
{
public:
    static QRect viewRect;
    static float format;
    static float quotient;
    static int height;
    static int width;
    static int const originalHeight=1080;
    static int const originalWidth=1920;

    Config();
    static QRect getWindowSize();
    void setFormat(float);
};

#endif // CONFIG_H
