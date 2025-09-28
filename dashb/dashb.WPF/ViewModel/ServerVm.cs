using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace dashb.ViewModel
{
    internal class ServerVm : INotifyPropertyChanged
    {
        private Ntrip.Server server;

        public ServerVm(Ntrip.Server server)
        {
            this.server = server;
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(Caption)));
        }
        
        public void SetServerVm(Ntrip.Server server)
        {
            this.server = server;
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(Caption)));
        }
                
        public event PropertyChangedEventHandler? PropertyChanged;

        public string Caption => "RTK base: " + server.Name;
    }
}
