#include <iostream>
#include <bit>

using u8 = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;
using u64 = unsigned long long;

class Sudoku {
  u16 board[9][9];

public:
  Sudoku() {
    for (u8 x = 0; x < 9; x++)
      for (u8 y = 0; y < 9; y++)
        board[x][y] = 0b111111111;
  }

  void update(u8 x, u8 y, u16 value) {
    board[x][y] = value;
    for (u8 i = 0; i < 9; i++)
      if (i != x)
        board[i][y] &= ~value;
    for (u8 i = 0; i < 9; i++)
      if (i != y)
        board[x][i] &= ~value;
    for (u8 i = x / 3 * 3; i < x / 3 * 3 + 3; i++)
      for (u8 j = y / 3 * 3; j < y / 3 * 3 + 3; j++)
        if (i != x && j != y)
          board[i][j] &= ~value;
  }

  bool solve(u8 x = 0, u8 y = 0) {
    u16 originalRow[9], originalColumn[9];
    u16 originalBox[3][3];
    for (u8 i = 0; i < 9; i++) {
      originalRow[i] = board[x][i];
      originalColumn[i] = board[i][y];
    }
    for (u8 i = x / 3 * 3; i < x / 3 * 3 + 3; i++)
      for (u8 j = y / 3 * 3; j < y / 3 * 3 + 3; j++)
        originalBox[i - x / 3 * 3][j - y / 3 * 3] = board[i][j];
    for (u16 value = board[x][y]; value; value &= (value - 1)) {
      update(x, y, 1 << std::countr_zero(value));
      if (x == 8 && y == 8) return true;
      if (solve((x + 1) % 9, y + (x + 1) / 9)) return true;
      for (u8 i = 0; i < 9; i++) {
        board[x][i] = originalRow[i];
        board[i][y] = originalColumn[i];
      }
      for (u8 i = x / 3 * 3; i < x / 3 * 3 + 3; i++)
        for (u8 j = y / 3 * 3; j < y / 3 * 3 + 3; j++)
          board[i][j] = originalBox[i - x / 3 * 3][j - y / 3 * 3];
    }
    return false;
  }

  friend std::ostream& operator<<(std::ostream& os, const Sudoku& sudoku) {
    for (u8 x = 0; x < 9; x++) {
      for (u8 y = 0; y < 9; y++) {
        if (isKnown(sudoku.board[x][y]))
          os << std::countr_zero(sudoku.board[x][y]) + 1;
        else
          os << ' ';
      }
      os << std::endl;
    }
    return os;
  }

  friend std::istream& operator>>(std::istream& is, Sudoku& sudoku) {
    char c[82];
    is >> c;
    for (u8 x = 0; x < 9; x++)
      for (u8 y = 0; y < 9; y++)
        sudoku.board[x][y] = 0b111111111;
    for (u8 x = 0; x < 9; x++)
      for (u8 y = 0; y < 9; y++)
        if (std::isdigit(c[x * 9 + y]) && c[x * 9 + y] != '0')
          sudoku.update(x, y, 1 << (c[x * 9 + y] - '1'));
    return is;
  }

  static bool isKnown(u16 value) {
    return value && !(value & (value - 1));
  }
};


int main() {
  Sudoku sudoku;
  std::cin >> sudoku;
  if (sudoku.solve())
    std::cout << sudoku << std::endl;
  return 0;
}
