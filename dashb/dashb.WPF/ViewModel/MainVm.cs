using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace dashb.ViewModel
{
    internal class MainVm : INotifyPropertyChanged
    {
        private ClientVm? _selectedClient;
        public ClientVm? SelectedClient
        {
            get => _selectedClient;
            set
            {
                _selectedClient = value;
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(SelectedClient)));
            }
        }

        public ObservableCollection<ClientVm>? Clients { get; set; }
        public ObservableCollection<ServerVm>? Servers { get; set; }

        public event PropertyChangedEventHandler? PropertyChanged;
    }
}
