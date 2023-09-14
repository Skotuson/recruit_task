# DNS Task
## Řešení

Datová struktura serveru, která umožňuje najít odpovědi je ve formě `Trie`. Vybral jsem jí, jelikož se lze vyhnout duplicitním záznamům (adresy začinající na stejný prefix budou využívat stejně větve) a lze efektivně reprezentovat masky různé délky. Použití paměti by šlo zlepšit použitím kompresních metod, poté by struktura byla tzv. `Radix tree/Kompresovaná trie`.  
Časová složitost je `O(n)`, kde `n` je délka nejdelšího slova uloženého ve stromě. Na velikosti routing dat tedy složitost nezáleží.  
  
---

## Postup řešení a problémy
Nejprve jsem přemýšlel, jakou datovou strukturu zvolím. Jak jsem psal výše, zvolil jsem `Trii`, jelikož se často opakují prefixy.  
Původně jsem ukládal data v trii po celých 16bitových chuncích, to se ale po částí testování ukázalo jako nevhodné, jelikož to nesprávně zpracovávalo různé masky.  
Další krok byl ukládat data po jednotlivých číslicích adresy, to ale trpělo stejným problémem, který se ale hůř odhaloval.  
Například u subnetů
```
2a04:2e00::\32

a

2a04:2e00::\29
```
zasahuje maska i do posledních 4 bitů, což se tímto nedalo pekně zachytit. Proto jsem nakonec zvolil ukládat adresy jako jejich skutečnou bitovou reprezentaci.  
Tento přístup již odhálí všechny případy, ale původně mé řešení neobsahovalo backtracking, a tedy se při hledání co nejspecifičtějšího subnetu zastavilo. Po přidání backtrackingu
se již podařilo různé masky odlišit a získat správný výsledek.  

