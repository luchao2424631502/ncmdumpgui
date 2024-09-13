//
// Created by lucha on 2024/8/28.
//
#include "editor.h"
#include <iostream>
#include <cstring>

void EditorWindow::replace_dialog_replaceall_cb(Fl_Widget *widget, void *data)
{
    // 先拿到fl_input的值试一试
    EditorWindow *mainwindow = reinterpret_cast<EditorWindow *>(data);
    ReplaceDialog *dialog = mainwindow->replace_dialog;
    const char *find_str = dialog->find_input->value();
    const char *replace_str = dialog->replace_input->value();

    if (0 == std::strcmp(find_str, "")) {
        fl_alert("Find content is null");
        return ;
    }

    int foundPos = 0;
    int isfind = mainwindow->text_buffer->search_forward(foundPos,
                                                         find_str,
                                                         &foundPos,
                                                         0);
    if (!isfind) {
        fl_alert("Not Found!!!");
        return ;
    } else {
        // 进行全局替换
        do {
            mainwindow->text_buffer->replace(
                    foundPos,
                    foundPos+std::strlen(find_str),
                    replace_str);
        } while (mainwindow->text_buffer->search_forward(foundPos, find_str, &foundPos, 0));
        fl_alert("Replace All Success!!!");
    }
}

void EditorWindow::replace_dialog_replacenext_cb(Fl_Widget *widget, void *data)
{
    EditorWindow *mainwindow = reinterpret_cast<EditorWindow *>(data);
    ReplaceDialog *dialog = mainwindow->replace_dialog;

    const char *find_str = dialog->find_input->value();
    const char *replace_str = dialog->replace_input->value();

    if (0 == std::strcmp(find_str, "")) {
        fl_alert("Find content is null");
        return ;
    }
    int foundPos;

    int isfind = mainwindow->text_buffer->search_forward(
            0,
            find_str,
            &foundPos,
            0);
    if (isfind) {
        mainwindow->text_buffer->replace(
                foundPos,
                foundPos+std::strlen(find_str),
                replace_str);
        fl_alert("Replace Success!");
    } else { // 没有找到
        fl_alert("Not Found!!!");
    }
}

void EditorWindow::replace_dialog_cancel_cb(Fl_Widget *widget, void *data)
{
    EditorWindow *mainwindow = reinterpret_cast<EditorWindow *>(data);
    mainwindow->replace_dialog->hide();
}

ReplaceDialog::ReplaceDialog(int w, int h): Fl_Window(w, h)
{
    this->label("Replace");
    main_container = new Fl_Pack(0, 0, this->w(), this->h());
    main_container->type(Fl_Pack::VERTICAL);
    main_container->spacing(10);

    input_region = new Fl_Group(0, 0, this->w(), 10 + 25 + 8 + 25);

    find_input = new Fl_Input(70, 10, input_region->w() - 70 - 20, 25, "Find:");
    replace_input = new Fl_Input(70, 10 + 25 + 8, input_region->w() - 70 - 20, 25, "Replace:");
    input_region->end();

    button_region = new Fl_Pack(0, 0, this->w(), 30);
    button_region->type(Fl_Pack::HORIZONTAL);
    button_region->spacing(15);

    placeholder_left_box = new Fl_Box(0,0,30,30, nullptr);
    placeholder_left_box->box(FL_NO_BOX);
    replace_all = new Fl_Button(30,0, 90, 30, "Replace All");
    replace_next = new Fl_Button(0,0, 120, 30, "Replace Next");
    cancel = new Fl_Button(0, 0, 90, 30, "Cancel");
    button_region->end();

    main_container->end();
}

ReplaceDialog::~ReplaceDialog()
{
    delete cancel;
    delete replace_next;
    delete replace_all;
    delete placeholder_left_box;
    delete button_region;
    delete replace_input;
    delete find_input;
    delete input_region;
    delete main_container;
}

void EditorWindow::find_dialog_find_cb(Fl_Widget *widget, void *data)
{
    EditorWindow *mainwindow = reinterpret_cast<EditorWindow *>(data);

    const char *find_str = mainwindow->find_dialog->find_input->value();
    if (std::strcmp(find_str, "") == 0) {
        fl_alert("Find String is NULL!!!");
        return ;
    }

    // 处理find
    int foundPos = 0;
    auto isfound = mainwindow->text_buffer->search_forward(0, find_str, &foundPos, 0);
    if (isfound) {
        mainwindow->text_buffer->select(foundPos, foundPos + std::strlen(find_str));
    } else {
        fl_alert("Content isn't found!!!");
    }
    return ;
}

