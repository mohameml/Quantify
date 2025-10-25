# Pricing-Enigne

> _« Je reçois un marché + un instrument + une méthode → je retourne un prix et des greeks. »_

### Rôle

-   Évaluation des produits dérivés (pricing)
-   Calcul des Greeks (Δ, Γ, Vega, etc.)
-   Gestion des méthodes numériques : Monte Carlo, PDE, arbre, closed-form
-   Implémentation des modèles (Black-Scholes, Heston, CRR…)
-   Ne contient **aucune logique de marché** (pas de fetching de data ni estimation)

### Entrée

-   Market snapshot (spots, taux, vols, corr)
-   Instrument (legs, payoffs, maturité…)
-   Méthode de pricing

### Sortie

-   Prix, Greeks, paths simulés (si demandé)

📦 **Exemples de dossiers** :

```
pricing_engine/
  models/
  methods/
  instruments/
  pricers/
  ffi/ (ou grpc)
```
