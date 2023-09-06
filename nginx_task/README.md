# NGINX Task
## Řešení
### 1.
NGINX vyhledává podle klíče, jehož podobu lze nastavit direktivou `proxy_cache_key`. Defaultně vypadá podoba klíče přibližně takto: `$scheme$proxy_host$request_uri`.  
