using System.Threading.Tasks;
using Microsoft.AspNetCore.SignalR;

namespace Problem3 {
    public class GameHub : Hub, IGameHandler {
        private readonly GameManager _gameManager;

        public GameHub() {
            _gameManager = GameManager.Instance;
        }
        
        public void Click(int x, int y) {
            _gameManager.OnClick(x, y, Context.ConnectionId, this);
        }

        public void MoveLeft() => _gameManager.OnMoveLeft(Context.ConnectionId, this);
        public void MoveRight() => _gameManager.OnMoveRight(Context.ConnectionId, this);
        public void MoveDown() => _gameManager.OnMoveDown(Context.ConnectionId, this);
        public void MoveUp() => _gameManager.OnMoveUp(Context.ConnectionId, this);

        public override Task OnConnectedAsync() {
            Clients.Client(Context.ConnectionId).InvokeAsync("setBoardSize", 
                                                             GameManager.BoardSize);
            if (!_gameManager.AddPlayer(Context.ConnectionId)) {
                Clients.Client(Context.ConnectionId)
                    .InvokeAsync("broadcastMessage", 
                                 "The max number of players has been exceeded");
            }
            return base.OnConnectedAsync();
        }

        public Task BroadcastMessage(bool isPrivate, string message) {
            if (isPrivate) {
                return Clients.Client(Context.ConnectionId)
                              .InvokeAsync("broadcastMessage", message);
            }
            return Clients.All.InvokeAsync("broadcastMessage", message);
        }

        public Task Clear() {
            return Clients.All.InvokeAsync("clear");
        }

        public Task DrawX(int i, int j) {
            return Clients.All.InvokeAsync("drawX", i, j);
        }

        public Task DrawZero(int i, int j) {
            return Clients.All.InvokeAsync("drawZero", i, j);
        }
    }
}
