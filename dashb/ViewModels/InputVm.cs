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
    public partial class InputVm(IWnd wnd) : ViewModelBase
    {
        public void Close()
        {
            wnd.Close();
        }

        [ObservableProperty]
        private UserVm? _user;

        [ObservableProperty]
        private RelayCommand? _OkCmd;
    }
}
