#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <algorithm>

const int N = 9;
const int EMPTY = 0;

// 函数原型
void generateSudoku(std::vector<std::vector<int>>& grid);
void writeSudokuToFile(const std::vector<std::vector<int>>& grid, const std::string& filename);

// 生成数独终局
void generateSudoku(std::vector<std::vector<int>>& grid) {
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9};
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
    std::ofstream file(filename);
    if (file.is_open()) {
        for (const auto& row : grid) {
            for (int num : row) {
                file << num << " ";
            }
            file << "\n";
        }
        file.close();
        std::cout << "数独终局已写入文件：" << filename << std::endl;
    } else {
        std::cout << "无法打开文件：" << filename << std::endl;
    }
}

int main() {
    // 1. 生成数独终局
    std::vector<std::vector<int>> grid(N, std::vector<int>(N, EMPTY));
    generateSudoku(grid);
    writeSudokuToFile(grid, "sudoku.txt");

    return 0;
}
