using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Threading.Tasks;

namespace Problem3 {
    /*
     * Dictionary Of Keys based sparse matrix
     */
    public class SparseMatrix<T> {
        private readonly Dictionary<ValueTuple<int, int>, T> _keys 
            = new Dictionary<ValueTuple<int, int>, T>();

        public bool IsCellEmpty(int row, int col) => !_keys.ContainsKey((row, col));

        public T this[int row, int col] {
            get {
                _keys.TryGetValue((row, col), out var res);
                return res;
            }
            set => _keys[(row, col)] = value;
        }

        public IEnumerable<KeyValuePair<ValueTuple<int, int>, T>> AsEnumerable() 
            => _keys.AsEnumerable();
    }
}
