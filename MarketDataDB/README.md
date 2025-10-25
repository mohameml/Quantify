## 🧱 4. **MarketDataEngine** 🌍 _(responsable de la donnée brute)_

> 💡 _« Je collecte, nettoie et standardise la donnée de marché. »_

### Rôle

-   Connexion aux fournisseurs de données (API, fichiers, base interne)
-   Téléchargement / ingestion des prix historiques
-   Nettoyage, alignement, ajustement (split, dividendes)
-   Stockage et mise à disposition normalisée (parquet, SQL, cache, S3…)

### Entrée

-   Requête d’univers (tickers, sous-jacents, dates…)

### Sortie

-   DataFrame clean et alignée
-   Métadonnées (qualité, missing values…)

📦 Exemple :

```
market_data_engine/
  adapters/ (Polygon, Bloomberg, ECB…)
  cleaning/
  calendar/
  storage/
```
