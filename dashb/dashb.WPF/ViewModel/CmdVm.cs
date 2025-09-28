using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace dashb.ViewModel
{
    public class CmdVm : ICommand
    {
        public CmdVm(Predicate<object?> canExecute, Action<object?> execute)
        {
            _CanExecute = canExecute;
            _Execute = execute;
        }

        public event EventHandler? CanExecuteChanged;
        public void OnCanExecuteChanged()
        {
            CanExecuteChanged?.Invoke(this, EventArgs.Empty);
        }

        private readonly Predicate<object?> _CanExecute;
        public bool CanExecute(object? parameter)
        {
            return _CanExecute == null || _CanExecute(parameter);
        }

        private Action<object?> _Execute;
        public void Execute(object? parameter)
        {
            _Execute?.Invoke(parameter);
        }
    }
}
