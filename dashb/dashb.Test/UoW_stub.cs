using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace dashb.DAL
{
    public class UoW_stub : IUnitOfWork
    {
        public void Commit()
        {
            // Stub
        }

        public void Dispose()
        {
            // Stub
        }

        private readonly Dictionary<Type, object> _repos = new Dictionary<Type, object>();

        public UoW_stub()
        {
            _repos.Add(typeof(Ntrip.Client), new ClientRepo());
            _repos.Add(typeof(Ntrip.Server), new ServerRepo());
        }

        public IRepository<T> Repo<T>() where T : class
        {
            return (IRepository<T>)_repos[typeof(T)];
        }
    }

    internal class ClientRepo : IRepository<Ntrip.Client>
    {
        private List<Ntrip.Client> _clients = new();
        public ClientRepo()
        {
            _clients.Add(new Ntrip.Client("Client 1", "Secret 1"));
            _clients.Add(new Ntrip.Client("Client 2", "Secret 2"));
        }
        public IQueryable<Ntrip.Client> Items => _clients.AsQueryable();

        public void Add(Ntrip.Client item)
        {
            _clients.Add(item);
        }

        public void Remove(Ntrip.Client item)
        {
            _clients.Remove(item);
        }
    }

    internal class ServerRepo : IRepository<Ntrip.Server>
    {
        private List<Ntrip.Server> _clients = new();
        public ServerRepo()
        {
            _clients.Add(new() { Name = "Server 1", Password = "srvSecret 1" });
            _clients.Add(new() { Name = "Server 2", Password = "srvSecret 2" });
            _clients.Add(new() { Name = "Server 3", Password = "srvSecret 3" });
        }
        public IQueryable<Ntrip.Server> Items => _clients.AsQueryable();

        public void Add(Ntrip.Server item)
        {
            _clients.Add(item);
        }

        public void Remove(Ntrip.Server item)
        {
            _clients.Remove(item);
        }
    }
}
