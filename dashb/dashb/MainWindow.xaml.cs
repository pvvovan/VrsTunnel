using System.Collections.ObjectModel;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace dashb
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
            DAL.IUnitOfWork unitOfWork = new DAL.UoW_stub();

            Ntrip.Server server = new()
            {
                Name = "server",
                Password = "Password"
            };
            server.ToString();

            ViewModel.ClientVm clientVm = new(unitOfWork.Repo<Ntrip.Client>().Items.Last());
            ViewModel.MainVm mainVm = new();
            mainVm.SelectedClient = clientVm;

            mainVm.Clients = new ObservableCollection<ViewModel.ClientVm>(
                from cl in unitOfWork.Repo<Ntrip.Client>().Items
                select new ViewModel.ClientVm(cl));

            DataContext = mainVm;
        }
    }
}
