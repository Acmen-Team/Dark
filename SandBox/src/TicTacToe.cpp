#include "TicTacToe.h"

#include <Dark.h>

namespace TTT {

  TicTacToe::TicTacToe()
  {
    // Initialize chess map index
    const std::string preChessName = "qizi";
    int index                      = 1;
    for (size_t i = 0; i < 3; ++i)
    {
      for (size_t j = 0; j < 3; ++j)
      {
        int* pos     = &m_ChessMap[i][j];
        auto tmpName = preChessName + std::to_string(index);
        m_MapIndex.insert(std::pair<std::string, int*>(tmpName, pos));
        index++;
      }
    }
  }

  bool TicTacToe::PlayChess(const std::string& posName)
  {
    if (m_MapIndex.empty())
    {
      DK_CORE_ERROR("TicTacToe Error: Map Index nullptr");
      return false;
    }

    auto mapIndex = m_MapIndex.find(posName);
    if (mapIndex == m_MapIndex.end())
    {
      DK_CORE_ERROR("TicTacToe Error: Map Index Out Of Bounds");
      return false;
    }

    // Cherboard position index information
    auto pos = mapIndex->second;
    if (*pos == 0)
    {
      // Update chess map
      UpdateChessMap(pos);
      return true;
    }

    return false;
  }

  void TicTacToe::UpdateChessMap(int* chessPos)
  {
    if (m_Offensive)
    {
      *chessPos = 1;
    }
    else
    {
      *chessPos = 2;
    }
  }

  bool TicTacToe::CheckWin()
  {
    // horizontal
    for (size_t i = 0; i < 3; ++i)
    {
      if (m_ChessMap[i][0] == m_ChessMap[i][1] && m_ChessMap[i][1] == m_ChessMap[i][2])
      {
        if (m_ChessMap[i][0] == 0)
          continue;
        m_WinFlag = true;
        return true;
      }
    }

    // vertical
    for (size_t i = 0; i < 3; ++i)
    {
      if (m_ChessMap[0][i] == m_ChessMap[1][i] && m_ChessMap[1][i] == m_ChessMap[2][i])
      {
        if (m_ChessMap[0][i] == 0)
          continue;
        m_WinFlag = true;
        return true;
      }
    }

    // forward slash
    if (m_ChessMap[0][0] == m_ChessMap[1][1] && m_ChessMap[1][1] == m_ChessMap[2][2])
    {
      if (m_ChessMap[0][0] == 0)
        return false;
      m_WinFlag = true;
      return true;
    }

    // back slash
    if (m_ChessMap[0][2] == m_ChessMap[1][1] && m_ChessMap[1][1] == m_ChessMap[2][0])
    {
      if (m_ChessMap[0][2] == 0)
        return false;
      m_WinFlag = true;
      return true;
    }

    return false;
  }

  void TicTacToe::print() const
  {
    // Print Chess Map
    for (size_t i = 0; i < 3; ++i)
    {
      for (size_t j = 0; j < 3; ++j)
      {
        std::cout << m_ChessMap[i][j] << "\t";
      }
      std::cout << std::endl;
    }

    // Print Chess Map Index
    if (!m_MapIndex.empty())
    {
      for (auto item : m_MapIndex)
      {
        std::cout << item.first << ": " << item.second << std::endl;
      }
    }
  }

} // namespace TTT
