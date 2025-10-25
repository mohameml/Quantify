# MarketEstimationEngine

> 💡 _« Je prends les données de marché historiques → j’en extrais les paramètres calibrés. »_

### Rôle

-   Estimation de volatilités (EWMA, GARCH, realized…)
-   Estimation des corrélations (sample, shrinkage, factor…)
-   Calcul des drifts (CIP, risk-neutral drift…)
-   Calibration éventuelle des modèles vol implicite
-   Vérification et régularisation PSD

### Entrée

-   Données historiques (retours, FX, taux)
-   Méthodologie d’estimation
-   Fenêtres temporelles

### Sortie

-   Market snapshot structuré (spots + vols + corr + drift)
-   Diagnostics (condition number, CI, outliers…)

📦 Exemple :

```
market_estimation_engine/
  vol/
  correlation/
  drift/
  calibration/
  diagnostics/
```
