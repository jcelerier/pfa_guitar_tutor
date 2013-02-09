#include "config.h"

/*
 * Cette classe servira a la configuration graphique du programme.
 * Tous les parametres ne sont pas encore utilisés.
 *
 */

float Config::format = 0.5f;
int Config::width = 1366;
int Config::height = 768;
QRect Config::viewRect(0, 0, Config::width, Config::height);
float Config::quotient = float(Config::height)/1080; // L'image de base est de hauteur 1080, pour mise a l'echelle

Config::Config()
{

}

QRect Config::getWindowSize()
{
    viewRect = QRect(0, 0, Config::width, Config::height);
    return viewRect;
}

void Config::setFormat(float f) {
    format = f;
    height = round(width/format);
}
