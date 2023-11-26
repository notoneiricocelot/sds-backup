# USER DEFINED
$outDir = "SDS\Interface\Translations"

$strings = @('czech', 'english', 'french', 'german', 'italian', 'japanese', 'polish', 'russian', 'spanish')

ForEach ($string in $strings)
{
    Copy-Item "sds_interfaces_english.txt" -Destination "$outDir\SDS_$string.txt"
}