void EditorWindow::find_dialog_cancel_cb(Fl_Widget *widget, void *data)
{
    EditorWindow *mainwindow = reinterpret_cast<EditorWindow *>(data);
    // 关闭窗口即可
    mainwindow->find_dialog->hide();
    return ;
}

FindDialog::FindDialog(int w, int h): Fl_Window(w, h)
{
    this->label("Find");

    main_container = new Fl_Pack(0, 0, this->w(), this->h());
    main_container->type(Fl_Pack::VERTICAL); // 整体容器采用垂直排列
    main_container->spacing(10);

    // 容器1
    input_region = new Fl_Group(0, 0, this->w(), 10 + 25);
    find_input = new Fl_Input(70, 10, input_region->w() - 70 - 20, 25, "Find:");
    input_region->end();

    // 容器2
    button_region = new Fl_Pack(0, 0, this->w(), 30);
    button_region->type(Fl_Pack::HORIZONTAL);

    placeholder_left_box = new Fl_Box(0, 0, 30, 30, nullptr);
    placeholder_left_box->box(FL_NO_BOX);

    find_button = new Fl_Button(0, 0, 90, 30, "Find");
    placeholder_right_box = new Fl_Box(0,0, 40, 30, nullptr);
    placeholder_right_box->box(FL_NO_BOX);
    cancel = new Fl_Button(0, 0, 90, 30, "Cancel");

    button_region->end();
    main_container->end();
}

FindDialog::~FindDialog()
{
    delete cancel;
    delete placeholder_right_box;
    delete find_button;
    delete placeholder_left_box;
    delete button_region;
    delete find_input;
    delete input_region;
    delete main_container;
}

EditorWindow::EditorWindow(int w, int h, int argc, char *argv[], const char *t)
        :Fl_Double_Window(w, h, t), argc(argc), argv(argv), ischanged(0), current_file(nullptr)
{
    /* 0. 主窗口初始化 */
    this->label("SimpleEditor");

    /* 1. 菜单栏初始化 */
    menuitems[0] = {"&File", 0, 0, 0, FL_SUBMENU};
    menuitems[1] = {"&New File", 0,
                    (Fl_Callback *)MenuBar_Utility::newfile_cb,
                    this};

    menuitems[2] = {"&Open File...", FL_COMMAND+'o',
                    (Fl_Callback *)MenuBar_Utility::openfile_cb,
                    this};
    menuitems[3] = {"&Insert File...", FL_COMMAND + 'i',
                    (Fl_Callback *)MenuBar_Utility::insert_cb,this, FL_MENU_DIVIDER};
    menuitems[4] = {"&Save File", FL_COMMAND+'s',
                    (Fl_Callback *)MenuBar_Utility::save_cb,
                    this};
    menuitems[5] = {"&Save File As", FL_COMMAND+FL_SHIFT+'s',
                    (Fl_Callback *)MenuBar_Utility::saveas_cb,
                    this, FL_MENU_DIVIDER};
    menuitems[6] = {"New &View", FL_ALT+'v',
                    (Fl_Callback *)MenuBar_Utility::view_cb,
                    this};
    menuitems[7] = {"&Close View", FL_COMMAND + 'w',
                    (Fl_Callback *)MenuBar_Utility::view_close_cb,
                    this, FL_MENU_DIVIDER};
    menuitems[8] = {"E&xit", FL_COMMAND + 'q',
                    (Fl_Callback *)MenuBar_Utility::exit_cb,
                    this};
    menuitems[9] = {0};
    menuitems[10] = {"&Edit", 0, 0, 0, FL_SUBMENU};
    menuitems[11] = {"&Undo", FL_COMMAND + 'z',
                     (Fl_Callback *)MenuBar_Utility::undo_cb,
                     this};
    menuitems[12] = {"&Redo", FL_COMMAND + 'r',
                     (Fl_Callback *)MenuBar_Utility::redo_cb,
                     this, FL_MENU_DIVIDER};
    menuitems[13] = {};
    menuitems[14] = {"&Search", 0, 0, 0, FL_SUBMENU};
    menuitems[15] = {"&Find", FL_COMMAND + 'f',
                    (Fl_Callback *)MenuBar_Utility::find1_cb,
                    this};
    menuitems[16] = {"&Replace", FL_COMMAND + 'r',
                     (Fl_Callback *)MenuBar_Utility::replace1_cb,
                    this};
    menuitems[17] = {0};

    menubar = new Fl_Menu_Bar(0, 0, this->w(), 30);
    menubar->copy(menuitems);


    /* 2. 文本编辑器 */
    editor = new Fl_Text_Editor(0, menubar->h(), this->w(), this->h()-menubar->h());
    editor->cursor_style(Fl_Text_Display::BLOCK_CURSOR);
    editor->textfont(FL_COURIER);

    text_buffer = new Fl_Text_Buffer;
    text_buffer->add_modify_callback((Fl_Text_Modify_Cb)Editor_Utility::bufferchange_cb
                                        , this);

    editor->buffer(text_buffer);

    editor->end();

    /* replace窗口初始化 */
    replace_dialog = new ReplaceDialog(400, 120);
    replace_dialog->end();
    replace_dialog->replace_all->callback((Fl_Callback *)replace_dialog_replaceall_cb, this);
    replace_dialog->replace_next->callback((Fl_Callback *)replace_dialog_replacenext_cb, this);
    replace_dialog->cancel->callback((Fl_Callback *)replace_dialog_cancel_cb, this);

    /* find窗口初始化 */
    find_dialog = new FindDialog(300, 80);
    find_dialog->end();
    find_dialog->find_button->callback((Fl_Callback *) find_dialog_find_cb, this);
    find_dialog->cancel->callback((Fl_Callback *) find_dialog_cancel_cb, this);
}

