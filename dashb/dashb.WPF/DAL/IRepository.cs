using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace dashb.DAL
{
    internal interface IRepository<T> where T : class
    {
        IQueryable<T> Items { get; }
        void Add(T item);
        void Remove(T item);
    }
}
