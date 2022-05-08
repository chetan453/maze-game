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
        kumaon_hostel,
        jwalamukhi_hostel,
        nilgiri_hostel,
        SAC,
        main_building,
        hospital1,
        hospital2,
        eatery1,
        eatery2,	
        ground,
        count
    };

    Task_data lhc_data = {Point{1128,380},"Go to LHC to submit your chemistry lab experiment."};
    Task_data library_data = {Point{800,344},"Go to Library to return a borroed book."};
    Task_data academic_data = {Point{880,217},"Go to academic area to discuss a project proposal with your professor."};
    Task_data girnar_data = {Point{352,160},"Go to Girnar hostel to say hi to your friend Hitesh there."};
    Task_data himadri_data = {Point{1144,168},"Go to Himadri hostel to say hi to your friend Shivani there."};
    Task_data kumaon_data = {Point{242,160},"Go to Kumaon hostel to say hi to your friend Akram there."};
    Task_data jwalamukhi_data = {Point{544,176},"Go to Jwalamukhi hostel to go say hi to your friend Satish there."};
    Task_data nilgiri_data = {Point{136,176},"Go to Nilgiri hostel to go say hi to your friend John there."};
    Task_data sac_data = {Point{440,380},"Go to SAC to play a badminton match."};
    Task_data main_building_data = {Point{1032,344},"Go to the Main Building to get your documents verified."};
    Task_data hospital1_data = {Point{640,344},"Go to hospital to get yourself checked up."};
    Task_data hospital2_data = {Point{1130,48},"Go to covid centre for your covid test."};
    Task_data eatery1_data = {Point{372,174},"Go to eatery to get yourself energized."};
    Task_data eatery2_data = {Point{976,380},"Go to eatery to reward yourself with some snacks."};
    Task_data ground_data = {Point{280,380},"Go to ground for your daily exercise."};
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
            task_data_list[task_names::himadri_hostel] = task_names::himadri_data;
            task_data_list[task_names::kumaon_hostel] = task_names::kumaon_data;
            task_data_list[task_names::jwalamukhi_hostel] = task_names::jwalamukhi_data;
            task_data_list[task_names::nilgiri_hostel] = task_names::nilgiri_data;
            task_data_list[task_names::SAC] = task_names::sac_data;
            task_data_list[task_names::main_building] = task_names::main_building_data;
            task_data_list[task_names::hospital1] = task_names::hospital1_data;
            task_data_list[task_names::hospital2] = task_names::hospital2_data;
            task_data_list[task_names::eatery1] = task_names::eatery1_data;
            task_data_list[task_names::eatery2] = task_names::eatery2_data;
            task_data_list[task_names::ground] = task_names::ground_data;       

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