#pragma once

#include<iostream>
#include<SDL2/SDL_image.h>


namespace screen{
    enum{
        width=1280,
        height=720
    };
}

struct RGB{
    int r;
    int g;
    int b;
};



struct Point{
    int x;
    int y;

    public:
        bool operator==(struct Point &rhs){
            return x==rhs.x && y==rhs.y;
        }

        bool operator!=(struct Point &rhs){
            return !(x==rhs.x && y==rhs.y);
        }

        Point operator+(struct Point &rhs){
            return Point{x + rhs.x,y+rhs.y};
        }

        void operator+=(struct Point &rhs){
            x+=rhs.x;
            y=rhs.y;
        }
};

struct Task_data{
    Point p;
    string task_text;
    public:
        bool operator==(struct Task_data rhs){
            return p == rhs.p;
        }
};