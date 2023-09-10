# DNS Task
## Řešení
Datová struktura serveru, která umožňuje najít odpovědi je ve formě `Trie`. Vybral jsem jí, jelikož se lze vyhnout duplicitním záznamům, jelikož adresy začinající na stejný prefix budou využívat stejně větve a lze efektivně reprezentovat masky různé délky.  
Časová složitost je `O(n)`, kde `n` je délka nejdelšího slova uloženého ve stromě. Na velikosti routing dat tedy složitost nezáleží.  

## Problémy
1) Maska
