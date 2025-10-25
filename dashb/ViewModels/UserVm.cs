using CommunityToolkit.Mvvm.ComponentModel;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace dashb.ViewModels
{
    public partial class UserVm : ViewModelBase
    {
        [ObservableProperty]
        private string _name = string.Empty;

        [ObservableProperty]
        private string _password = string.Empty;
    }
}
