using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace dashb.Ntrip
{
    internal interface IUser
    {
        string Name { get; set; }
        string Password { get; set; }
    }
}
