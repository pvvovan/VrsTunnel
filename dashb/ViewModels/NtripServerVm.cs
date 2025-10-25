using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace dashb.ViewModels
{
    public partial class NtripServerVm : UserVm
    {
        public NtripServerVm(RelayCommand<NtripServerVm> removeCmd)
        {
            _removeCmd = removeCmd;
        }

        [ObservableProperty]
        private ObservableCollection<NtripClientVm> _clients = [];

        [ObservableProperty]
        private RelayCommand<NtripServerVm> _removeCmd;
    }
}
