# FinSimX
A stock trading strategy simulator using real AAPL data, built with Python and C++
#  Stock Trading Simulator (RSI + MACD Strategy)

This project is a **C++ stock trading simulator** that uses historical **AAPL (Apple Inc.)** data and applies a combined **RSI (Relative Strength Index)** and **MACD (Moving Average Convergence Divergence)** strategy to make **Buy / Sell / Hold** decisions. It simulates portfolio performance based on those signals.

---

##  Features

- Reads historical price data from a CSV file (`aapl_data.csv`)
- Calculates RSI and MACD indicators
- Executes trade logic with clear **Buy**, **Sell**, and **Hold** signals
- Simulates a portfolio starting with $100,000
- Logs all trading actions and final portfolio value

---

## Strategy Logic

**RSI Thresholds**:
- `RSI < 60` → *BUY* signal
- `RSI > 40` → *SELL* signal

**MACD Confirmation**:
- *BUY* if `MACD > Signal` **and** `RSI < 60`
- *SELL* if `MACD < Signal` **and** `RSI > 40`
- *HOLD* in all other cases

This combination provides realistic market behavior with diverse trade signals.





