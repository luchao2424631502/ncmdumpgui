#include <iostream>
#include <filesystem>
#include <cstring>
#include <memory>
#include <thread>
#include <vector>
#include <windows.h>

// c header
#include <getopt.h>
#include <unistd.h>
#include <dirent.h>
#include <cstdlib>
#include <cstring>

// 3-rd lib
#include "ThreadPool.h"

// ncm
#include "dump.h"

class OptParse {
public:
    OptParse() = delete;
    ~OptParse() = default;
    OptParse(int argc, char *argv[], const char *format):argc(argc),argv(argv),format(format) {}
public:
    int getopt();
    char *getoptarg();
private:
    int argc;
    char **argv;
    const char *format;
};

int OptParse::getopt() {
    return ::getopt(argc, argv, format);
}

char *OptParse::getoptarg() {
    return optarg;
}

double twork(double n1, double n2) {
    double ans = 1.0;
    for (size_t i = 0; i < (1 << 31); i++) {
        double tmp = ans * n1 * n2;
        long tmpl = static_cast<long>(tmp);
        tmpl %= 100;
        tmp = static_cast<double>(tmpl);
        ans += tmp;
    }
    return ans;
}

void test_func() {
    std::vector<std::future<double>> all_handle;

    ThreadPool pool(20);
    pool.init();

    for (size_t i = 0; i < 20; i++)
        all_handle.emplace_back(pool.submit(twork, 7.8, 9.9));

    for (auto &entry : all_handle) {
        std::cout <<  entry.get() << std::endl;
    }
    pool.shutdown();
}

#define GRAPHICS
#ifndef GRAPHICS
int main(int argc, char *argv[]) {
    // std::cout << msg << std::endl;
    std::cout << argv[1] << std::endl;
    /* 解析命令行 */
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    int option, is_dir = 0, thread_nums = sysInfo.dwNumberOfProcessors;
	char *work_dir = nullptr;
    bool exit_flag = false;

    // 加入到线程池中的句柄
    std::vector<std::future<double>> all_handle;

    OptParse optparse(argc, argv, "o:hd:j:");
	// while ((option = getopt(argc, argv, "o:hd:j:")) != -1) {
    while ((option = optparse.getopt()) != -1) {
		switch(option) {
		case 'h':
			printf("Usage %s [OPTIONS] FILES \n"
				"Convert CloudMusic ncm files to mp3/flac files\n\n"
				"Examples:\n"
				"\tncm2dump test.ncm\n"
				"\tncm2dump test1.ncm test2.ncm test3.ncm\n"
				"\tncm2dump -d ./download/\n"
				"\tncm2dump -j 6 -d ./download/\n\n"
				"Options:\n"
				"\t-h                   display HELP and EXIT\n"
				"\t-j [N]               start N threads to convert\n"
				"\t-d <Directory>       batch convert ncm in a specified <directory>\n"
				"\t-o <file>            place out file in <file>\n"
				"", argv[0]);
            exit_flag = true;
			break;

		case 'o':
			// printf("arg is %s\n", optarg);
            std::cout << "std::cout arg is " << optparse.getoptarg() << "\n";
			break;
		case 'd':
			is_dir = 1;
			work_dir = optarg;
			printf("arg is %s\n", optarg);
			break;
		case 'j':
            thread_nums = strtol(optparse.getoptarg(), nullptr, 10);
			printf("arg is %d\n", thread_nums);
			break;
		default: // 选项和参数不规范直接exit
            exit_flag = true;
		}
	}

    // 参数解析失败
    if (exit_flag)
        return -1;

	// 没有输入文件
	if (optind == argc && !is_dir) {
		printf("%s: fatal error: no input files. USE %s -h\n",
				argv[0], argv[0]);
		// goto END;
        exit_flag = true;
	}

    if (exit_flag)
        return -1;

	char **file_queue = &argv[optind];
	size_t file_nums = argc - optind;

	// for (size_t index = optind; index < argc; index++) {
	// 	printf("%s\n", argv[index]);
	// }

	// 创建线程池
    ThreadPool pool(thread_nums);
    pool.init();

	char **file_name = nullptr;
	int count = 0;
	if (!is_dir) {
		for (size_t i = 0; i < file_nums; i++) {
			work_convert(file_queue[i]);
			// 过滤不合法文件名
			if (0 == strcmp(file_queue[i] + strlen(file_queue[i]) -  4,
					       	".ncm")) {
				// thpool_add_work(pool, (wrap_func)work_convert,
				// 		file_queue[i]);

                // pool.submit(work_convert, file_queue[i]);

				printf("[SUCCESS] %s\n", file_queue[i]);
			}
			else
				printf("[INVALID FILE] %s\n", file_queue[i]);
		}
	} else {
		// 0. 切换到子目录
		if (chdir(work_dir) == -1) {
			printf("chdir ERROR\n");
			goto END;
		}

		// 1. 遍历目录, 得到目录下面的所有文件
		DIR *dir = opendir("./");
		if (!dir) {
			printf("opendir ERROR\n");
			goto END;
		}

		struct dirent *entry;
		while ((entry = readdir(dir)) != nullptr) {
			if (strcmp(entry->d_name + strlen(entry->d_name) - 4,
					       	".ncm"))
				continue;
			count++;
		}

		// file_name = malloc(sizeof(char *) * count);
        file_name = reinterpret_cast<char **>( malloc(sizeof(char *) * count) );
		size_t index = 0;

		rewinddir(dir);
		while ((entry = readdir(dir)) != nullptr) {
			if (strcmp(entry->d_name + strlen(entry->d_name) - 4,
						".ncm"))
				continue;
			file_name[index] = strdup(entry->d_name);
			// thpool_add_work(pool, (wrap_func)work_convert,
			// 		file_name[index]);
			index++;
		}

	}

	if (is_dir) {
        for (size_t i = 0; i < count; i++)
            free(file_name[i]);
        if (!file_name)
            free(file_name);
    }

    // 同步线程池中所有线程
    for (auto &thread : all_handle) {
        thread.get();
    }

END:
    pool.shutdown();
    return 0;
}
#else

#include "app.h"
#include "editor.h"

int main(int argc, char *argv[])
{
    std::cout << "start" << std::endl;
    EditorWindow *myEditor = new EditorWindow(800, 500, argc, argv);
    myEditor->show(argc, argv);

    // dialog空间测试
    // ReplaceDialog *dialog = new ReplaceDialog(400,120);
    // dialog->show(argc, argv);

    return Fl::run();
}
#endif