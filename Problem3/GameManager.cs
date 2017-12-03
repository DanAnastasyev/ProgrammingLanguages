using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Problem3 {
    public class GameManager {
        private static readonly Lazy<GameManager> _instance =
            new Lazy<GameManager>(() => new GameManager());

        private enum CellType {
            X,
            Zero,
            Empty
        }

        public const int BoardSize = 600;
        private const int CellsCount = 10;
        private const int RequiredToWin = 5;
        private const int CellSize = BoardSize / CellsCount;
        private readonly List<List<CellType>> _cells = new List<List<CellType>>();
        private readonly List<string> _players = new List<string>();
        private bool _isFirstPlayerTurn = true;

        public GameManager() {
            for (int i = 0; i < CellsCount; ++i) {
                _cells.Add(new List<CellType>());
                for (int j = 0; j < CellsCount; ++j) {
                    _cells[i].Add(CellType.Empty);
                }
            }
        }

        public static GameManager Instance => _instance.Value;

        public bool AddPlayer(string playerId) {
            if (_players.Count >= 2) {
                return false;
            }
            _players.Add(playerId);
            return true;
        }

        public void OnClick(int x, int y, string playerId, IGameHandler handler) {
            if (_players.Count < 2) {
                handler.BroadcastMessage(true,
                    "Please, wait for the second player before starting the game");
                return;
            }
            var i = x / CellSize;
            var j = y / CellSize;
            if (_isFirstPlayerTurn && _players[0] == playerId) {
                _cells[i][j] = CellType.Zero;
                handler.DrawZero(i, j);
                _isFirstPlayerTurn = false;
            } else if (!_isFirstPlayerTurn && _players[1] == playerId) {
                _cells[i][j] = CellType.X;
                handler.DrawX(i, j);
                _isFirstPlayerTurn = true;
            } else {
                handler.BroadcastMessage(true, "Please, wait... it's not your turn");
            }
            if (IsWinMove(i, j)) {
                handler.BroadcastMessage(false, 
                    "The " + (_isFirstPlayerTurn ? "Second" : "First") + " player won!");
            }
        }

        private bool IsWinMove(int i, int j) {
            var moveType = _cells[i][j];
            return moveType != CellType.Empty && (HasWinningRow(i, j, moveType) 
                || HasWinningColumn(i, j, moveType) || HasWinningDiagonal(i, j, moveType));
        }

        private bool HasWinningRow(int i, int j, CellType moveType) {
            var numOfCellsInRow = 0;
            for (var k = Math.Max(0, i - RequiredToWin + 1); 
                    k < Math.Min(_cells.Count, i + RequiredToWin); ++k) {
                if (_cells[k][j] == moveType) {
                    numOfCellsInRow += 1;
                    if (numOfCellsInRow == RequiredToWin) {
                        return true;
                    }
                } else {
                    numOfCellsInRow = 0;
                }
            }
            return false;
        }

        private bool HasWinningColumn(int i, int j, CellType moveType) {
            var numOfCellsInColumn = 0;
            for (var k = Math.Max(0, j - RequiredToWin + 1); 
                    k < Math.Min(_cells.Count, j + RequiredToWin); ++k) {
                if (_cells[i][k] == moveType) {
                    numOfCellsInColumn += 1;
                    if (numOfCellsInColumn == RequiredToWin) {
                        return true;
                    }
                } else {
                    numOfCellsInColumn = 0;
                }
            }
            return false;
        }

        private bool HasWinningDiagonal(int i, int j, CellType moveType) {
            var numOfCellsInFirstDiagonal = 0;
            var numOfCellsInSecondDiagonal = 0;
            for (var k = -RequiredToWin + 1; k < RequiredToWin; ++k) {
                if (i + k >= 0 && i + k < _cells.Count && j + k >= 0 
                        && j + k < _cells.Count && _cells[i + k][j + k] == moveType) {
                    numOfCellsInFirstDiagonal += 1;
                    if (numOfCellsInFirstDiagonal == RequiredToWin) {
                        return true;
                    }
                } else {
                    numOfCellsInFirstDiagonal = 0;
                }
                if (i + k >= 0 && i + k < _cells.Count && j - k >= 0
                        && j - k < _cells.Count && _cells[i + k][j - k] == moveType) {
                    numOfCellsInSecondDiagonal += 1;
                    if (numOfCellsInSecondDiagonal == RequiredToWin) {
                        return true;
                    }
                } else {
                    numOfCellsInSecondDiagonal = 0;
                }
            }
            return false;
        }
    }
}
