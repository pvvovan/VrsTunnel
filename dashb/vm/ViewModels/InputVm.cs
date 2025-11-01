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
    public partial class InputVm : ViewModelBase
    {
        public IWnd? Wnd;

        public void User_PropertyChanged(object? sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            _CanOkExecute = User is not null && User.Name.Length > 0;
            OkCmd?.NotifyCanExecuteChanged();
        }

        public void Close()
        {
            Wnd?.Close();
        }

        [ObservableProperty]
        private UserVm? _user;

        [ObservableProperty]
        private RelayCommand? _OkCmd;

        private bool _CanOkExecute = false;
        public bool CanOkExecute()
        {
            return _CanOkExecute;
        }
    }
}