EditorWindow::~EditorWindow() // 待实现释放资源
{
}

int Editor_Utility::check_save(EditorWindow *mainwindow)
{
    if (mainwindow->ischanged == 0)
        return 1;

    // 状态改变
    auto op = fl_choice("The current file has not been saved.\n Would you like to save it now?",
                        "Cancel", "Save", "Discard");
    switch (op) {
        case 1:
            // 调用save_as()
            MenuBar_Utility::saveas_cb(nullptr, mainwindow);
            return 1;
        case 2:
            // 摒弃缓冲区内容, 打开的文件是不变的
            mainwindow->text_buffer->remove(0, mainwindow->text_buffer->length());
            mainwindow->ischanged = 0; // 新缓冲区内容应该是不变的
            return 0;
        case 0:
            // cancel, 动作取消，状态保持不变
            return 0;
        default:
            return 0;
    }
    return 0;
}

void MenuBar_Utility::newfile_cb(Fl_Widget *widget, void *data)
{
    // 思路1: 创建一个新状态的组件
    std::cout << "MenuBar_Utility::newfile_cb()" << std::endl;
    // 思路2: 组件不动，但是删除组件text中内容即可
    EditorWindow *mainwindow = reinterpret_cast<EditorWindow *>(data);
    if (!Editor_Utility::check_save(mainwindow))
        return ;
    /* 上面没有处理返回的情况2 所以会remove两次 */
    mainwindow->text_buffer->remove(0, mainwindow->text_buffer->length());
}

void MenuBar_Utility::save_cb(Fl_Widget *widget, void *data)
{
    EditorWindow *mainwindow = reinterpret_cast<EditorWindow *>(data);
    if (mainwindow->current_file == nullptr) {
        std::cerr << "[warning] current_file is not specified" << std::endl;
        saveas_cb(widget, data);
    }

    if (mainwindow->text_buffer->savefile(mainwindow->current_file) != 0) {
        std::cerr << " MenuBar_Utility::save_cb() error occupied" << std::endl;
    } else { // 更新窗口状态
        mainwindow->label(mainwindow->current_file);
    }
}


void MenuBar_Utility::saveas_cb(Fl_Widget *widget, void *data) {
    char *new_file_name = fl_file_chooser("Save File As", "*", nullptr);
    if (new_file_name != nullptr) {
        reinterpret_cast<EditorWindow *>(data)->current_file = new_file_name;
        save_cb(widget, data);
    }
}

