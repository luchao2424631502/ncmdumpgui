//
// Created by lucha on 2024/8/27.
//

#ifndef C__PROJECT_APP_H
#define C__PROJECT_APP_H

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Output.H>


class myApp
{
    enum { EX_WIDTH_MARGIN = 4,
           EX_HEIGHT_MARGIN = 4,
           EX_ALL_WIDTH = 8,
           EX_ALL_HEIGHT = 8,
           BUTTON_WIDTH = 45,
           BUTTON_HEIGH = 45 };
private:
    int argc;
    char **argv;

    Fl_Window *main_window;
    Fl_Group *left_vertical_group;
    Fl_Group *main_vertical_group;

    Fl_Group *main_list_group;
    Fl_Box *h_line;
    Fl_Group *main_button_group;

    Fl_Button *ncm_page_button;
    Fl_Button *mp3_modify_button;

public:
    myApp(int argc, char *argv[], int w = 800, int h = 500);
    virtual ~myApp();

public:
    virtual void show();
};

#endif //C__PROJECT_APP_H
