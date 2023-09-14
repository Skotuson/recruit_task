# DNS Task
## Řešení

Datová struktura serveru, která umožňuje najít odpovědi je ve formě `Trie`. Vybral jsem jí, jelikož se lze vyhnout duplicitním záznamům (adresy začinající na stejný prefix budou využívat stejně větve) a lze efektivně reprezentovat masky různé délky. Použití paměti by šlo zlepšit použitím kompresních metod, poté by struktura byla tzv. `Radix tree/Kompresovaná trie`.  
Časová složitost je `O(n)`, kde `n` je délka nejdelšího slova uloženého ve stromě. Na velikosti routing dat tedy složitost nezáleží.  

---

Samotné řešení je rozdělené do několika částí:

### Pomocné funkce
```cpp
std::string StringRepeat ( const std::string & str, size_t nrepeats );
```
```cpp
std::string HexToBin ( const std::string & hex );
```
### Struktura Subnet
### Struktury TrieNode a Data
### Funkce Route

## Problémy
1) Maska
2) No Backtrack