#pragma once

#include <string>
#include <map>

// Tic-Tac-Toe
namespace TTT {

  class TicTacToe
  {
  public:
    TicTacToe();
    ~TicTacToe() = default;

    bool PlayChess(const std::string& posName);

    bool CheckWin();

    inline bool GetOffensive() const
    {
      return m_Offensive;
    }

    inline void SetOffensive()
    {
      m_Offensive = !m_Offensive;
    }

    inline bool GetWinFlag() const
    {
      return m_WinFlag;
    }

  public:
    // For Test
    void print() const;

  private:
    void UpdateChessMap(int* chessPos);

  private:
    // True is turn to [xingxing] operator
    // False is turn to [yueliang] operator
    bool m_Offensive = true;

    // The initialzation is 0
    // 1 indicates the location of the [xingxing] chess
    // 2 indicates the location of the [yueliang] chess
    // int m_ChessMap[3][3];
    int (*m_ChessMap)[3] = new int[3][3]{0};

    std::map<std::string, int*> m_MapIndex;

    bool m_WinFlag = false;
  };

} // namespace TTT