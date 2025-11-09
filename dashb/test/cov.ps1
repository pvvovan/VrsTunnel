cd $PSScriptRoot

$COVDATA=$args[0]

reportgenerator `
-reports:"$COVDATA" `
-targetdir:"TestResults/coveragereport" `
-reporttypes: Html
