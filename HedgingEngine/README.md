# Hedging-Engine

> _« Je construis, je rebalance, je calcule le PnL, j’exécute les stratégies. »_

### Rôle

-   Définition et simulation de stratégies de couverture
-   Rebalancement dynamique selon calendrier / signaux
-   Calcul du PnL quotidien, PnL explain, attribution des risques
-   Peut utiliser le **PricingEngine** pour recalculer les Greeks à chaque pas de temps

### Entrée

-   Portefeuille initial (positions, instruments)
-   Calendrier de hedging
-   MarketScenario (simulé ou historique)
-   Paramètres de stratégie (delta hedging, fixed, trigger…)

### Sortie

-   Trajectoire PnL
-   Deltas, hedge ratios, trades exécutés
-   Rapport de hedging

📦 Exemple :

```
hedging_engine/
  portfolio/
  strategies/
  pnl/
  rebalancing/
  reporting/
```
