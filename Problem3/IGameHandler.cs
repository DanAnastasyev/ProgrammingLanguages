using System.Threading.Tasks;

namespace Problem3 {
    public interface IGameHandler {
        Task BroadcastMessage(bool isPrivate, string message);
        Task Clear();
        Task DrawX(int i, int j);
        Task DrawZero(int i, int j);
    }
}
