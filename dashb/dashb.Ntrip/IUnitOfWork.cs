using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace dashb.DAL
{
    public interface IUnitOfWork : IDisposable
    {
        void Commit();
        IRepository<T> Repo<T>() where T : class;
    }
}
