using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace dashb.Ntrip
{
    public interface IUser
    {
        string Name { get; set; }
        string Password { get; set; }
    }
}
