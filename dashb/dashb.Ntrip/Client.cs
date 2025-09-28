using System;
using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace dashb.Ntrip
{
    public class Client : IUser
    {
        [SetsRequiredMembers]
        public Client(string name, string password) =>
        (Name, Password) = (name, password);

        public required string Name { get; set; }
        public required string Password { get; set; }
    }
}
