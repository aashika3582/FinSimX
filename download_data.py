import yfinance as yf
import pandas as pd

ticker = "AAPL"
data = yf.download(ticker, period="6mo", interval="1d")

# Save only Date and Close columns
data = data.reset_index()[["Date", "Close"]]
data.to_csv("aapl_data.csv", index=False)
