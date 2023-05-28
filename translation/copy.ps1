# USER DEFINED
$outDir = "SDS\Interface\Translations"

$strings = @('czech', 'english', 'french', 'german', 'italian', 'japanese', 'polish', 'russian', 'spanish')

ForEach ($string in $strings)
{
    Copy-Item "SDS_english.txt" -Destination "$outDir\SDS_$string.txt"
}
