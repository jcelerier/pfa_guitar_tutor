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

    Config();
    static QRect getWindowSize();
    void setFormat(float);
};

#endif // CONFIG_H
