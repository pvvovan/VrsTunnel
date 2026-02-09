Push-Location
Set-Location $PSScriptRoot

dotnet test --logger html --collect:"XPlat Code Coverage"

Pop-Location
