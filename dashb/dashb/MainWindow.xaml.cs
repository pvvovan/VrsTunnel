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
            Ntrip.Client client = new(name: "RTK rover", password: "psw");
            client.ToString();

            Ntrip.Server server = new()
            {
                Name = "server",
                Password = "Password"
            };
            server.ToString();

            ViewModel.ClientVm clientVm = new(client);
            ViewModel.MainVm mainVm = new();
            mainVm.SelectedClient = clientVm;
            DataContext = mainVm;
        }
    }
}
