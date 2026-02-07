# USER DEFINED
$outDir = "Interface\Translations"

$strings = @('english', 'russian')

ForEach ($string in $strings)
{
    Copy-Item "SDSInterface_$string.txt" -Destination "$outDir\SDSInterface_$string.txt"
}
