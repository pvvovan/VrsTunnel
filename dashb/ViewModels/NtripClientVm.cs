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
        public NtripClientVm(
            RelayCommand<NtripClientVm> removeCmd,
            RelayCommand<NtripClientVm> assignCmd,
            RelayCommand<NtripClientVm> unassignCmd)
        {
            _removeCmd = removeCmd;
            _assignCmd = assignCmd;
            _unassignCmd = unassignCmd;
        }

        [ObservableProperty]
        private RelayCommand<NtripClientVm> _removeCmd;

        [ObservableProperty]
        private RelayCommand<NtripClientVm> _assignCmd;

        [ObservableProperty]
        private RelayCommand<NtripClientVm> _unassignCmd;
    }
}
