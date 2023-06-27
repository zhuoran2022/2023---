#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include <random>
#include <algorithm>
#include <string>
#include<assert.h>
#include <stdio.h>                  /* for EOF */
#include <io.h>

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
bool solveSudoku(std::vector<std::vector<int>>& grid, int& solutionCount);
bool findEmptyLocation(const std::vector<std::vector<int>>& grid, int& row, int& col);
bool isValid(const std::vector<std::vector<int>>& grid, int row, int col, int num);
void generateSudoku(std::vector<std::vector<int>>& grid);
void writeSudokuToFile(const std::vector<std::vector<int>>& grid, const std::string& filename);
void readSudokuFromFile(std::vector<std::vector<int>>& grid, const std::string& filename);

// 数独求解函数
bool solveSudoku(std::vector<std::vector<int>>& grid, int& solutionCount) {
    int row, col;
    if (!findEmptyLocation(grid, row, col)) {
        solutionCount++;
        return true;  // 所有空格已填满，数独已解决
    }

    // 尝试填入数字
    for (int num = 1; num <= 9; num++) {
        if (isValid(grid, row, col, num)) {
            grid[row][col] = num;

            if (solveSudoku(grid, solutionCount) && solutionCount > 1) {
                return true;  // 存在多个解，停止搜索
            }

            grid[row][col] = EMPTY;  // 回溯，尝试下一个数字
        }
    }
    return false;  // 无解
}

// 检查数独终局是否具有唯一解
bool hasUniqueSolution(const std::vector<std::vector<int>>& grid) {
    int solutionCount = 0;
    std::vector<std::vector<int>> clonedGrid = grid;

    // 尝试求解数独，并记录解的数量
    solveSudoku(clonedGrid, solutionCount);

    return solutionCount == 1;
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
    std::string filePath = filename;
    std::ofstream file(filePath, std::ios::out | std::ios::app);
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

void generateGame(std::vector<std::vector<int>>& grid, int difficultyLevel, int empty_num, bool uniqueSolution) {
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
            int temp = grid[row][col];
            grid[row][col] = EMPTY;
            if (uniqueSolution && !hasUniqueSolution(grid)) {
                // 如果唯一解为 true 且数独终局不具有唯一解，则恢复挖去的数字
                grid[row][col] = temp;
            }
            else {
                numToRemove--;
            }
            //numToRemove--;
        }
    }
}

void displayGrid(std::vector<std::vector<int>>& grid)
{
    std::cout << "displayGrid...\n";
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
    int empty_down = 0, empty_up = 0, empty_num = 0;// -r 生成游戏中挖空的数量范围
    std::string empty_region = "";
    bool onlySolution = false;// -u 代表生成的解唯一，将onlySolution置true; onlySolution为false则代表解不唯一
    bool hasn = false;
    // 读取并解析输入的参数
    char* c = nullptr;
    try {
        c = new char[6]; // 分配内存成功，可以继续使用 c 指向的内存
    } catch (const std::bad_alloc& e) { // 分配内存失败，处理异常
        std::cout << "分配内存失败: " << e.what() << std::endl;
        delete[] c;
    }
    int ch;
    c[0] = 'c'; c[1] = ':'; c[2] = 's'; c[3] = ':'; c[4] = 'n'; c[5] = ':'; c[6] = 'm'; c[7] = ':'; c[8] = 'r'; c[9] = ':'; c[10] = 'u'; c[11] = '\0';
    std::cout << "argc = " << argc << "; argv = " << argv << "\n";
    int pos = -1;
    while ((ch = getopt(argc, argv, c)) != -1) {
        switch (ch) {
            case 'c':
                final_num = atoll(optarg);
                if(final_num<0||final_num>1000000){
                    std::cout<<"-c number should be >=0 and <= 1000000.\n";
                    assert(0);
                }
                std::cout << "final_num = " << final_num << "\n";
                break;
            case 's':
                filename = optarg;
                std::cout << "filename = " << filename << "\n";
                break;
            case 'n':
                game_num = atoll(optarg);
                if(game_num<0 || game_num >10000){
                    std::cout<<"-n number should be >=0 and <= 10000.\n";
                    assert(0);
                }
                std::cout << "game_num = " << game_num << "\n";
                hasn = true;
                break;
            case 'm':
                if(hasn){
                    degree = atoll(optarg);
                    std::cout << "degree = " << degree << "\n";
                }
                else {
                    std::cout<<"must have arg -n!\n";
                    assert(0);
                }
                break;
            case 'r':
                if(hasn){
                    empty_region = optarg;
                    pos = empty_region.find("~");
                    empty_down = std::stoi(empty_region.substr(0, pos));
                    empty_up = std::stoi(empty_region.substr(pos + 1, empty_region.length() - pos - 1));
                    std::cout << "empty_down = " << empty_down << "\n";
                    std::cout << "empty_up = " << empty_up << "\n";
                    empty_num = (rand() % (empty_up + 1 - empty_down)) + empty_down;
                    std::cout << "empty_num = " << empty_num << "\n";
                    if(empty_down <= 20 || empty_up >= 55){
                        std::cout<<"-r number should range from 20 to 55.\n";
                        assert(0);
                    }
                }
                else {
                    std::cout<<"must have arg -n!\n";
                    assert(0);
                }
                break;
            case 'u':
                if(hasn){
                    onlySolution = true;
                    std::cout << "onlySolution = " << onlySolution << "\n";
                }
                else {
                    std::cout<<"must have arg -n!\n";
                    assert(0);
                }
                break;
            default:
                std::cout<<"incorrect input!\n";
                assert(0);
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
            int solutionCount = 0;
            solveSudoku(problem, solutionCount);
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
            generateGame(grid, degree, empty_num, onlySolution);
            std::cout << "=================数独题目如下================\n";
            displayGrid(grid);
            std::string path = "shuduques" + std::to_string(i) + ".txt";
            writeSudokuToFile(grid, path);
            int solutionCount = 0;
            solveSudoku(grid, solutionCount);
            std::cout << "=================数独求解如下================\n";
            displayGrid(grid);
            path = "shuduans" + std::to_string(i) + ".txt";
            writeSudokuToFile(grid, path);
            std::cout << "---------------------------------------------\n";
        }
    }

    return 0;
}
