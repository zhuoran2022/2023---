#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <algorithm>
#include <string>
//#include "header1.h"
#include <stdio.h>                  /* for EOF */
//#include <string.h>                 /* for strchr() */
//#include<CL/sycl.hpp>

//using namespace sycl;

//bool debug1006 = false;
int gpu_iteration = 0;

/* static (global) variables that are specified as exported by getopt() */
char* optarg = NULL;    /* pointer to the start of the option argument  */
int   optind = 1;       /* number of the next argv[] to be evaluated    */
int   opterr = 1;       /* non-zero if a question mark should be returned
                           when a non-valid option character is detected */

                           /* handle possible future character set concerns by putting this in a macro */
#define _next_char(string)  (char)(*(string+1))

int getopt(int argc, char* argv[], char* opstring)
{
    static char* pIndexPosition = NULL; /* place inside current argv string */
    char* pArgString = NULL;        /* where to start from next */
    char* pOptString;               /* the string in our program */


    if (pIndexPosition != NULL) {
        /* we last left off inside an argv string */
        if (*(++pIndexPosition)) {
            /* there is more to come in the most recent argv */
            pArgString = pIndexPosition;
        }
    }

    if (pArgString == NULL) {
        /* we didn't leave off in the middle of an argv string */
        if (optind >= argc) {
            /* more command-line arguments than the argument count */
            pIndexPosition = NULL;  /* not in the middle of anything */
            return EOF;             /* used up all command-line arguments */
        }

        /*---------------------------------------------------------------------
         * If the next argv[] is not an option, there can be no more options.
         *-------------------------------------------------------------------*/
        pArgString = argv[optind++]; /* set this to the next argument ptr */

        if (('/' != *pArgString) && /* doesn't start with a slash or a dash? */
            ('-' != *pArgString)) {
            --optind;               /* point to current arg once we're done */
            optarg = NULL;          /* no argument follows the option */
            pIndexPosition = NULL;  /* not in the middle of anything */
            return EOF;             /* used up all the command-line flags */
        }

        /* check for special end-of-flags markers */
        if ((strcmp(pArgString, "-") == 0) ||
            (strcmp(pArgString, "--") == 0)) {
            optarg = NULL;          /* no argument follows the option */
            pIndexPosition = NULL;  /* not in the middle of anything */
            return EOF;             /* encountered the special flag */
        }

        pArgString++;               /* look past the / or - */
    }

    if (':' == *pArgString) {       /* is it a colon? */
        /*---------------------------------------------------------------------
         * Rare case: if opterr is non-zero, return a question mark;
         * otherwise, just return the colon we're on.
         *-------------------------------------------------------------------*/
        return (opterr ? (int)'?' : (int)':');
    }
    else if ((pOptString = strchr(opstring, *pArgString)) == 0) {
        /*---------------------------------------------------------------------
         * The letter on the command-line wasn't any good.
         *-------------------------------------------------------------------*/
        optarg = NULL;              /* no argument follows the option */
        pIndexPosition = NULL;      /* not in the middle of anything */
        return (opterr ? (int)'?' : (int)*pArgString);
    }
    else {
        /*---------------------------------------------------------------------
         * The letter on the command-line matches one we expect to see
         *-------------------------------------------------------------------*/
        if (':' == _next_char(pOptString)) { /* is the next letter a colon? */
            /* It is a colon.  Look for an argument string. */
            if ('\0' != _next_char(pArgString)) {  /* argument in this argv? */
                optarg = &pArgString[1];   /* Yes, it is */
            }
            else {
                /*-------------------------------------------------------------
                 * The argument string must be in the next argv.
                 * But, what if there is none (bad input from the user)?
                 * In that case, return the letter, and optarg as NULL.
                 *-----------------------------------------------------------*/
                if (optind < argc)
                    optarg = argv[optind++];
                else {
                    optarg = NULL;
                    return (opterr ? (int)'?' : (int)*pArgString);
                }
            }
            pIndexPosition = NULL;  /* not in the middle of anything */
        }
        else {
            /* it's not a colon, so just return the letter */
            optarg = NULL;          /* no argument follows the option */
            pIndexPosition = pArgString;    /* point to the letter we're on */
        }
        return (int)*pArgString;    /* return the letter that matched */
    }
}

const int N = 9;
const int EMPTY = 0;

// 函数原型
bool solveSudoku(std::vector<std::vector<int>>& grid);
bool findEmptyLocation(const std::vector<std::vector<int>>& grid, int& row, int& col);
bool isValid(const std::vector<std::vector<int>>& grid, int row, int col, int num);
void generateSudoku(std::vector<std::vector<int>>& grid);
void writeSudokuToFile(const std::vector<std::vector<int>>& grid, const std::string& filename);
void readSudokuFromFile(std::vector<std::vector<int>>& grid, const std::string& filename);
void printSudoku(const std::vector<std::vector<int>>& grid);
void playSudoku();

