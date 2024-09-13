//
// Created by lucha on 2024/8/28.
//

#ifndef C__PROJECT_EDITOR_H
#define C__PROJECT_EDITOR_H

#include <FL/Fl_Window.H>
#include <Fl/Fl_Pack.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Double_Window.H>
#include <Fl/Fl_Text_Editor.H>

#include <Fl/Fl_Menu.H>
#include <Fl/Fl_Menu_Bar.H>
#include <Fl/Fl_Menu_Item.H>

#include <Fl/fl_ask.H>
#include <Fl/Fl_File_Chooser.H>

/* 菜单回调函数类 */
class MenuBar_Utility
{
public:
    MenuBar_Utility() = delete;
    ~MenuBar_Utility() = delete;

public:
    static void newfile_cb(Fl_Widget *widget, void *data);
    static void openfile_cb(Fl_Widget *widget, void *data);
    static void save_cb(Fl_Widget *widget, void *data);
    static void saveas_cb(Fl_Widget *widget, void *data);
    static void replace1_cb(Fl_Widget *widget, void *data);
    static void find1_cb(Fl_Widget *widget, void *data);
    static void insert_cb(Fl_Widget *widget, void *data);
    static void view_cb(Fl_Widget *widget, void *data);
    static void view_close_cb(Fl_Widget *widget, void *data);
    static void exit_cb(Fl_Widget *widget, void *data);
    static void undo_cb(Fl_Widget *widget, void *data);
    static void redo_cb(Fl_Widget *widget, void *data);
};

class EditorWindow;

/* 编辑器回调函数类 */
class Editor_Utility
{
public:
    Editor_Utility() = delete;
    ~Editor_Utility() = delete;

public:
    static int check_save(EditorWindow *mainwindow);
    static void bufferchange_cb(int pos, int nInserted,
                                int nDeleted, int nRestyled,
                                const char *deletedText, void *cbArg);
};

/* Replace 替换窗口 */
class ReplaceDialog : public Fl_Window
{
public:
    ReplaceDialog(int w = 400, int h = 120);
    ~ReplaceDialog();

public:
    Fl_Pack *main_container;

    Fl_Group *input_region;
    Fl_Input *find_input;
    Fl_Input *replace_input;

    Fl_Pack *button_region;
    Fl_Box *placeholder_left_box;
    Fl_Button *replace_all;
    Fl_Button *replace_next;
    Fl_Button *cancel;

};

/* Find 窗口 */
class FindDialog : public Fl_Window
{
public:
    FindDialog(int w = 400, int h = 120);
    ~FindDialog();

public:
    Fl_Pack *main_container;
    Fl_Group *input_region;
    Fl_Input *find_input;

    Fl_Pack *button_region;
    Fl_Box *placeholder_left_box;
    Fl_Button *find_button;
    Fl_Box *placeholder_right_box;
    Fl_Button *cancel;
};

class EditorWindow : public Fl_Double_Window
{
    /* 类回调函数友元 */
    friend class MenuBar_Utility;
    friend class Editor_Utility;

    /* ReplaceDialog 回调 */
    static void replace_dialog_replaceall_cb(Fl_Widget *widget, void *data);
    static void replace_dialog_replacenext_cb(Fl_Widget *widget, void *data);
    static void replace_dialog_cancel_cb(Fl_Widget *widget, void *data);
    /* FindDialog 回调 */
    static void find_dialog_find_cb(Fl_Widget *widget, void *data);
    static void find_dialog_cancel_cb(Fl_Widget *widget, void *data);
public:
    /* 构造函数 */
    EditorWindow(int w, int h, int argc, char *argv[], const char *t=nullptr);
    ~EditorWindow();

public:
    /* 主体窗口成员 */
    Fl_Menu_Bar         *menubar;

    Fl_Text_Editor      *editor;
    Fl_Text_Buffer      *text_buffer;

    ReplaceDialog       *replace_dialog;    // 替换窗口
    FindDialog          *find_dialog;   // 查找窗口
public:
    /* 状态变量 */
    Fl_Menu_Item menuitems[32];

    int ischanged;
    const char *current_file;
    int argc;
    char **argv;
};

#endif //C__PROJECT_EDITOR_H
