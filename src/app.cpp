//
// Created by lucha on 2024/8/27.
//

#include "app.h"
#include <iostream>

myApp::myApp(int argc, char *argv[], int w, int h):argc(argc), argv(argv)
{
    main_window = new Fl_Window(w, h);

    /* 1.菜单栏 */
    left_vertical_group = new Fl_Group(main_window->x(), main_window->y(), 50, main_window->h(), "left_vertical_group");
    left_vertical_group->box(FL_PLASTIC_UP_BOX);    // 整块浮动
    ncm_page_button = new Fl_Button(EX_WIDTH_MARGIN, EX_HEIGHT_MARGIN, left_vertical_group->w()-EX_ALL_WIDTH, BUTTON_HEIGH);
    ncm_page_button->label("@refresh");
    ncm_page_button->type(FL_RADIO_BUTTON);
    mp3_modify_button = new Fl_Button(EX_WIDTH_MARGIN, EX_HEIGHT_MARGIN+BUTTON_HEIGH+EX_ALL_HEIGHT, left_vertical_group->w()-EX_ALL_WIDTH, BUTTON_HEIGH);
    mp3_modify_button->type(FL_RADIO_BUTTON);
    mp3_modify_button->label("@+98->");
    mp3_modify_button->labeltype(FL_ENGRAVED_LABEL);
    left_vertical_group->end(); // 结束左侧菜单栏布局

    /* 2.主要功能区 */
    main_vertical_group = new Fl_Group(left_vertical_group->w(),
                                       0,
                                       main_window->w()-left_vertical_group->w(),
                                       main_window->h());
    main_vertical_group->box(FL_THIN_UP_FRAME);

    /* 2.1 表格区 */
    main_list_group = new Fl_Group(main_vertical_group->x(),
                                   main_vertical_group->y()+0,
                                   main_vertical_group->w(),
                                   main_vertical_group->h() * 0.7);
    main_list_group->end();

    // 横分割线
    h_line = new Fl_Box(FL_DOWN_BOX, main_vertical_group->x(), main_list_group->h(),
                        main_vertical_group->w(), 2, nullptr);
    h_line->box(FL_FLAT_BOX);
    h_line->color(FL_BLACK);

    /* 2.2 按钮操作区 */
    main_button_group = new Fl_Group(main_vertical_group->x(),
                                     main_vertical_group->y()+main_list_group->h(),
                                     main_vertical_group->w(),
                                     main_vertical_group->h() - main_list_group->h());
    // main_button_group->box(FL_ROUNDED_BOX);
    main_button_group->end();

    main_window->end(); // 默认布局结束
}

myApp::~myApp()
{
    delete ncm_page_button;
    delete mp3_modify_button;
    delete left_vertical_group;

    delete main_button_group;
    delete main_list_group;
    delete main_vertical_group;

    delete main_window;
}

void myApp::show()
{
    main_window->labelfont(FL_ITALIC + FL_BOLD);
    main_window->labelsize(20);
    main_window->label("ncm2mp3");

    main_window->show(argc, argv);
}
