cd $PSScriptRoot

dotnet test --logger html --collect:"XPlat Code Coverage"
