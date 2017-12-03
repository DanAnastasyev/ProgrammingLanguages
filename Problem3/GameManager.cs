using System;
using System.Collections.Generic;

namespace Problem3 {
    public class GameManager {
        private static readonly Lazy<GameManager> _instance =
            new Lazy<GameManager>(() => new GameManager());

        private enum CellType {
            X,
            Zero,
            Empty
        }

        private class PlayerModel {
            public string PlayerId { get; set; }
            public int ViewportLeft { get; set; } = int.MaxValue / 2;
            public int ViewportTop { get; set; } = int.MaxValue / 2;
        }

        public const int BoardSize = 600;
        private const int CellsCount = 10;
        private const int RequiredToWin = 5;
        private const int CellSize = BoardSize / CellsCount;
        private readonly SparseMatrix<CellType> _cells = new SparseMatrix<CellType>();
        private readonly List<PlayerModel> _players = new List<PlayerModel>();
        private bool _isFirstPlayerTurn = true;

        public static GameManager Instance => _instance.Value;

        public bool AddPlayer(string playerId) {
            if (_players.Count >= 2) {
                return false;
            }
            _players.Add(new PlayerModel{ PlayerId = playerId });
            return true;
        }

        public void OnClick(int x, int y, string playerId, IGameHandler handler) {
            if (_players.Count < 2) {
                handler.BroadcastMessage(true,
                    "Please, wait for the second player before starting the game");
                return;
            }
            var i = x / CellSize + _players[0].ViewportLeft;
            var j = y / CellSize + _players[0].ViewportTop;
            UpdateCell(i, j, playerId, handler);
            if (IsWinMove(i, j)) {
                handler.BroadcastMessage(false, 
                    "The " + (_isFirstPlayerTurn ? "Second" : "First") + " player won!");
            }
        }

        public void OnMoveLeft(string playerId, IGameHandler handler) {
            _players[0].ViewportLeft -= 1;
            Redraw(handler);
        }

        public void OnMoveRight(string playerId, IGameHandler handler) {
            _players[0].ViewportLeft += 1;
            Redraw(handler);
        }

        public void OnMoveDown(string playerId, IGameHandler handler) {
            _players[0].ViewportTop += 1;
            Redraw(handler);
        }

        public void OnMoveUp(string playerId, IGameHandler handler) {
            _players[0].ViewportTop -= 1;
            Redraw(handler);
        }

        private void Redraw(IGameHandler handler) {
            handler.Clear();
            foreach (var cell in _cells.AsEnumerable()) {
                int i = cell.Key.Item1 - _players[0].ViewportLeft;
                int j = cell.Key.Item2 - _players[0].ViewportTop;
                if (0 > i || i >= CellsCount || 0 > j || j >= CellsCount) {
                    continue;
                }
                if (cell.Value == CellType.X) {
                    handler.DrawX(i, j);
                } else {
                    handler.DrawZero(i, j);
                }
            }
        }

        private void UpdateCell(int i, int j, string playerId, IGameHandler handler) {
            if (_isFirstPlayerTurn && _players[0].PlayerId == playerId) {
                _cells[i, j] = CellType.Zero;
                handler.DrawZero(i - _players[0].ViewportLeft, j - _players[0].ViewportTop);
                _isFirstPlayerTurn = false;
            } else if (!_isFirstPlayerTurn && _players[1].PlayerId == playerId) {
                _cells[i, j] = CellType.X;
                handler.DrawX(i - _players[0].ViewportLeft, j - _players[0].ViewportTop);
                _isFirstPlayerTurn = true;
            } else {
                handler.BroadcastMessage(true, "Please, wait... it's not your turn");
            }
        }

        private bool IsWinMove(int i, int j) {
            var moveType = _cells[i, j];
            return moveType != CellType.Empty && (HasWinningRow(i, j, moveType) 
                || HasWinningColumn(i, j, moveType) || HasWinningDiagonal(i, j, moveType));
        }

        private bool HasWinningRow(int i, int j, CellType moveType) {
            var numOfCellsInRow = 0;
            var startIndex = Math.Max(0, i - RequiredToWin + 1);
            var endIndex = i < int.MaxValue - RequiredToWin ? i + RequiredToWin 
                                                            : int.MaxValue;
            for (var k = startIndex; k < endIndex; ++k) {
                if (IsCellValueEqual(k, j, moveType)) {
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
            var startIndex = Math.Max(0, j - RequiredToWin + 1);
            var endIndex = j < int.MaxValue - RequiredToWin ? j + RequiredToWin 
                                                            : int.MaxValue;
            for (var k = startIndex; k < endIndex; ++k) {
                if (IsCellValueEqual(i, k, moveType)) {
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
                if (IsValidOffset(i, k) && IsValidOffset(j, k) 
                        && IsCellValueEqual(i + k, j + k, moveType)) {
                    numOfCellsInFirstDiagonal += 1;
                    if (numOfCellsInFirstDiagonal == RequiredToWin) {
                        return true;
                    }
                } else {
                    numOfCellsInFirstDiagonal = 0;
                }
                if (IsValidOffset(i, k) && j - k >= 0 
                        && IsCellValueEqual(i + k, j - k, moveType)) {
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

        private static bool IsValidOffset(int index, int offset)
            => index + offset >= 0 && index < int.MaxValue - offset;

        private bool IsCellValueEqual(int i, int j, CellType expected) 
            => !_cells.IsCellEmpty(i, j) && _cells[i, j] == expected;
    }
}
