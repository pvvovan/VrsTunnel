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
            RelayCommand<NtripClientVm> unassignCmd,
            Models.NtripClient? model)
        {
            _removeCmd = removeCmd;
            _assignCmd = assignCmd;
            _unassignCmd = unassignCmd;
            Model = model;
            if (Model is not null)
            {
                Name = Model.Name;
                Password = Model.Password;
            }
        }

        public readonly Models.NtripClient? Model;

        [ObservableProperty]
        private RelayCommand<NtripClientVm> _removeCmd;

        [ObservableProperty]
        private RelayCommand<NtripClientVm> _assignCmd;

        [ObservableProperty]
        private RelayCommand<NtripClientVm> _unassignCmd;
    }
}
