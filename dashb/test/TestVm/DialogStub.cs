using vm.ViewModels;

namespace test.TestVm;

internal class DialogStub : IDialog
{
    public string InputName = "inputName";
    public string InputPassword = "inputPassword";

    public void Show(InputVm inputVm)
    {
        inputVm.User!.Name = this.InputName;
        inputVm.User!.Password = InputPassword;
        inputVm.OkCmd?.Execute(null);
    }
}
