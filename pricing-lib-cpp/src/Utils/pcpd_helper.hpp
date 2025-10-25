#pragma once
#include <iostream>
#include "pnl/pnl_vector.h"

class PricingResults
{
  private:
    double price;
    const PnlVect* delta;
    double priceStdDev;
    const PnlVect* deltaStdDev;

  public:
    PricingResults(double p_price, double p_priceStdDev, const PnlVect* const p_delta, const PnlVect* const p_deltaStdDev)
      : price(p_price)
      , priceStdDev(p_priceStdDev)
      , delta(p_delta)
      , deltaStdDev(p_deltaStdDev)
    {
    }

    friend std::ostream& operator<<(std::ostream& stm, const PricingResults& res);
};

class HedgingResults
{
  private:
    double initialPrice;
    double initialPriceStdDev;
    double finalPnL;

  public:
    HedgingResults(double p_initialPrice, double p_initialPriceStdDev, double p_finalPnL)
      : initialPrice(p_initialPrice)
      , initialPriceStdDev(p_initialPriceStdDev)
      , finalPnL(p_finalPnL)
    {
    }

    friend std::ostream& operator<<(std::ostream& o, const HedgingResults& res);
};
