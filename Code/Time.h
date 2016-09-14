#ifndef TIME_H
#define TIME_H

#include<time.h>
#include"DataStructs.h"

double CalcFPS(FrameTime* time);
void RenderFPS(Window* win, FrameTime* time);

#endif //TIME_H
