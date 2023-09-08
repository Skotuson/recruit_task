# NGINX Task
## Řešení
### 1)
**NGINX** vyhledává podle klíče, jehož podobu lze nastavit direktivou `proxy_cache_key`. Defaultně vypadá podoba klíče přibližně takto: `$scheme$proxy_host$request_uri`.  
Samotný klíč se vypočítá aplikováním `MD5 hashe` na řetězec, který je nastavený direktivou `proxy_cache_key`. Používá se jako název zacachovaného souboru.
Klíč je uložen ve struktuře `ngx_http_cache_t`, v proměnné `key`. Struktura slouží jako cache kontext ke konkrétnímu requestu, který je uložen ve struktuře `ngx_http_request_t`.