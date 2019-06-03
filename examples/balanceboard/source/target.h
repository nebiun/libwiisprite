#ifndef __TARGET__
#define __TARGET__

#define MARGE 50

#include "global.h"

int randMinMax(int min,int max); //Fonction qui renvoie un nombre pseudo-aléatoire entre min et max inclus

class Target
{
    public:
        Target();
        void reinit();
        void draw();
        bool check(Point pos_point);
        bool getType();
    private:
        Point pos;
        int delay,creation;
        bool type;
};

#endif
