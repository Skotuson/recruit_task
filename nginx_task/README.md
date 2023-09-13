# NGINX Task
## Řešení
### 1)
**NGINX** vyhledává podle klíče, jehož podobu lze nastavit direktivou `proxy_cache_key`. Defaultně vypadá podoba klíče přibližně takto: `$scheme$proxy_host$request_uri`.  
Samotný klíč se vypočítá aplikováním `MD5 hashe` na řetězec, který je nastavený direktivou `proxy_cache_key`. Tento řetězec se uloží ve struktuře určené `levels` v direktivě `proxy_cache_path`, kde se klíč
použije jako cesta k souboru.
Klíč je uložen ve struktuře `ngx_http_cache_t`, v proměnné `key`. Struktura slouží jako cache kontext ke konkrétnímu requestu, který je uložen ve struktuře `ngx_http_request_t`.  
V paměti je uložen ve složce, která je specifikovaná direktivou `proxy_cache_path`.