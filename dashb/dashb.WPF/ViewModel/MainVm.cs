using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

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

        private string _NewName = "Enter new name";
        public string NewName
        {
            get
            {
                return _NewName;
            }
            set
            {
                _NewName = value;
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(_NewName)));
                AddClientCmd.OnCanExecuteChanged();
            }
        }

        private CmdVm? _AddClientCmd;
        public CmdVm AddClientCmd
        {
            get
            {
                _AddClientCmd ??= new(p => NewName.Length > 0, p => AddClient());
                return _AddClientCmd;
            }
        }

        private void AddClient()
        {
            MessageBox.Show("Client added: " + NewName);
        }
    }
}