// 数独求解函数
bool solveSudoku(std::vector<std::vector<int>>& grid) {
    int row, col;
    if (!findEmptyLocation(grid, row, col))
        return true;  // 所有空格已填满，数独已解决

    // 尝试填入数字
    for (int num = 1; num <= 9; num++) {
        if (isValid(grid, row, col, num)) {
            grid[row][col] = num;

            if (solveSudoku(grid))
                return true;  // 递归解决剩余空格

            grid[row][col] = EMPTY;  // 回溯，尝试下一个数字
        }
    }
    return false;  // 无解
}

// 寻找数独中的空格
bool findEmptyLocation(const std::vector<std::vector<int>>& grid, int& row, int& col) {
    for (row = 0; row < N; row++) {
        for (col = 0; col < N; col++) {
            //if (grid[row][col] == '$')
            if (grid[row][col] == EMPTY)
            {
                //std::cout << "get $!!\n";
                return true;
            }
        }
    }
    return false;
}

// 检查数字是否在行、列和宫内重复
bool isValid(const std::vector<std::vector<int>>& grid, int row, int col, int num) {
    // 检查行
    for (int i = 0; i < N; i++) {
        if (grid[row][i] == num)
            return false;
    }

    // 检查列
    for (int i = 0; i < N; i++) {
        if (grid[i][col] == num)
            return false;
    }

    // 检查宫
    int startRow = row - row % 3;
    int startCol = col - col % 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (grid[i + startRow][j + startCol] == num)
                return false;
        }
    }

    return true;  // 数字不重复
}

// 生成数独终局
void generateSudoku(std::vector<std::vector<int>>& grid) {
    std::vector<int> numbers = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    std::random_device rd;
    std::mt19937 rng(rd());
    std::shuffle(numbers.begin(), numbers.end(), rng);

    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
            grid[row][col] = numbers[(col + row * 3 + row / 3) % N];
        }
    }
}

// 将数独终局写入文件
void writeSudokuToFile(const std::vector<std::vector<int>>& grid, const std::string& filename) {
    std::ofstream file(filename, std::ios::out | std::ios::app);
    if (file.is_open()) {
        for (const auto& row : grid) {
            for (int num : row) {
                file << num << " ";
            }
            file << "\n";
        }
        file << "=============================\n";
        file.close();
        std::cout << "数独终局已写入文件：" << filename << std::endl;
    }
    else {
        std::cout << "无法打开文件：" << filename << std::endl;
    }
}

void generateGame(std::vector<std::vector<int>>& grid, int difficultyLevel, int empty_num) {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> dist(0, N - 1);

    int numToRemove = 0;
    if (empty_num != 0)
    {
        numToRemove = empty_num;
    }
    else
    {
        switch (difficultyLevel) {
            case 1: // 简单
                numToRemove = 30;
                break;
            case 2: // 中等
                numToRemove = 40;
                break;
            case 3: // 困难
                numToRemove = 50;
                break;
            default:
                std::cout << "无效的难度级别。" << std::endl;
                return;
        }
    }

    while (numToRemove > 0) {
        int row = dist(rng);
        int col = dist(rng);
        if (grid[row][col] != EMPTY) {
            grid[row][col] = EMPTY;
            numToRemove--;
        }
    }
}

void displayGrid(std::vector<std::vector<int>>& grid)
{
    std::cout << "displayGrid!!\n";
    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid[0].size(); j++)
        {
            if (grid[i][j] != 0)
            {
                std::cout << grid[i][j] << " ";
            }
            else
            {
                std::cout << "$ ";
            }
        }
        std::cout << "\n";
    }
}
#include<iostream>
// 从文件中读取数独问题
int readSudokuFromFile(std::vector<std::vector<int>>& grid, const std::string& filename, int lastpos = 0) {
    std::ifstream file(filename);
    int current_pos = 0;
    if (file.is_open()) {
        file.seekg(lastpos, std::ios::beg);
        displayGrid(grid);
        char c;
        for (auto& row : grid) {
            for (int& num : row) {
                file >> c;
                num = (int)c - '0';
                if (!(num >= 1 && num <= 9))
                {
                    num = 0;
                }
            }
        }
        current_pos = file.tellg();
        file.close();
        std::cout << "从文件读取数独问题：" << filename << std::endl;

        displayGrid(grid);
    }
    else {
        std::cout << "无法打开文件：" << filename << std::endl;
    }
    return current_pos;
}

// 打印数独
void printSudoku(const std::vector<std::vector<int>>& grid) {
    for (const auto& row : grid) {
        for (int num : row) {
            std::cout << num << " ";
        }
        std::cout << "\n";
    }
}

