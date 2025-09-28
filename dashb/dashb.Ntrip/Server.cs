using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace dashb.Ntrip
{
    public class Server : IUser
    {
        private string _name = string.Empty;
        private string _password = string.Empty;
        public string Name { get => _name; set => _name = value; }
        public string Password { get => _password; set => _password = value; }
    }
}
