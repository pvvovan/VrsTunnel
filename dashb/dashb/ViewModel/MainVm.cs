using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace dashb.ViewModel
{
    internal class MainVm
    {
        public ClientVm? SelectedClient { get; set; }

        public ObservableCollection<ClientVm>? Clients { get; set; }
    }
}