// 玩数独游戏
void playSudoku() {
    std::vector<std::vector<int>> grid(N, std::vector<int>(N, EMPTY));
    generateSudoku(grid);

    std::cout << "欢迎来到数独游戏！请输入行、列和要填入的数字，用空格隔开。输入0 0 0退出游戏。\n";

    while (true) {
        std::cout << "当前数独局面：\n";
        printSudoku(grid);

        int row, col, num;
        std::cout << "请输入行、列和数字（空格隔开）：";
        std::cin >> row >> col >> num;

        if (row == 0 && col == 0 && num == 0) {
            std::cout << "游戏已退出。\n";
            break;
        }

        if (row < 1 || row > 9 || col < 1 || col > 9 || num < 1 || num > 9) {
            std::cout << "无效的输入，请重新输入。\n";
            continue;
        }

        if (grid[row - 1][col - 1] != EMPTY) {
            std::cout << "该位置已经填入数字，请重新选择。\n";
            continue;
        }

        grid[row - 1][col - 1] = num;

        if (solveSudoku(grid)) {
            std::cout << "恭喜！数独已解决。\n";
            break;
        }
    }
}

int how_much_shudu(std::string filename)
{
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Failed to open the file in how_much_shudu." << std::endl;
        return -1;
    }

    std::string line;
    int lineCount = 0;
    while (std::getline(file, line)) {
        lineCount++;
    }

    //std::cout << "The file has " << lineCount << " lines." << std::endl;

    file.close();
    return lineCount / 9;
}

int main(int argc, char* argv[]) {
    // 初始化要读入的参数
    int final_num = 0;// -c 需要的数独终盘数量
    std::string filename = "";// -s 需要解的数独棋盘文件路径
    int game_num = 1;// -n 需要的游戏数量
    int degree = 1;// -m 生成游戏的难度，分为1-3
    int empty_num = 0;// -r 生成游戏中挖空的数量范围
    bool onlySolution = false;// -u 代表生成的解唯一，将onlySolution置true; onlySolution为false则代表解不唯一
    // 读取并解析输入的参数
    char* c = new char[6];
    int ch;
    c[0] = 'c'; c[1] = ':'; c[2] = 's'; c[3] = ':'; c[4] = 'n'; c[5] = ':'; c[6] = 'm'; c[7] = ':'; c[8] = 'r'; c[9] = ':'; c[10] = 'u'; c[11] = '\0';
    std::cout << "argc = " << argc << "; argv = " << argv << "\n";
    while ((ch = getopt(argc, argv, c)) != -1) {
        switch (ch) {
            case 'c':
                final_num = atoll(optarg);
                std::cout << "final_num = " << final_num << "\n";
                break;
            case 's':
                filename = optarg;
                std::cout << "filename = " << filename << "\n";
                break;
            case 'n':
                game_num = atoll(optarg);
                std::cout << "game_num = " << game_num << "\n";
                break;
            case 'm':
                degree = atoll(optarg);
                std::cout << "degree = " << degree << "\n";
                break;
            case 'r':
                empty_num = atoll(optarg);
                std::cout << "empty_num = " << empty_num << "\n";
                break;
            case 'u':
                onlySolution = atoll(optarg);
                std::cout << "onlySolution = " << onlySolution << "\n";
                break;
            default:
                break;
        }
    }
    // 1. 生成数独终局
    if (final_num > 0)
    {
        std::string txt = ".txt";
        std::vector<std::vector<int>> grid(N, std::vector<int>(N, EMPTY));
        for (int i = 0; i < final_num; i++)
        {
            generateSudoku(grid);
            std::string path = "shudu_final" + std::to_string(i) + txt;
            writeSudokuToFile(grid, path);
        }
    }

    // 2. 读取文件内的数独问题，求解并将结果输出至文件
    if (filename != "")
    {
        std::vector<std::vector<int>> problem(N, std::vector<int>(N, EMPTY));
        int last_pos = 0;
        int num_need_solve = how_much_shudu(filename);
        // 清空sudoku.txt的内容
        fopen("sudoku.txt", "w+");
        // 读取需要解的数独棋盘文件并将若干个解写入sudoku.txt
        for (int i = 0; i < num_need_solve; i++)
        {
            last_pos = readSudokuFromFile(problem, filename, last_pos);

            solveSudoku(problem);
            //std::string path = "shudu_solution" + std::to_string(i) + txt;
            writeSudokuToFile(problem, "sudoku.txt");
        }

        std::cout << "生成的数独终局已保存至sudoku.txt\n";
        std::cout << "数独问题的解已保存至solution.txt\n";
    }

    // 3.生成数独游戏并让计算机解答
    if (game_num > 0)
    {
        std::cout << "生成的数独游戏及求解如下：\n";
        std::cout << "game_num = " << game_num << "\n";
        std::vector<std::vector<int>> grid(N, std::vector<int>(N, EMPTY));
        for (int i = 0; i < game_num; i++)
        {
            generateSudoku(grid);
            generateGame(grid, degree, empty_num);
            std::cout << "=================数独题目如下================\n";
            displayGrid(grid);
            solveSudoku(grid);
            std::cout << "=================数独求解如下================\n";
            displayGrid(grid);
            std::cout << "---------------------------------------------\n";
        }
    }

     //playSudoku();

    return 0;
}
