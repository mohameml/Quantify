import QuantLib as ql
import numpy as np

# 1. Définir les paramètres de la simulation
evaluation_date = ql.Date(8, 12, 2024)  # Date d'évaluation
ql.Settings.instance().evaluationDate = evaluation_date

# 2. Définir les actifs sous-jacents
spot_prices = [100, 120]  # Prix spot des actifs sous-jacents
volatilities = [0.2, 0.25]  # Volatilités des actifs
correlation_matrix = [[1.0, 0.3], [0.3, 1.0]]  # Matrice de corrélation
dividends = [0.01, 0.02]  # Rendements de dividendes
weights = [0.5, 0.5]  # Pondérations dans la basket

# 3. Définir le processus stochastique (Black-Scholes multidimensionnel)
risk_free_rate = 0.05  # Taux sans risque
day_count = ql.Actual365Fixed()

spot_handles = [ql.QuoteHandle(ql.SimpleQuote(price)) for price in spot_prices]

vol_handles = [ql.BlackVolTermStructureHandle(ql.BlackConstantVol(evaluation_date, ql.NullCalendar(), vol, day_count)) for vol in volatilities]

dividend_handles = [ql.YieldTermStructureHandle(ql.FlatForward(evaluation_date, div, day_count))  for div in dividends]

risk_free_handle = ql.YieldTermStructureHandle(ql.FlatForward(evaluation_date, risk_free_rate, day_count))

# Création du processus stochastique
processes = [ql.BlackScholesMertonProcess(spot, div, risk_free_handle, vol)  for spot, div, vol in zip(spot_handles, dividend_handles, vol_handles)]

# Création de la matrice de corrélation
correlation_matrix = ql.Matrix(len(spot_prices), len(spot_prices))
for i in range(len(spot_prices)):
    for j in range(len(spot_prices)):
        correlation_matrix[i][j] = correlation_matrix[i][j]

# Processus joint
joint_process = ql.StochasticProcessArray(processes, correlation_matrix)

# 4. Définir la date d'expiration et la payoff de la Basket Option
maturity_date = ql.Date(8, 12, 2025)  # Date d'expiration
#  basket_payoff = ql.PlainVanillaPayoff(ql.Option.Call, strike=100)  # Payoff Call avec strike 100
basket_payoff = ql.MaxBasketPayoff(ql.PlainVanillaPayoff(ql.Option.Call, strike=100))

# Pondération des actifs
basket_engine = ql.MCEuropeanBasketEngine(
    joint_process, "pseudorandom", timeSteps=1, requiredSamples=100000
)

# Définir la Basket Option
basket_option = ql.BasketOption(basket_payoff, weights)

# 5. Calcul du prix
basket_option.setPricingEngine(basket_engine)
price = basket_option.NPV()

# 6. Affichage du prix
print(f"Le prix de la Basket Option Call est de : {price:.2f}")
