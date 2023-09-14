# NGINX Task
## Řešení
### 1)
**NGINX** vyhledává podle klíče, jehož podobu lze nastavit direktivou `proxy_cache_key`. Defaultně vypadá podoba klíče přibližně takto: `$scheme$proxy_host$request_uri`.  
Samotný klíč se vypočítá aplikováním `MD5 hashe` na řetězec, který je nastavený direktivou `proxy_cache_key`. Tento řetězec se uloží ve struktuře určené v direktivě `proxy_cache_path`, konkrétně parametr `levels`, kde se klíč
použije jako cesta k souboru.   

---
`levels` bere až `3` úrovně, do každé z nich jde dát hodnota `1` nebo `2`. Pro nastavení: 
```
proxy_cache_path /data/nginx/cache levels=1:2
```
by klíč `b7f54b2df7773722d382f4809d65029c` byl uložen v následujicí struktuře: `/data/nginx/cache/c/29/b7f54b2df7773722d382f4809d65029c`.

Pro nastavení:
```
proxy_cache_path /data/nginx/cache levels=2:2:2
```
by klíč `b7f54b2df7773722d382f4809d65029c` byl uložen v následujicí struktuře: `/data/nginx/cache/9c/02/65/b7f54b2df7773722d382f4809d65029c`.

---

Klíč je uložen ve struktuře `ngx_http_cache_t`, v proměnné `key`. Struktura slouží jako cache kontext ke konkrétnímu requestu, který je uložen ve struktuře `ngx_http_request_t`.  

### 2)

### 3)
**NGINX** využívá hash tabulku uloženou ve struktuře `ngx_hash_wildcard_t`, která v sobě má samotnou hashovací tabulku (struktura `ngx_hash_t`) a další informace v proměnné `value`.

## Postup řešení a problémy
