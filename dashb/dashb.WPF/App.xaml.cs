using Microsoft.Practices.Unity;
using System.Configuration;
using System.Data;
using System.Windows;
using Unity;

namespace dashb
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        public App() : base()
        {
            //IUnityContainer container = new UnityContainer();
            //container.RegisterType<DAL.IUnitOfWork, DAL.UoW_stub>();
        }
    }

}
