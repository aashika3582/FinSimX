#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <iomanip>

using namespace std;

vector<double> read_close_prices(const string& filename) {
    vector<double> closes;
    ifstream file(filename);
    string line;

    getline(file, line);  // Skip header

    while (getline(file, line)) {
        stringstream ss(line);
        string date, close_str;
        getline(ss, date, ',');
        getline(ss, close_str, ',');

        try {
            closes.push_back(stod(close_str));
        } catch (...) {
            continue;
        }
    }
    return closes;
}

vector<double> calculate_rsi(const vector<double>& prices, int period = 14) {
    vector<double> rsi(prices.size(), 0.0);
    if (prices.size() <= period) return rsi;

    double gain = 0, loss = 0;
    for (int i = 1; i <= period; ++i) {
        double change = prices[i] - prices[i - 1];
        if (change >= 0) gain += change;
        else loss -= change;
    }

    gain /= period;
    loss /= period;
    rsi[period] = 100.0 - (100.0 / (1.0 + (gain / loss)));

    for (size_t i = period + 1; i < prices.size(); ++i) {
        double change = prices[i] - prices[i - 1];
        double curr_gain = (change >= 0) ? change : 0;
        double curr_loss = (change < 0) ? -change : 0;

        gain = (gain * (period - 1) + curr_gain) / period;
        loss = (loss * (period - 1) + curr_loss) / period;

        if (loss == 0) rsi[i] = 100;
        else rsi[i] = 100.0 - (100.0 / (1.0 + gain / loss));
    }

    return rsi;
}

vector<double> ema(const vector<double>& prices, int period) {
    vector<double> result(prices.size(), 0.0);
    if (prices.size() < period) return result;

    double multiplier = 2.0 / (period + 1);
    double sum = 0.0;

    for (int i = 0; i < period; ++i) sum += prices[i];
    result[period - 1] = sum / period;

    for (size_t i = period; i < prices.size(); ++i)
        result[i] = ((prices[i] - result[i - 1]) * multiplier) + result[i - 1];

    return result;
}

void simulate_trading(const vector<double>& prices, const vector<double>& rsi) {
    int short_ema_period = 12, long_ema_period = 26, signal_period = 9;

    vector<double> short_ema = ema(prices, short_ema_period);
    vector<double> long_ema = ema(prices, long_ema_period);

    vector<double> macd(prices.size(), 0.0);
    for (size_t i = 0; i < prices.size(); ++i)
        macd[i] = short_ema[i] - long_ema[i];

    vector<double> signal = ema(macd, signal_period);

    double cash = 100000.0;
    int shares = 0;

    for (size_t i = 1; i < prices.size(); ++i) {
        if (i <= long_ema_period || i <= signal_period) continue;

        bool buy_signal = (macd[i] > signal[i]) && (macd[i - 1] <= signal[i - 1]) && rsi[i] < 60;
        bool sell_signal = (macd[i] < signal[i]) && (macd[i - 1] >= signal[i - 1]) && rsi[i] > 40;

        if (buy_signal && cash >= prices[i]) {
            shares = cash / prices[i];
            cash -= shares * prices[i];
            cout << "BUY at $" << prices[i] << " (RSI: " << rsi[i] << ", MACD: " << macd[i] << ")\n";
        } else if (sell_signal && shares > 0) {
            cash += shares * prices[i];
            shares = 0;
            cout << "SELL at $" << prices[i] << " (RSI: " << rsi[i] << ", MACD: " << macd[i] << ")\n";
        } else {
            cout << "HOLD at $" << prices[i] << " (RSI: " << rsi[i] << ", MACD: " << macd[i] << ")\n";
        }
    }

    double portfolio_value = cash + shares * prices.back();
    cout << fixed << setprecision(2);
    cout << "\nFinal Portfolio Value: $" << portfolio_value << endl;
}

int main() {
    string file = "aapl_data.csv";
    vector<double> closes = read_close_prices(file);
    if (closes.empty()) {
        cerr << "Error: Couldn't read price data.\n";
        return 1;
    }

    vector<double> rsi = calculate_rsi(closes);
    simulate_trading(closes, rsi);

    return 0;
}
