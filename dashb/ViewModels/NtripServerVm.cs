using CommunityToolkit.Mvvm.ComponentModel;
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
        [ObservableProperty]
        private ObservableCollection<NtripClientVm> _clients = [];
    }
}
