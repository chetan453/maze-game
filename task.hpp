#pragma once

#include<iostream>
#include<SDL2/SDL.h>
#include<queue>
#include<vector>
#include<stdlib.h>
#include<algorithm>
#include<time.h>
#include<string>
#include "player.hpp"
#include "global.hpp"
#include "font.hpp"

using namespace std;

namespace task_names{
    enum{
        lhc,
        library,
        academic_area,
        girnar_hostel,
        himadri_hostel,
        kumaon,
        count
    };

    Task_data lhc_data = {Point{624,392},"goto LHC"};
    Task_data library_data = {Point{800,352},"goto Library"};
    Task_data academic_data = {Point{880,217},"go to academic area"};
    Task_data girnar_data = {Point{352,160},"girnar"};
    Task_data kumaon_data = {Point{242,160},"Kumaon"};
    Task_data himadri_data = {Point{85,49},"himadri"};
}


class Task{
    Player* p;
    queue<Task_data> to_do;
    vector<Task_data> completed;
    Task_data task_data_list[task_names::count];

    public:
        Task(Player* _p):p(_p){
            task_data_list[task_names::lhc] = task_names::lhc_data;
            task_data_list[task_names::library] = task_names::library_data; 
            task_data_list[task_names::academic_area] = task_names::academic_data; 
            task_data_list[task_names::girnar_hostel] = task_names::girnar_data; 
            task_data_list[task_names::himadri_hostel] = task_names::himadri_data;
            srand(time(0));
            to_do.push(task_data_list[rand()%task_names::count]);
        }


        void add_new_task(){
            to_do.pop();
            int pos = rand()%task_names::count;
            while(count(completed.begin(),completed.end(),task_data_list[pos])){
                pos = rand()%task_names::count;
            }
            to_do.push(task_data_list[pos]);
        }

        void task_completed(){
            Task_data tmp = to_do.front();
            completed.push_back(tmp);
        }

        bool has_reached(){
            bool ans = true;
            ans = ans && (abs(p->get_pos().x-to_do.front().p.x) <= 4 && abs(p->get_pos().y-to_do.front().p.y) <= 4 ) ;
            return ans;
        }

        string get_task_text(){return to_do.front().task_text;}
        int completed_size(){return completed.size();}
        Point get_dst(){return to_do.front().p;}

};