void MenuBar_Utility::openfile_cb(Fl_Widget *widget, void *data)
{
    std::cout << "MenuBar_Utility::openfile_cb()" << std::endl;
    EditorWindow *mainwindow = reinterpret_cast<EditorWindow *>(data);
    // 0. 检查当前缓冲区是否需要保存
    if (!Editor_Utility::check_save(mainwindow))
        return ;

    // 1. 打开文件选择器，读取文件内容
    char *open_file_path = fl_file_chooser("Open File", "*", nullptr);
    if (!open_file_path)
        return ;
    mainwindow->current_file = open_file_path;

    // 2. 读取到text_buffer组件中
    auto ins_ret = mainwindow->text_buffer->insertfile(open_file_path,0);
    if (1 == ins_ret) {
        std::cerr << "oped or read failed" << std::endl;
        return ;
    } else if (2 == ins_ret) {
        std::cerr << "reading data error occurred" << std::endl;
        return ;
    } else if (ins_ret != 0) {
        std::cerr << "Other error" << std::endl;
        return ;
    }

    // 3. 调用回调，通知文件内容已经被改变
    mainwindow->text_buffer->call_modify_callbacks();
    mainwindow->label(open_file_path);
}

// 从菜单栏打开ReplaceDialog窗口
void MenuBar_Utility::replace1_cb(Fl_Widget *widget, void *data)
{
    // 显示replace dialog窗口即可
    EditorWindow *mainwindow = reinterpret_cast<EditorWindow *>(data);
    if (mainwindow->replace_dialog) {
        mainwindow->replace_dialog->show(mainwindow->argc, mainwindow->argv);
    }
}

// 从菜单栏打开FindDialog窗口
void MenuBar_Utility::find1_cb(Fl_Widget *widget, void *data)
{
    //
    EditorWindow *mainwindow = reinterpret_cast<EditorWindow *>(data);
    if (mainwindow->find_dialog) {
        mainwindow->find_dialog->show(mainwindow->argc, mainwindow->argv);
    }
}

void MenuBar_Utility::insert_cb(Fl_Widget *widget, void *data)
{
    EditorWindow *mainwindow = reinterpret_cast<EditorWindow *>(data);

    char *file_path = fl_file_chooser("Insert File", "*", nullptr);
    if (!file_path) {
        return ;
    }

    auto insertpos = mainwindow->editor->insert_position();
    mainwindow->text_buffer->insertfile(file_path, insertpos);
}

void MenuBar_Utility::view_cb(Fl_Widget *widget, void *data)
{
    EditorWindow *mainwindow = reinterpret_cast<EditorWindow *>(data);

    if (!Editor_Utility::check_save(mainwindow)) {
        return ;
    }
}

void MenuBar_Utility::view_close_cb(Fl_Widget *widget, void *data)
{
    EditorWindow *mainwindow = reinterpret_cast<EditorWindow *>(data);
    mainwindow->text_buffer->remove(0, mainwindow->text_buffer->length());
    mainwindow->ischanged = 0;
    mainwindow->label("SimpleEditor");
}

void MenuBar_Utility::exit_cb(Fl_Widget *widget, void *data)
{
    exit(0);
}

void MenuBar_Utility::undo_cb(Fl_Widget *widget, void *data)
{
    EditorWindow *mainwindow = reinterpret_cast<EditorWindow *>(data);

    if (mainwindow->text_buffer->can_undo()) {
        mainwindow->text_buffer->undo();
    }
}

void MenuBar_Utility::redo_cb(Fl_Widget *widget, void *data)
{
    EditorWindow *mainwindow = reinterpret_cast<EditorWindow *>(data);

    if (mainwindow->text_buffer->can_redo()) {
        mainwindow->text_buffer->redo();
    }
}

void Editor_Utility::bufferchange_cb(int pos, int nInserted,
                                   int nDeleted, int nRestyled,
                                   const char *deletedText,
                                   void *cbArg)
{
    printf("pos=%d, nInserted=%d, nDeleted=%d nRestlyed=%d deletedText=%s\n",
           pos, nInserted, nDeleted, nRestyled, deletedText);

    auto *mainwindow = reinterpret_cast<EditorWindow *>(cbArg);

    if (mainwindow->current_file == nullptr) {
        mainwindow->label(u8"*无标题");
    } else {
        std::string str("*");
        str += mainwindow->current_file;
        mainwindow->label(str.c_str());
    }

    if (0 == mainwindow->ischanged) {
        mainwindow->ischanged = 1;
    }
}