using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace dashb.ViewModel
{
    internal class ClientVm : INotifyPropertyChanged
    {
        private Ntrip.Client client;

        public ClientVm(Ntrip.Client client)
        {
            this.client = client;
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(Title)));
        }
        
        public void SetClient(Ntrip.Client client)
        {
            this.client = client;
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(Title)));
        }
                
        public event PropertyChangedEventHandler? PropertyChanged;

        public string Title => "Client is " + client.Name;
    }
}
