using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.SignalR;

namespace Problem3 {
    public class GameHub : Hub {
        public void Send(string name, string message) {
            Clients.All.InvokeAsync("broadcastMessage", name, message);
        }

        public Task Click(int x, int y)
        {
            return Clients.All.InvokeAsync("broadcastMessage", $"click in {x}, {y}");
        }

        public override Task OnConnectedAsync()
        {
            Clients.All.InvokeAsync("broadcastMessage", "system", $"{Context.ConnectionId} joined the conversation");
            return base.OnConnectedAsync();
        }
    }
}
