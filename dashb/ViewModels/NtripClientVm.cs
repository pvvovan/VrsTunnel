using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace dashb.ViewModels
{
    public partial class NtripClientVm : UserVm
    {
        public NtripClientVm(RelayCommand<NtripClientVm> removeCmd)
        {
            _removeCmd = removeCmd;
        }

        [ObservableProperty]
        private RelayCommand<NtripClientVm> _removeCmd;
    }
}
