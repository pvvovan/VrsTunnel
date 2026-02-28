using vm.ViewModels;

namespace test.TestVm;

internal class DialogStub : IDialog
{
    public string InputName = "inputName";
    public string InputPassword = "inputPassword";

    public Task<string> Save()
    {
        throw new NotImplementedException();
    }

    public Task<string> Open()
    {
        throw new NotImplementedException();
    }

    public void Show(InputVm inputVm)
    {
        inputVm.User!.Name = this.InputName;
        inputVm.User!.Password = InputPassword;
        inputVm.OkCmd?.Execute(null);
    }
}
